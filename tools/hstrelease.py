#!/usr/bin/env python3
"""Maintainer tool: publish the full branch to the public repository in one command.

    python tools/hstrelease.py                     dry run (the default): nothing is committed, synced or pushed
    python tools/hstrelease.py --apply -m MSG      commit on full, sync the public worktree and commit it there
    python tools/hstrelease.py --push -m MSG       --apply, then push `public` as the remote's main and verify it

Run it from the maintainers' checkout of `full`. The steps (each refuses, exit 1, on a problem):

 1. Push preparation: `bash tools/hstpushprep.sh`, then `bash tools/hstpushprep.sh stage`; commit on full when
    anything is staged (message: -m). The dry run skips this step (push preparation takes the shared lock and runs the
    clone passes) and uses the checkout's HEAD.
 2. A clean temporary git worktree of that commit (the export copies the working tree, so a lane's uncommitted work
    must not reach it), with the git-ignored inputs copied in: hst-config.json, try/build/*report.json,
    try/port-report.json and try/port-private-report.json. Without them T2/T3 and the portable-source milestone
    silently drop to 0, so a missing input refuses. Then `hstprogress.py --write` there, which also rewrites the
    generated figure blocks (hstprogress.BLOCK_FILES), and `hstprogress.py --check-readme`. Then licence
    compliance: `hstreuse.py check` there must be COMPLIANT (any PROBLEM line, a non-zero exit or a missing verdict
    refuses), so nothing is synced or committed on the public branch from a commit that is not.
 3. Never-lower guard: every figure of progress.json, docs/milestones.json and docs/objdiff-report.json is compared
    with the last published one (HEAD of the public worktree); a figure that goes down or becomes null refuses.
 4. `hstpublic.py sync` from the clean worktree into the public worktree (the dry run mirrors into a detached
    temporary worktree of the public HEAD instead and commits nothing), then `hstreuse.py check` on the synced public
    tree (what is pushed) must be COMPLIANT too, then `hstmatch.py` on every added or modified src/**/*.c|cpp of the
    public tree: every one must be EXACT.
 5. --apply/--push: the refreshed figure files are committed on full too (refused if full's copies changed meanwhile).
 6. --push: `git push origin public:main`, then `git ls-remote origin main` must equal the public HEAD; prints the
    `gh run list` command for the Progress report workflow.

The public worktree is the worktree of this repository that has the `public` branch checked out (--public overrides).
The push preparation lock (try/auto.lock) is released at the end of an --apply/--push run.
"""
from __future__ import annotations

import argparse
import datetime
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FIGURE_FILES = ("progress.json", "docs/milestones.json", "docs/objdiff-report.json")
IGNORED_INPUTS = ("hst-config.json", "try/build/*report.json", "try/port-report.json", "try/port-private-report.json")
SOURCE = re.compile(r"^src/.+\.(c|cpp)$")
TRAILER = re.compile(r"^[A-Z][\w-]*: .+$")
WORKFLOW = "progress-report.yml"


def bash() -> str:
    """The shell that runs tools/hstpushprep.sh. On Windows a bare `bash` from Python resolves to WSL's
    System32\\bash.exe, where the config's Windows paths (campaign_root) do not exist and the promotion gate refuses
    (2026-10-22); use Git for Windows' bash, as the maintainers do by hand. HST_BASH overrides it."""
    if os.environ.get("HST_BASH"):
        return os.environ["HST_BASH"]
    if os.name == "nt":
        for candidate in (Path(os.environ.get("ProgramFiles", r"C:\Program Files")) / "Git" / "bin" / "bash.exe",
                          Path(shutil.which("git") or "").parent.parent / "bin" / "bash.exe"):
            if candidate.is_file():
                return str(candidate)
        raise Refused("Git for Windows' bash.exe was not found; set HST_BASH to it (WSL's bash cannot run the "
                      "push preparation with Windows config paths)")
    return "bash"


class Refused(Exception):
    pass


def git(cwd: Path, *args: str, check: bool = True) -> str:
    r = subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True, encoding="utf-8", errors="replace")
    if check and r.returncode:
        raise Refused(f"git {' '.join(args)} failed in {cwd}:\n{r.stderr.strip()}")
    return r.stdout


MATCH_CHUNK = 300   # sources per hstmatch.py call in step 4


def run(cmd: list[str], cwd: Path) -> tuple[int, str]:
    """Run `cmd` and echo its output; (exit status, output)."""
    print(f"$ {' '.join(cmd)}", flush=True)
    r = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, encoding="utf-8",
                       errors="replace")
    out = r.stdout.replace("\r\n", "\n")
    print(out, end="" if out.endswith("\n") or not out else "\n", flush=True)
    return r.returncode, out


def step(n: int, title: str) -> None:
    print(f"\n== {n}. {title}", flush=True)


# --------------------------------------------------------------------------------------------------------------
# never-lower guard

def figures(name: str, doc) -> dict[str, float | None]:
    """{figure key: value} of one figure file. Counts that may fall as work is done (the open candidate spans and
    the missing imports in the milestones' detail text) are not figures; a milestone's reached date and regressed
    flag are (a date that disappears or a flag that turns on is a drop)."""
    out: dict[str, float | None] = {}
    if name == "progress.json":
        m = re.fullmatch(r"([\d.]+)% \(tier 1: ([\d.]+)%\)", str(doc.get("message", "")))
        out["strict_percent"] = float(m.group(1)) if m else None
        out["t1_percent"] = float(m.group(2)) if m else None
    elif name == "docs/milestones.json":
        for ms in doc.get("milestones", []):
            out[f"{ms['id']}.progress"] = ms.get("progress")
            out[f"{ms['id']}.reached"] = 1.0 if ms.get("reached") else 0.0
            out[f"{ms['id']}.not_regressed"] = 0.0 if ms.get("regressed") else 1.0
    elif name == "docs/objdiff-report.json":
        # The layout of the units is not a figure: a function that becomes matched leaves its not-yet-recovered unit
        # (the unit vanishes, `total_units` falls), and an SDK-class match leaves a recovered file's unit for the
        # unlisted SDK unit (that unit's total_* fall). moved() checks instead that every such function is still
        # accounted for. The aggregate measures and every unit's match measures stay figures.
        def measures(prefix: str, m: dict, unit: bool) -> None:
            for k, v in m.items():
                if (unit and k.startswith("total_")) or (not unit and k == "total_units"):
                    continue
                out[f"{prefix}{k}"] = None if v is None else float(v)
        measures("", doc.get("measures", {}), False)
        for unit in doc.get("units", []):
            measures(f"{unit['name']}.", unit.get("measures", {}), True)
    return out


def _module(unit: dict) -> str:
    return (unit.get("metadata") or {}).get("module_name") or unit["name"].split("/", 1)[0]


def _functions(unit: dict) -> dict[str, tuple[str, int, bool]]:
    """{address: (name, size, matched)} of a unit's listed functions; matched means fuzzy_match_percent 100."""
    out = {}
    for f in unit.get("functions") or []:
        address = str((f.get("metadata") or {}).get("virtual_address", f.get("address", f["name"])))
        out[address] = (f["name"], int(f.get("size") or 0), float(f.get("fuzzy_match_percent") or 0.0) >= 100.0)
    return out


def moved(name: str, old_doc, new_doc) -> list[str]:
    """Every function listed in a published unit of the objdiff report that is no longer listed in that unit and is
    not accounted for in the candidate. Accounted for means: listed in another unit of the same module, matched, at
    least as large as before; or, for a function that was unmatched, absorbed by the module's unlisted unit (an
    auto-generated unit that lists no functions, the SDK code that is counted but not published), whose
    total_functions and total_code grow by at least the absorbed count and bytes."""
    if name != "docs/objdiff-report.json":
        return []
    old_units = {u["name"]: u for u in old_doc.get("units", [])}
    new_units = {u["name"]: u for u in new_doc.get("units", [])}
    listed: dict[tuple[str, str], tuple[str, str, int, bool]] = {}
    for u in new_units.values():
        for address, (fname, size, matched) in _functions(u).items():
            listed[(_module(u), address)] = (u["name"], fname, size, matched)
    out = []
    pending: list[tuple[str, str]] = []  # (module, description) of unmatched functions no unit lists any more
    absorbed: dict[str, list[int]] = {}
    for uname, u in old_units.items():
        still = _functions(new_units[uname]) if uname in new_units else {}
        for address, (fname, size, matched) in _functions(u).items():
            if address in still:
                continue
            where = f"{name} {uname}: function {fname} ({size} B)"
            now = listed.get((_module(u), address))
            if now is not None:
                nunit, _, nsize, nmatched = now
                if not nmatched or nsize < size:
                    out.append(f"{where} moved to {nunit} but is not matched there at {size} B or more")
            elif not matched:
                acc = absorbed.setdefault(_module(u), [0, 0])
                acc[0] += 1
                acc[1] += size
                pending.append((_module(u), where))
            else:
                out.append(f"{where} disappeared from the report (it was matched)")
    grown = {}
    for uname, u in new_units.items():
        if u.get("functions") or not (u.get("metadata") or {}).get("auto_generated"):
            continue
        before = (old_units.get(uname) or {}).get("measures", {})
        after = u.get("measures", {})
        g = grown.setdefault(_module(u), [0, 0])
        g[0] += int(after.get("total_functions") or 0) - int(before.get("total_functions") or 0)
        g[1] += int(after.get("total_code") or 0) - int(before.get("total_code") or 0)
    for module, where in pending:
        count, size = absorbed[module]
        g = grown.get(module, [0, 0])
        if g[0] < count or g[1] < size:
            out.append(f"{where} disappeared from the report (the module's unlisted units grew by {g[0]} "
                       f"functions / {g[1]} B, {count} functions / {size} B left the listed units)")
    return out


def lowered(name: str, old_doc, new_doc) -> list[str]:
    """Every figure of `name` that is lower in `new_doc` than in `old_doc`, null, or gone."""
    old, new = figures(name, old_doc), figures(name, new_doc)
    out = []
    for key, was in old.items():
        if was is None:
            continue
        now = new.get(key)
        if key not in new:
            out.append(f"{name} {key}: {was:g} published, now missing")
        elif now is None:
            out.append(f"{name} {key}: {was:g} published, now null")
        elif now < was - 1e-9:
            out.append(f"{name} {key}: {was:g} published, now {now:g}")
    return out


def guard(published: dict[str, str | None], candidate: dict[str, str]) -> list[str]:
    """Drops of every FIGURE_FILES figure from the published text to the candidate text ({path: text}; a path that
    was never published has no baseline)."""
    out = []
    for name in FIGURE_FILES:
        if published.get(name) is None:
            continue
        if name not in candidate:
            out.append(f"{name}: published, now missing")
            continue
        old_doc, new_doc = json.loads(published[name]), json.loads(candidate[name])
        out += lowered(name, old_doc, new_doc) + moved(name, old_doc, new_doc)
    return out


# --------------------------------------------------------------------------------------------------------------
# steps

def public_worktree(repo: Path) -> Path:
    path = None
    for line in git(repo, "worktree", "list", "--porcelain").splitlines():
        if line.startswith("worktree "):
            path = Path(line[len("worktree "):])
        elif line == "branch refs/heads/public" and path is not None:
            return path
    raise Refused("no worktree of this repository has the public branch checked out (pass --public)")


def push_preparation(repo: Path, message: str | None, apply: bool) -> str:
    """Step 1; the commit to publish."""
    branch = git(repo, "rev-parse", "--abbrev-ref", "HEAD").strip()
    if not apply:
        changed = [ln for ln in git(repo, "status", "--porcelain", "--", "src").splitlines() if ln.strip()]
        print(f"dry run: push preparation skipped (it takes try/auto.lock and runs the clone passes); {branch} has "
              f"{len(changed)} changed path(s) under src/ that --apply would send through the promotion gate")
        return git(repo, "rev-parse", "HEAD").strip()
    if branch != "full":
        raise Refused(f"{repo} has {branch} checked out, not full")
    rc, out = run([bash(), "tools/hstpushprep.sh"], repo)
    if "PROMOTION REFUSED" in out:
        raise Refused("the promotion gate refused (see try/pp-promote.log); nothing was staged")
    rc, out = run([bash(), "tools/hstpushprep.sh", "stage"], repo)
    if rc:
        raise Refused(f"push preparation `stage` failed (exit {rc})")
    scan = out.split("\nNUL:", 1)[1] if "\nNUL:" in out else ""
    hits = [ln for ln in scan.splitlines() if ln.strip() and ln.strip() not in ("CRLF:", "PATTERNS:")]
    if hits:
        raise Refused("the staged-blob scan found NUL bytes, CRLF line ends or private patterns:\n  "
                      + "\n  ".join(hits[:10]) + "\n(the index is left staged for review)")
    if subprocess.run(["git", "diff", "--cached", "--quiet"], cwd=repo).returncode:
        subprocess.run(["git", "commit", "-q", "-F", "-"], cwd=repo, input=message, text=True, check=True)
        print(git(repo, "log", "-1", "--oneline").strip())
    else:
        print("nothing staged; publishing HEAD")
    return git(repo, "rev-parse", "HEAD").strip()


def clean_worktree(repo: Path, commit: str, tmp: Path, date: str) -> Path:
    """Step 2; the clean worktree, its figures written and checked."""
    clean = tmp / "full"
    git(repo, "worktree", "add", "--detach", str(clean), commit)
    print(f"clean worktree of {commit[:12]} at {clean}")
    copied = []
    for pattern in IGNORED_INPUTS:
        found = sorted(repo.glob(pattern))
        if not found:
            raise Refused(f"git-ignored input {pattern} is missing in {repo}; without it the published figures drop")
        for src in found:
            rel = src.relative_to(repo)
            (clean / rel).parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(src, clean / rel)
            copied.append(rel.as_posix())
    print(f"copied {len(copied)} git-ignored input(s): {', '.join(copied)}")
    rc, _ = run([sys.executable, "tools/hstprogress.py", "--write", "--date", date], clean)
    if rc:
        raise Refused(f"hstprogress.py --write failed (exit {rc})")
    rc, _ = run([sys.executable, "tools/hstprogress.py", "--check-readme"], clean)
    if rc:
        raise Refused("hstprogress.py --check-readme found a difference after --write (a figure block is missing?)")
    return clean


REUSE_VERDICT = "hstreuse check: COMPLIANT"


def reuse_check(tree: Path, what: str) -> None:
    """Steps 2 and 4: `tools/hstreuse.py check` of `tree` (its own copy, which checks the files git tracks there) must
    pass. Fails closed: a PROBLEM line, a non-zero exit or a missing COMPLIANT verdict refuses."""
    rc, out = run([sys.executable, "tools/hstreuse.py", "check"], tree)
    problems = [ln for ln in out.splitlines() if ln.startswith("PROBLEM:")]
    verdict = any(ln.strip() == REUSE_VERDICT for ln in out.splitlines())
    if rc or problems or not verdict:
        detail = problems or [f"exit {rc}, no '{REUSE_VERDICT}' line; last output:"] + out.splitlines()[-10:]
        raise Refused(f"hstreuse.py check is not COMPLIANT in {what}:\n  " + "\n  ".join(detail)
                      + "\nFix the licence annotations (REUSE.toml; `hstreuse.py newlib --write` for the generated "
                        "newlib block) on full before publishing.")
    print(f"licence compliance: {what} is COMPLIANT")


def never_lower(public: Path, clean: Path) -> None:
    """Step 3."""
    published = {}
    for name in FIGURE_FILES:
        r = subprocess.run(["git", "show", f"HEAD:{name}"], cwd=public, capture_output=True, text=True,
                           encoding="utf-8")
        published[name] = r.stdout if r.returncode == 0 else None
    candidate = {name: (clean / name).read_text(encoding="utf-8") for name in FIGURE_FILES if (clean / name).exists()}
    for name in FIGURE_FILES:
        if published[name] is None:
            print(f"  {name}: never published, no baseline")
    drops = guard(published, candidate)
    if drops:
        raise Refused("a published figure would go down (never lower a published figure):\n  " + "\n  ".join(drops)
                      + "\nCheck the git-ignored inputs (try/build/*report.json, try/port*-report.json) and the "
                        "sources before publishing.")
    print(f"no figure of {', '.join(FIGURE_FILES)} is lower than the published one "
          f"({git(public, 'rev-parse', '--short', 'HEAD').strip()})")


def sync(public: Path, clean: Path, tmp: Path, message: str | None, apply: bool) -> tuple[Path, list[str]]:
    """Step 4 (the sync); the public tree and the src sources it adds or modifies."""
    if apply:
        before = git(public, "rev-parse", "HEAD").strip()
        rc, _ = run([sys.executable, str(clean / "tools" / "hstpublic.py"), "sync", "--worktree", str(public),
                     "-m", message], clean)
        if rc:
            raise Refused(f"hstpublic.py sync failed (exit {rc})")
        changed = git(public, "diff", "--name-only", "--diff-filter=AM", before, "HEAD").splitlines()
        return public, [p for p in changed if SOURCE.match(p)]
    tree = tmp / "public"
    git(public, "worktree", "add", "--detach", str(tree), "HEAD")
    shutil.copyfile(clean / "hst-config.json", tree / "hst-config.json")
    out = tmp / "export"
    rc, _ = run([sys.executable, str(clean / "tools" / "hstpublish.py"), "--out", str(out)], clean)
    if rc:
        raise Refused(f"hstpublish.py failed (exit {rc})")
    sys.path.insert(0, str(clean / "tools"))
    import hstpublic
    hstpublic.mirror(out, tree)
    git(tree, "add", "-A")
    if hstpublic.check_tree(tree, None):
        raise Refused("the exported tree contains a non-publishable source")
    status = git(tree, "diff", "--cached", "--name-status", "HEAD").splitlines()
    counts: dict[str, int] = {}
    for line in status:
        counts[line[0]] = counts.get(line[0], 0) + 1
    print(f"dry run: the sync would change {len(status)} file(s) in {public} "
          f"({', '.join(f'{k} {v}' for k, v in sorted(counts.items())) or 'nothing'}); mirrored into {tree}, not committed")
    for line in status:
        if not SOURCE.match(line.split("\t")[-1]):
            print(f"  {line}")
    changed = git(tree, "diff", "--cached", "--name-only", "--diff-filter=AM", "HEAD").splitlines()
    return tree, [p for p in changed if SOURCE.match(p)]


def match(tree: Path, sources: list[str], parallel: int) -> None:
    """Step 4 (the check): every added or modified public source is EXACT."""
    if not sources:
        print("no added or modified src/ source in the public tree")
        return
    print(f"hstmatch.py on {len(sources)} added or modified public source(s)")
    status: dict[str, str] = {}
    failed: list[str] = []
    # chunks keep each command line under Windows' 32,767-character limit (WinError 206 with thousands of paths)
    for k in range(0, len(sources), MATCH_CHUNK):
        part = sources[k:k + MATCH_CHUNK]
        rc, out = run([sys.executable, "tools/hstmatch.py", "--quiet", "--parallel", str(parallel), *part], tree)
        for line in out.splitlines():
            parts = line.replace("\\", "/").split()
            if len(parts) >= 4 and parts[3] in part:
                status[parts[3]] = parts[0]
        m = re.search(r"^EXACT (\d+)/(\d+)", out, re.M)
        if rc or not m or int(m.group(1)) != len(part) or int(m.group(2)) != len(part):
            failed.append(out[-500:])
    bad = [f"{p}: {status[p]}" for p in sources if status.get(p, "EXACT") != "EXACT"]
    if failed or bad:
        raise Refused("not every added or modified public source is EXACT:\n  " + "\n  ".join(bad or failed))
    print(f"all {len(sources)} EXACT")


def commit_figures(repo: Path, clean: Path, base: str, date: str, message: str) -> None:
    """Step 5: the figure files --write refreshed in the clean worktree, committed on full."""
    paths = [ln[3:] for ln in git(clean, "status", "--porcelain", "--untracked-files=no").splitlines() if ln.strip()]
    if not paths:
        print("no figure file changed; nothing to commit on full")
        return
    if subprocess.run(["git", "diff", "--quiet", base, "--", *paths], cwd=repo).returncode:
        raise Refused(f"full's copies of {', '.join(paths)} changed since {base[:12]}; the figures were published but "
                      "not committed on full (commit them from the clean worktree by hand)")
    for rel in paths:
        shutil.copyfile(clean / rel, repo / rel)
    trailers = [ln for ln in message.strip().splitlines() if TRAILER.match(ln)]
    msg = f"Refresh progress figures for {date}\n\nWritten by tools/hstrelease.py in a clean worktree of {base[:12]}."
    if trailers:
        msg += "\n\n" + "\n".join(trailers)
    subprocess.run(["git", "commit", "-q", "-F", "-", "--", *paths], cwd=repo, input=msg, text=True, check=True)
    print(git(repo, "log", "-1", "--oneline").strip())


def push(public: Path) -> None:
    """Step 6."""
    env = dict(os.environ)
    if "GIT_CONFIG_COUNT" not in env and shutil.which("gh"):   # non-interactive credentials through the GitHub CLI
        env.update(GIT_CONFIG_COUNT="1", GIT_CONFIG_KEY_0="credential.helper",
                   GIT_CONFIG_VALUE_0="!gh auth git-credential")
    print("$ git push origin public:main", flush=True)
    if subprocess.run(["git", "push", "origin", "public:main"], cwd=public, env=env).returncode:
        raise Refused("git push failed")
    head = git(public, "rev-parse", "HEAD").strip()
    remote = git(public, "ls-remote", "origin", "main").split()
    if not remote or remote[0] != head:
        raise Refused(f"origin main is {remote[0] if remote else 'missing'}, not the public HEAD {head}")
    print(f"origin main = public HEAD = {head}")
    url = git(public, "remote", "get-url", "origin").strip()
    slug = re.search(r"github\.com[:/](.+?)(?:\.git)?$", url)
    print("Progress report workflow:\n  gh run list "
          + (f"--repo {slug.group(1)} " if slug else "") + f"--workflow {WORKFLOW} --limit 3")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--dry-run", action="store_true", help="the default: nothing is committed, synced or pushed")
    mode.add_argument("--apply", action="store_true", help="commit on full and sync the public worktree; no push")
    mode.add_argument("--push", action="store_true", help="--apply, then push public to origin main")
    ap.add_argument("-m", "--message", help="commit message for full and the public branch (needed with --apply/--push)")
    ap.add_argument("--public", type=Path, help="the public worktree (default: the worktree with `public` checked out)")
    ap.add_argument("--date", default=datetime.date.today().isoformat(), help="date of the figures (default today)")
    ap.add_argument("--parallel", type=int, default=12, help="hstmatch.py concurrency")
    ap.add_argument("--keep", action="store_true", help="keep the temporary worktrees for inspection")
    args = ap.parse_args()
    apply = args.apply or args.push
    if apply and not args.message:
        ap.error("--apply and --push need -m MESSAGE")
    repo = ROOT
    tmp = Path(tempfile.mkdtemp(prefix="hstrelease-"))
    public = None
    lock = repo / "try" / "auto.lock"
    prepared = False        # push preparation ran: it takes try/auto.lock and leaves it to the caller
    print(f"hstrelease: {'PUSH' if args.push else 'APPLY' if apply else 'DRY RUN'} from {repo}, figures dated {args.date}")
    try:
        public = (args.public or public_worktree(repo)).resolve()
        if git(public, "symbolic-ref", "--short", "HEAD").strip() != "public":
            raise Refused(f"{public} does not have the public branch checked out")
        print(f"public worktree {public} at {git(public, 'log', '-1', '--oneline').strip()}")
        step(1, "push preparation and commit on full")
        prepared = apply
        base = push_preparation(repo, args.message, apply)
        print(f"commit to publish: {git(repo, 'log', '-1', '--oneline', base).strip()}")
        step(2, "clean worktree, git-ignored inputs, hstprogress.py --write and --check-readme, hstreuse.py check")
        clean = clean_worktree(repo, base, tmp, args.date)
        reuse_check(clean, "the clean worktree")
        step(3, "never-lower guard against the published figures")
        never_lower(public, clean)
        step(4, "sync the public tree, check its licence compliance and match its new sources")
        tree, sources = sync(public, clean, tmp, args.message, apply)
        reuse_check(tree, "the synced public tree")
        match(tree, sources, args.parallel)
        if apply:
            step(5, "commit the refreshed figures on full")
            commit_figures(repo, clean, base, args.date, args.message)
        if args.push:
            step(6, "push and verify the remote")
            push(public)
        print("\nhstrelease: " + ("pushed" if args.push else "applied; push with --push (or git push origin public:main)"
                                  if apply else "dry run passed; nothing was committed, synced or pushed"))
        return 0
    except Refused as e:
        print(f"\nREFUSED: {e}", file=sys.stderr)
        return 1
    finally:
        for wt_repo, path in ((public, tmp / "public"), (repo, tmp / "full")):
            if path.exists() and wt_repo is not None and not args.keep:
                git(wt_repo, "worktree", "remove", "--force", str(path), check=False)
        if args.keep:
            print(f"kept {tmp}")
        else:
            shutil.rmtree(tmp, ignore_errors=True)
        if prepared and lock.is_dir():
            lock.rmdir()
            print("released try/auto.lock")


if __name__ == "__main__":
    raise SystemExit(main())
