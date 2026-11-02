#!/usr/bin/env python3
"""Mutation suite for the promotion gate (tools/hstpromote.py, tools/hstproof.py).

Every test works on a DISPOSABLE SANDBOX, never on the real tree: a temporary directory with copies of the comparison
code, the symbol maps, include/, the data files, a reduced copy of the campaign ledger (the same modules, segments and
functions) and a few real matched sources, its own target and toolchain locks and its own ledger, seeded by a real
verification run (the configured compilers and the decrypted modules). Each mutation must be REFUSED and must leave the
ledger and the journal byte-identical; the positive controls show that the gate still promotes genuine work, so a gate
that refused everything would fail them.

    python tools/test_hstpromote.py [-v] [-k PATTERN]

Needs hst-config.json (toolchain, campaign); skipped without it. Takes a few minutes (each case compiles).
"""
from __future__ import annotations

import json
import os
import re
import shutil
import stat
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REAL = Path(__file__).resolve().parents[1]
# tools/hstlane.py runs the maintainers' agent lanes and is not published (tools/hstpublish.py PRIVATE_PATHS)
HAVE_LANE = (REAL / "tools" / "hstlane.py").is_file()
sys.path.insert(0, str(REAL / "tools"))
import hstproof as P  # noqa: E402

TOOL_FILES = ("hstmatch.py", "hstgate.py", "hstconfig.py", "hstproof.py", "hstpromote.py", "hstlane.py", "hstlint.py")
MAT4 = "src/eboot/mat4_apply_angle_0012aa1c.c"            # MWCC c99; a named symbol and address-named calls
JTBL = "src/eboot/FUN_0021ab8c_0021ab8c.c"                # MWCC with rodata= (a placed jump table)
GCC = "src/eboot/strtok_r_00015f98.c"                     # psp-gcc profile
RUN = "src/eboot/member_pair_update_002059ec_run.cpp"     # multi-function source (four functions)
INNER = ("src/eboot/wrap_angles_00205a18.cpp", "src/eboot/FUN_00205b4c.c")   # inside RUN: subsumed by the seed
DTOR = "src/eboot/dtor_00115800.cpp"                      # C++ with vtable symbols
TWIN = "src/sharing_main/wrap_angles_0018a3f0.cpp"        # the second module
SEEDED = (MAT4, JTBL, GCC, RUN, *INNER, DTOR, TWIN)
NEW = "src/eboot/FUN_00000008.c"                          # genuine, not seeded: promoted by the controls
NEW_BIG = "src/eboot/obj_report_001550c0.cpp"             # genuine, 132 bytes, not seeded
ASM_ADDR = 0x10                                           # an EBOOT function the asm mutation reproduces with .word
HELPED = "src/eboot/power_callback_00291434.cpp"          # helpers=: a static stub at 0x28fb44 placed apart from the target
HELPER_OWN = "src/eboot/FUN_0028fb44.cpp"                 # the same stub as its own one-function source

# RUN owns the INNER functions' bytes, so the real tree keeps no separate sources for them (ff11391): the test
# copies them from tools/testdata/hstpromote/ to their src/ paths.
FIXTURES = REAL / "tools" / "testdata" / "hstpromote"


def real_source(rel: str) -> Path:
    """The source file a sandbox path is copied from: the real tree, or the fixture copy for an INNER source."""
    return FIXTURES / Path(rel).name if rel in INNER else REAL / rel


def _rmtree(path: Path) -> None:
    def onexc(func, p, _exc):
        os.chmod(p, stat.S_IWRITE)
        func(p)
    shutil.rmtree(path, onexc=onexc)


def have_toolchain() -> bool:
    try:
        import hstmatch as M
        M.campaign_root()
        M.native_dir()
        return (REAL / "hst-config.json").exists()
    except BaseException:  # noqa: BLE001 - hstconfig raises SystemExit when unset
        return False


class Sandbox:
    def __init__(self, root: Path):
        self.root = root

    def run(self, tool: str, *args: str, env: dict | None = None) -> subprocess.CompletedProcess:
        e = dict(os.environ)
        e.update(env or {})
        return subprocess.run([sys.executable, str(self.root / "tools" / tool), *args], cwd=self.root,
                              capture_output=True, text=True, env=e)

    def gate(self, *args: str, env: dict | None = None) -> subprocess.CompletedProcess:
        return self.run("hstpromote.py", *args, env=env)

    def promote(self, *args: str, env: dict | None = None) -> subprocess.CompletedProcess:
        return self.run("hstpromote.py", "promote", *args, env=env)

    def check(self, *args: str, env: dict | None = None) -> subprocess.CompletedProcess:
        return self.run("hstproof.py", "check", *args, env=env)

    def state(self) -> tuple[bytes, bytes]:
        return (self.root / "data" / "accepted-ledger.jsonl").read_bytes(), \
            (self.root / "data" / "accepted-journal.jsonl").read_bytes()

    def text(self, rel: str) -> str:
        return (self.root / rel).read_text(encoding="utf-8")

    def write(self, rel: str, text: str) -> None:
        p = self.root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_bytes(text.encode("utf-8"))

    def edit(self, rel: str, old: str, new: str, count: int = 1) -> None:
        t = self.text(rel)
        assert old in t, f"{old!r} not in {rel}"
        self.write(rel, t.replace(old, new, count))

    def git(self, *args: str) -> str:
        return subprocess.run(["git", "-c", "user.name=test", "-c", "user.email=test@invalid", *args], cwd=self.root,
                              capture_output=True, text=True, check=True).stdout

    def config(self, **changes: str) -> None:
        cfg = json.loads((self.root / "hst-config.json").read_text(encoding="utf-8"))
        cfg.update(changes)
        (self.root / "hst-config.json").write_text(json.dumps(cfg, indent=1), encoding="utf-8")

    def proofs(self) -> list[dict]:
        return [json.loads(x) for x in (self.root / "data" / "accepted-ledger.jsonl").read_text().splitlines()[1:]]

    def py(self, code: str) -> subprocess.CompletedProcess:
        """Run Python with the sandbox's tools first on the path (for checks below the CLI)."""
        return subprocess.run([sys.executable, "-c", f"import sys; sys.path.insert(0, {str(self.root / 'tools')!r})\n" + code],
                              cwd=self.root, capture_output=True, text=True)


def minimal_campaign(dest: Path) -> None:
    """The campaign files the tools read, reduced to the fields they use (the same modules, segments, functions)."""
    import hstmatch as M
    src = M.campaign_root()
    led = json.loads((src / "ledger" / "campaign.json").read_text(encoding="utf-8-sig"))
    small = {"modules": [{"name": m["name"], "input": m["input"], "input_sha256": m.get("input_sha256"),
                          "load_segments": m["load_segments"],
                          "functions": [{"start": f["start"], "end": f["end"], "boundary_status": f.get("boundary_status")}
                                        for f in m["functions"]]} for m in led["modules"]]}
    (dest / "ledger").mkdir(parents=True)
    (dest / "inputs").mkdir()
    (dest / "ledger" / "campaign.json").write_text(json.dumps(small), encoding="utf-8")
    shutil.copyfile(src / "inputs" / "revision-selection.json", dest / "inputs" / "revision-selection.json")


BASE: Path | None = None
WORK: Path | None = None


def setUpModule() -> None:
    global BASE, WORK
    if not have_toolchain():
        raise unittest.SkipTest("no configured toolchain/campaign (hst-config.json)")
    WORK = Path(tempfile.mkdtemp(prefix="hstpromote-test-"))
    BASE = WORK / "base"
    (BASE / "tools").mkdir(parents=True)
    for f in TOOL_FILES:
        if f != "hstlane.py" or HAVE_LANE:
            shutil.copy2(REAL / "tools" / f, BASE / "tools" / f)
    shutil.copytree(REAL / "symbols", BASE / "symbols")
    shutil.copytree(REAL / "include", BASE / "include")
    (BASE / "data").mkdir()
    for f in ["modules.json", *(p.name for p in (REAL / "data").glob("functions-*.txt"))]:
        shutil.copyfile(REAL / "data" / f, BASE / "data" / f)
    for rel in SEEDED:
        (BASE / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(real_source(rel), BASE / rel)
    minimal_campaign(BASE / "campaign")
    cfg = json.loads((REAL / "hst-config.json").read_text(encoding="utf-8"))
    cfg["campaign_root"] = str(BASE / "campaign")
    (BASE / "hst-config.json").write_text(json.dumps(cfg, indent=1), encoding="utf-8")
    (BASE / "try").mkdir()
    (BASE / "docs").mkdir()
    (BASE / "docs" / "MWCC-NOTES.md").write_text("# notes\n", encoding="utf-8")
    (BASE / ".gitignore").write_text("/build/\n/try/\n/campaign/\n/hst-config.json\n__pycache__/\n", encoding="utf-8")
    (BASE / ".gitattributes").write_text("* -text\n", encoding="utf-8")
    sb = Sandbox(BASE)
    r = sb.gate("lock", "--reason", "mutation-suite sandbox")
    assert r.returncode == 0, r.stdout + r.stderr
    r = sb.gate("seed")
    assert r.returncode == 0, r.stdout + r.stderr
    sb.git("init", "-q")
    sb.git("add", "-A")
    sb.git("commit", "-qm", "seeded sandbox")


def tearDownModule() -> None:
    if WORK is not None and WORK.exists() and not os.environ.get("HSTPROMOTE_TEST_KEEP"):
        _rmtree(WORK)


class GateCase(unittest.TestCase):
    def setUp(self) -> None:
        self.dir = WORK / self.id().split(".")[-1]
        shutil.copytree(BASE, self.dir)
        self.sb = Sandbox(self.dir)
        self.sb.config(campaign_root=str(self.dir / "campaign"))   # this copy's own campaign files
        self.before = self.sb.state()

    def tearDown(self) -> None:
        if not os.environ.get("HSTPROMOTE_TEST_KEEP"):
            _rmtree(self.dir)

    # -- assertions
    def observed(self, r: subprocess.CompletedProcess, what: str) -> None:
        """HSTPROMOTE_TEST_LOG=<file>: one JSON line per observed refusal (the report's mutation table)."""
        log = os.environ.get("HSTPROMOTE_TEST_LOG")
        if log:
            lines = [ln for ln in (r.stdout + r.stderr).splitlines()
                     if ln.startswith(("REFUSED", "  LOST", "  not promoted", "PROBLEM", "  DIFFERENT"))]
            with open(log, "a", encoding="utf-8") as fh:
                fh.write(json.dumps({"test": self.id().split(".")[-1], "check": what, "exit": r.returncode,
                                     "lines": lines[:4]}) + "\n")

    def assertRefused(self, r: subprocess.CompletedProcess, *needles: str) -> None:
        out = r.stdout + r.stderr
        self.observed(r, "refused")
        self.assertEqual(r.returncode, 1, out)
        self.assertIn("REFUSED", out)
        for n in needles:
            self.assertRegex(out, n)
        self.assertEqual(self.sb.state(), self.before, "a refused promotion changed the ledger or journal")

    def assertCandidateRefused(self, r: subprocess.CompletedProcess, rel: str, needle: str) -> None:
        out = r.stdout + r.stderr
        self.observed(r, "candidate refused")
        self.assertEqual(r.returncode, 3, out)
        self.assertRegex(out, rf"not promoted: {re.escape(rel)}: .*{needle}")
        self.assertNotIn(rel, {p["source"] for p in self.sb.proofs()})

    def assertCheckFails(self, *needles: str, args: tuple = ()) -> None:
        r = self.sb.check(*args)
        out = r.stdout + r.stderr
        self.observed(r, "check " + " ".join(args))
        self.assertEqual(r.returncode, 1, out)
        for n in needles:
            self.assertRegex(out, n)


class Controls(GateCase):
    """The gate still accepts genuine work (a gate that refuses everything fails here)."""

    def test_seeded_sandbox_checks_clean(self) -> None:
        r = self.sb.check("--base", "HEAD")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertEqual(len(self.sb.proofs()), len(SEEDED) - len(INNER))
        seed = json.loads(self.before[1].splitlines()[0])
        self.assertEqual(sorted(seed["subsumed"]), sorted(INNER))

    def test_nothing_to_do(self) -> None:
        r = self.sb.promote(TWIN)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("unchanged", r.stdout)
        self.assertEqual(self.sb.state(), self.before)

    def test_new_genuine_source_is_promoted(self) -> None:
        shutil.copyfile(REAL / NEW, self.dir / NEW)
        r = self.sb.promote(NEW, "--promoted-out", "try/promoted.txt")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn(NEW, {p["source"] for p in self.sb.proofs()})
        self.assertEqual((self.dir / "try" / "promoted.txt").read_text().split(), [NEW])
        self.assertEqual(self.sb.check("--base", "HEAD").returncode, 0)
        cov = self.sb.run("hstproof.py", "coverage", "--json", "try/cov.json")
        self.assertEqual(cov.returncode, 0, cov.stdout + cov.stderr)
        doc = json.loads((self.dir / "try" / "cov.json").read_text())
        self.assertEqual(doc["modules"]["EBOOT.BIN"]["accepted_range_bytes"],
                         sum(int(p["end"], 16) - int(p["start"], 16) for p in self.sb.proofs() if p["module"] == "EBOOT.BIN"))

    def test_rename_keeps_every_byte(self) -> None:
        self.sb.git("mv", DTOR, "src/eboot/dtor_renamed_00115800.cpp")
        r = self.sb.promote("src/eboot/dtor_renamed_00115800.cpp")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        sources = {p["source"] for p in self.sb.proofs()}
        self.assertNotIn(DTOR, sources)
        self.assertIn("src/eboot/dtor_renamed_00115800.cpp", sources)
        self.assertEqual(self.sb.check().returncode, 0)

    def test_unused_symbol_append_reverifies_nothing(self) -> None:
        with (self.dir / "symbols" / "EBOOT.BIN.txt").open("a", encoding="utf-8", newline="\n") as fh:
            fh.write("unused_name_for_the_test = 0x00001234;\n")
        r = self.sb.promote(TWIN)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("0 to reverify", r.stdout)

    def test_explicit_retraction_is_recorded_not_silent(self) -> None:
        r = self.sb.gate("retract", "EBOOT.BIN", "0x0012aa1c", "--reason", "test: the source is wrong")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        last = json.loads(self.sb.state()[1].splitlines()[-1])
        self.assertEqual((last["event"], last["reason"]), ("retract", "test: the source is wrong"))
        self.assertEqual(self.sb.check("--base", "HEAD").returncode, 0)   # a recorded retraction is allowed
        self.assertNotEqual(self.sb.gate("retract", "EBOOT.BIN", "0x0021ab8c", "--reason", " ").returncode, 0)


class Wiring(GateCase):
    """The calls the push procedure (tools/hstpushprep.sh) and the lane harvest (tools/hstlane.py) make."""

    def test_empty_paths_file_reverifies_only_stale_proofs(self) -> None:
        (self.dir / "try" / "changed.txt").write_text("")
        r = self.sb.promote("--paths-from", "try/changed.txt", "--promoted-out", "try/verified.txt")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("0 new candidate(s)", r.stdout)
        self.assertEqual((self.dir / "try" / "verified.txt").read_text(), "")

    def test_staged_check_follows_the_index(self) -> None:
        shutil.copyfile(REAL / NEW, self.dir / NEW)
        self.assertEqual(self.sb.promote(NEW).returncode, 0)
        r = self.sb.check("--base", "HEAD", "--staged")      # nothing staged yet
        self.assertEqual(r.returncode, 1, r.stdout)
        self.assertIn(f"{NEW}: proven but not in the index", r.stdout)
        self.sb.git("add", NEW, "data/accepted-ledger.jsonl", "data/accepted-journal.jsonl")
        r = self.sb.check("--base", "HEAD", "--staged")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        v = self.sb.gate("verify", "--since", "HEAD")        # the newly proven source is re-run and agrees
        self.assertEqual(v.returncode, 0, v.stdout + v.stderr)
        self.assertIn("1 proof(s) checked, 0 different", v.stdout)
        with (self.dir / NEW).open("a", encoding="utf-8") as fh:
            fh.write("/* edited after staging */\n")
        self.assertCheckFails("source changed", args=("--staged",))

    @unittest.skipUnless(HAVE_LANE, "tools/hstlane.py is maintainer-only")
    def test_harvest_promotes_and_removes_what_the_gate_refuses(self) -> None:
        lane = self.dir / "ext" / "lane1"
        for d in ("src/eboot", "symbols", "wip", "docs"):
            (lane / d).mkdir(parents=True)
        (lane / "docs" / "MWCC-NOTES.md").write_text("# notes\n", encoding="utf-8")
        shutil.copyfile(REAL / NEW, lane / NEW)
        (self.dir / INNER[0]).unlink()                        # unproven (subsumed): the lane offers it again
        shutil.copyfile(real_source(INNER[0]), lane / INNER[0])
        self.sb.config(ext_root=str(self.dir / "ext"))
        r = self.sb.run("hstlane.py", "harvest", "lane1")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn(f"REFUSED by the promotion gate (removed from src/): {INNER[0]}: overlap", r.stdout)
        self.assertFalse((self.dir / INNER[0]).exists())
        self.assertIn(NEW, {p["source"] for p in self.sb.proofs()})
        self.assertIn('"promoted": 1', r.stdout)


    def lane_wip(self, lane: Path, name: str, main_text: str, lane_text: str) -> None:
        self.sb.write(f"wip/{name}", main_text)
        (lane / "wip" / name).write_text(lane_text, encoding="utf-8")

    @unittest.skipUnless(HAVE_LANE, "tools/hstlane.py is maintainer-only")
    def test_harvest_replaces_a_wip_file_only_with_fewer_differing_words(self) -> None:
        lane = self.dir / "ext" / "lane2"
        for d in ("src/eboot", "symbols", "wip", "docs"):
            (lane / d).mkdir(parents=True)
        (lane / "docs" / "MWCC-NOTES.md").write_text("# notes\n", encoding="utf-8")
        new, big = (REAL / NEW).read_text(encoding="utf-8"), (REAL / NEW_BIG).read_text(encoding="utf-8")
        worse = new.replace("{\n}", "{\n  *(volatile int *)0x40 = 1;\n  *(volatile int *)0x44 = 2;\n}")
        better = new.replace("{\n}", "{\n  *(volatile int *)0x40 = 1;\n}")
        self.lane_wip(lane, "near_00000008.c", worse, better)                     # the lane improved it: copied
        self.lane_wip(lane, "near_001550c0.cpp", big, big.replace("return 0;", "return 1;"))   # the lane made it worse
        self.sb.config(ext_root=str(self.dir / "ext"))
        r = self.sb.run("hstlane.py", "harvest", "lane2")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn('"wip_improved": 1', r.stdout)
        got = self.sb.text("wip/near_00000008.c")
        self.assertIn("0x40 = 1;", got)
        self.assertNotIn("0x44", got)
        self.assertRegex(got, r"// improved by lane lane2 \(unreviewed\): \d+ -> \d+ differing words")
        self.assertEqual(self.sb.text("wip/near_001550c0.cpp"), big)


class HelperPlacement(GateCase):
    """Same-file helpers (helpers=): every function at its own retail address; a helper that differs refuses the file."""

    def copy(self, *rels: str) -> None:
        for rel in rels:
            shutil.copyfile(REAL / rel, self.dir / rel)

    def test_helper_source_is_promoted_and_each_byte_owned_once(self) -> None:
        self.copy(HELPED, HELPER_OWN)
        m = self.sb.run("hstmatch.py", HELPED)
        self.assertIn("EXACT 1/1", m.stdout, m.stdout + m.stderr)
        r = self.sb.promote(HELPED, HELPER_OWN)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        proofs = {p["source"]: p for p in self.sb.proofs()}
        p = proofs[HELPED]
        self.assertEqual((p["start"], p["end"]), ("0x00291434", "0x002914c0"))     # claims the target only
        self.assertEqual((p["compared"][".text:FUN_0028fb44"]["addr"], p["compared"][".text:FUN_0028fb44"]["size"]),
                         ("0x0028fb44", 44))                                         # ... but compared the helper
        self.assertEqual((proofs[HELPER_OWN]["start"], proofs[HELPER_OWN]["end"]), ("0x0028fb44", "0x0028fb70"))
        self.assertEqual(self.sb.check().returncode, 0)
        v = self.sb.gate("verify")                                                   # re-run: the same proof
        self.assertEqual(v.returncode, 0, v.stdout + v.stderr)

    def test_helper_source_matches_next_to_a_gcc_source(self) -> None:
        """The helpers= check reads each source's own profile: a psp-gcc source compiled in the same call (here the
        last one, so the compile loop ends on it) must not make the mwccpsp helpers= source UNPLACED_FUNCTION."""
        self.copy(HELPED)
        m = self.sb.run("hstmatch.py", HELPED, GCC)
        self.assertIn("EXACT 2/2", m.stdout, m.stdout + m.stderr)

    def test_helper_mismatch_is_refused(self) -> None:
        self.copy(HELPED)
        self.sb.edit(HELPED, "static void FUN_0028fb44(...)", "static void FUN_0028fb44(const char *s, ...)")
        m = self.sb.run("hstmatch.py", HELPED)
        self.assertRegex(m.stdout, r"MISMATCH .* 0 differ; helper FUN_0028fb44@0x0028fb44: .* differ")
        self.assertCandidateRefused(self.sb.promote(HELPED), HELPED, "helper FUN_0028fb44")
        self.assertEqual(self.sb.state(), self.before)

    def test_unlisted_helper_is_refused(self) -> None:
        self.copy(HELPED)
        self.sb.edit(HELPED, " helpers=FUN_0028fb44@0x0028fb44-0x0028fb70", "")
        m = self.sb.run("hstmatch.py", HELPED)
        self.assertIn("MISPLACED", m.stdout)
        self.assertIn("also defines FUN_0028fb44", m.stdout)
        self.assertCandidateRefused(self.sb.promote(HELPED), HELPED, "MISPLACED")

    def test_helper_off_the_inventory_is_refused(self) -> None:
        self.copy(HELPED)
        self.sb.edit(HELPED, "-0x0028fb70", "-0x0028fb6c")
        self.assertCandidateRefused(self.sb.promote(HELPED), HELPED, "helper FUN_0028fb44 .* not a function of the "
                                                                     "locked inventory")

    def test_lying_matcher_cannot_pass_a_mismatching_helper(self) -> None:
        self.sb.edit("tools/hstmatch.py", '        summary.append({"source": rel,',
                     '        status, detail = "EXACT", ""\n        summary.append({"source": rel,')
        r = self.sb.gate("migrate-toolchain", "--reason", "test: a matcher that always says EXACT")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.before = self.sb.state()
        self.copy(HELPED)
        self.sb.edit(HELPED, "static void FUN_0028fb44(...)", "static void FUN_0028fb44(const char *s, ...)")
        r = self.sb.promote(HELPED)
        self.assertEqual(r.returncode, 3, r.stdout + r.stderr)
        self.assertRegex(r.stdout, rf"not promoted: {HELPED}: .*independent comparison \(helper FUN_0028fb44\)")
        self.assertEqual(self.sb.state(), self.before)


class Migrations(GateCase):
    def test_gate_only_change_reruns_the_gate_without_recompiling(self) -> None:
        with (self.dir / "tools" / "hstgate.py").open("a", encoding="utf-8") as fh:
            fh.write("# a comment: still a change of the comparison code\n")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", r"tools/hstgate\.py changed")
        r = self.sb.gate("migrate-toolchain", "--reason", "test: gate comment")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("migrated (gate): 0 proofs reverified", r.stdout)
        self.assertEqual(self.sb.check().returncode, 0)
        hist = json.loads((self.dir / "data" / "toolchain-manifest.json").read_text())["history"]
        self.assertEqual((hist[-1]["event"], hist[-1]["reason"]), ("migrate-toolchain", "test: gate comment"))

    def test_header_change_reverifies_its_dependents_only(self) -> None:
        (self.dir / "include" / "hstpromote_test.h").write_text("#define HSTPROMOTE_TEST 1\n")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", "include/hstpromote_test.h added")
        r = self.sb.gate("migrate-toolchain", "--reason", "test: a new header")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("migrated (include): 0 proofs reverified", r.stdout)
        self.assertEqual(self.sb.check().returncode, 0)

    def test_compiler_flag_change_reverifies_everything_and_refuses(self) -> None:
        self.sb.edit("tools/hstmatch.py", "'-c -O4,p -sdatathreshold 0'", "'-c -O0 -sdatathreshold 0'")
        r = self.sb.gate("migrate-toolchain", "--reason", "test: -O0")
        self.assertRefused(r, "do not hold under the new toolchain")
        self.assertIn("migration scope: full", r.stdout)


class SourceMutations(GateCase):
    def test_alter_one_instruction_producing_expression(self) -> None:
        self.sb.edit(MAT4, "s->angle);", "s->trans[1]);")
        r = self.sb.promote(TWIN)          # whatever is named, a changed accepted source is reverified
        self.assertRefused(r, r"LOST EBOOT\.BIN \[0x0012aa1c, 0x0012aa6c\)", "mismatch")
        self.assertCheckFails("source changed")

    def test_delete_a_matched_source(self) -> None:
        (self.dir / DTOR).unlink()
        self.assertRefused(self.sb.promote(), r"LOST EBOOT\.BIN \[0x00115800, 0x0011587c\)", "source missing")
        self.assertCheckFails("source missing")

    def test_shorten_a_header_range(self) -> None:
        self.sb.edit(DTOR, "end=0x0011587c", "end=0x00115878")
        self.assertRefused(self.sb.promote(DTOR), r"LOST EBOOT\.BIN \[0x00115800, 0x0011587c\)")

    def test_shorten_a_multi_function_run_even_with_a_higher_total(self) -> None:
        # the shortened run still compares EXACT and ends on a function boundary, and a new genuine source adds more
        # bytes than the run loses: the total rises, but the lost range is not covered, so the gate refuses
        self.sb.edit(RUN, "end=0x00205bc0", "end=0x00205b54")
        shutil.copyfile(REAL / NEW_BIG, self.dir / NEW_BIG)
        r = self.sb.promote(RUN, NEW_BIG)
        self.assertRefused(r, r"LOST EBOOT\.BIN \[0x00205b54, 0x00205bc0\) 108 bytes")

    def test_claim_a_partial_function(self) -> None:
        self.sb.edit(TWIN, "end=0x0018a524", "end=0x0018a400")
        self.assertRefused(self.sb.promote(TWIN), "partial-function claim")

    def test_add_an_extra_flag_to_an_accepted_source(self) -> None:
        self.sb.edit(MAT4, "lang=c99 */", 'lang=c99 extra="-O4,p" */')
        self.assertRefused(self.sb.promote(), "extra-flags")

    def test_extra_flag_on_a_new_source_never_counts(self) -> None:
        shutil.copyfile(REAL / NEW, self.dir / NEW)
        self.sb.edit(NEW, " */", ' extra="-O4,p" */')
        self.assertCandidateRefused(self.sb.promote(NEW), NEW, "extra-flags")
        self.assertEqual(self.sb.state(), self.before)

    def test_count_a_clone_twice(self) -> None:
        shutil.copyfile(self.dir / TWIN, self.dir / "src/sharing_main/wrap_angles_again_0018a3f0.cpp")
        self.assertCandidateRefused(self.sb.promote("src/sharing_main/wrap_angles_again_0018a3f0.cpp"),
                                    "src/sharing_main/wrap_angles_again_0018a3f0.cpp", "overlap")
        self.assertEqual(self.sb.state(), self.before)

    def test_a_contained_source_cannot_double_count_a_run(self) -> None:
        self.assertCandidateRefused(self.sb.promote(INNER[0]), INNER[0], "overlap")

    def test_instruction_emitting_asm_that_really_matches(self) -> None:
        img = self.sb.py("import hstproof as P, json\nt = P.load_target_manifest()\n"
                         f"print(P.image_range('EBOOT.BIN', {ASM_ADDR}, {ASM_ADDR + 8}, t, True).hex())")
        self.assertEqual(img.returncode, 0, img.stderr)
        b = bytes.fromhex(img.stdout.strip())
        w = [int.from_bytes(b[i:i + 4], "little") for i in (0, 4)]
        rel = f"src/eboot/FUN_{ASM_ADDR:08x}.c"
        self.sb.write(rel, f"/* HSTMATCH module=EBOOT.BIN start=0x{ASM_ADDR:08x} end=0x{ASM_ADDR + 8:08x} "
                           f"symbol=FUN_{ASM_ADDR:08x} lang=c99 cc=gcc171-O2 */\n"
                           "/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */\n"
                           f'__asm__(".text\\n.globl FUN_{ASM_ADDR:08x}\\nFUN_{ASM_ADDR:08x}:\\n.word 0x{w[0]:08x}\\n.word 0x{w[1]:08x}\\n");\n')
        m = self.sb.run("hstmatch.py", rel)            # the bytes do match: only the gate stops it
        self.assertIn("EXACT_FAILS_GATE", m.stdout, m.stdout + m.stderr)
        self.assertCandidateRefused(self.sb.promote(rel), rel, "gate")
        self.assertEqual(self.sb.state(), self.before)


class SymbolMutations(GateCase):
    def test_change_a_referenced_symbol_address(self) -> None:
        self.sb.edit("symbols/EBOOT.BIN.txt", "mat4_copy_004d270 = 0x0004d270;", "mat4_copy_004d270 = 0x0004d274;")
        r = self.sb.promote(TWIN)
        self.assertRefused(r, r"LOST EBOOT\.BIN \[0x0012aa1c, 0x0012aa6c\)", "symbol resolution changed")
        self.assertCheckFails("symbol resolution changed: mat4_copy_004d270")

    def test_override_an_address_name(self) -> None:
        with (self.dir / "symbols" / "EBOOT.BIN.txt").open("a", encoding="utf-8", newline="\n") as fh:
            fh.write("FUN_00093434 = 0x00093438;\n")
        self.assertRefused(self.sb.promote(TWIN), r"LOST EBOOT\.BIN \[0x0012aa1c, 0x0012aa6c\)")


class ToolchainMutations(GateCase):
    def wsl(self, script: str) -> str:
        return subprocess.run(["wsl.exe", "-e", "sh", "-c", script], capture_output=True, text=True, check=True).stdout

    def test_replace_the_compiler_binary(self) -> None:
        original = json.loads((self.dir / "hst-config.json").read_text())["mwcc_native"]
        d = self.wsl("d=$(mktemp -d /tmp/hstmut.XXXXXX); mkdir -p $d/builds/219; cp $HOME/hst-mwcc/wibo $d/; "
                     "cp $HOME/hst-mwcc/builds/210/mwccpsp.exe $HOME/hst-mwcc/builds/210/LMGR8C.DLL $d/builds/219/; echo $d").strip()
        try:
            self.sb.config(mwcc_native=d)
            self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", r"mwcc: .*builds/219/mwccpsp\.exe")
            self.sb.config(mwcc_native=original)
            r = self.sb.promote(TWIN, env={"HST_MWCC_NATIVE": d})       # the same through the environment
            self.assertRefused(r, "toolchain-drift")
        finally:
            self.wsl(f"rm -rf '{d}'")

    def test_edit_the_compiler_hash_in_the_lock(self) -> None:
        path = self.dir / "data" / "toolchain-manifest.json"
        doc = json.loads(path.read_text())
        doc["core"]["programs"]["mwcc"]["files"]["builds/219/mwccpsp.exe"] = "0" * 64
        path.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-manifest", "edited")
        doc["core_digest"] = P.digest(doc["core"])      # a forger who also recomputes the digest
        path.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift")

    def test_add_a_flag_to_the_compiler_profile(self) -> None:
        self.sb.edit("tools/hstmatch.py", "'-c -O4,p -sdatathreshold 0'", "'-c -O4,p -sdatathreshold 0 -inline off'")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", r"tools/hstmatch\.py changed")

    def test_relax_the_fake_match_gate(self) -> None:
        self.sb.edit("tools/hstgate.py", "    return [f for f in findings if f.severity == REFUSE]", "    return []")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", r"tools/hstgate\.py changed")

    def test_matcher_that_always_returns_exact(self) -> None:
        self.sb.edit("tools/hstmatch.py", '        summary.append({"source": rel,',
                     '        status, detail = "EXACT", ""\n        summary.append({"source": rel,')
        # 1. the lock refuses the changed comparison code
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift", r"tools/hstmatch\.py changed")
        # 2. even if the lying matcher is migrated in (every accepted proof is genuine, so the migration passes) ...
        r = self.sb.gate("migrate-toolchain", "--reason", "test: an attacker re-locks a lying matcher")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.before = self.sb.state()
        # ... a changed source that no longer matches is caught by the promoter's own byte comparison
        self.sb.edit(MAT4, "s->angle);", "s->trans[1]);")
        self.assertRefused(self.sb.promote(TWIN), "independent comparison")
        # ... and a new non-matching source is refused, not promoted
        self.sb.edit(MAT4, "s->trans[1]);", "s->angle);")
        shutil.copyfile(REAL / NEW, self.dir / NEW)
        self.sb.edit(NEW, "{\n}", "{\n  *(volatile int *)0x40 = 1;\n}")
        r = self.sb.promote(NEW)
        self.assertEqual(r.returncode, 3, r.stdout + r.stderr)
        self.assertRegex(r.stdout, f"not promoted: {NEW}: .*(independent comparison|longer than the claimed)")

    def test_matcher_that_substitutes_the_object(self) -> None:
        # a matcher that touches the object after compiling: its own verdict would still be EXACT, the promoter's own
        # compile of the recorded profile is not byte-identical, so not even the migration of that matcher passes
        self.sb.edit("tools/hstmatch.py", "        obj = obj_path.read_bytes()\n",
                     "        obj_path.write_bytes(obj_path.read_bytes() + b'\\0')\n        obj = obj_path.read_bytes()\n")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift")
        r = self.sb.gate("migrate-toolchain", "--reason", "test: a matcher that alters objects")
        self.assertRefused(r, "do not hold under the new toolchain")
        self.assertIn("differs from the one the recorded profile produces", r.stdout)

    def test_stale_build_artifact_after_a_source_edit(self) -> None:
        # good artifacts in the matcher's usual build directory, then the source changes
        m = self.sb.run("hstmatch.py", MAT4)
        self.assertIn("EXACT 1/1", m.stdout, m.stdout + m.stderr)
        self.sb.edit(MAT4, "s->angle);", "s->trans[1]);")
        # (a) the promoter never reads them: every run has a fresh directory
        self.assertRefused(self.sb.promote(TWIN), r"LOST EBOOT\.BIN \[0x0012aa1c", "mismatch")
        # (b) a matcher changed to reuse an existing object is a toolchain change
        self.sb.edit("tools/hstmatch.py", "f.unlink()", "pass  # reuse what is there")
        self.assertRefused(self.sb.promote(TWIN), "toolchain-drift")

    def test_stale_artifact_is_refused_below_the_cli(self) -> None:
        code = f"""
import os, shutil, time, json
import hstpromote as R, hstproof as P
ctx = R.Context()
class A: parallel = 2; keep_run = False
first = R.new_run(ctx, A)
tree = P.tree_sources()
c = R.candidates_for([{TWIN!r}], tree)
proofs, refused = first.verify(c)
assert proofs, refused
old = first.build_dir(c[0])
time.sleep(3)
second = R.new_run(ctx, A)
c2 = R.candidates_for([{TWIN!r}], tree)
second.precheck(c2[0])
dest = second.build_dir(c2[0])
shutil.copytree(old, dest)                       # yesterday's artifacts, planted where this run would look
for f in dest.iterdir():
    os.utime(f, (time.time() - 3600, time.time() - 3600))
own = second.run_own_compile(c2)
row = {{"source": {TWIN!r}, "module": "hstp_trans_main.prx", "start": "0x0018a3f0", "status": "EXACT"}}
try:
    second.make_proof(c2[0], row, own[{TWIN!r}], {{}})
    print("ACCEPTED")
except P.ProofError as e:
    print("REFUSED", e.code)
first.finish(True); second.finish(True)
"""
        r = self.sb.py(code)
        self.assertIn("REFUSED stale-artifact", r.stdout, r.stdout + r.stderr)


class TargetMutations(GateCase):
    def test_change_the_function_inventory(self) -> None:
        path = self.dir / "data" / "functions-EBOOT.BIN.txt"
        lines = path.read_text().splitlines(keepends=True)
        path.write_text("".join(lines[:5] + lines[6:]))
        self.assertRefused(self.sb.promote(TWIN), "target-drift", "functions-EBOOT.BIN.txt differs")
        self.assertCheckFails("target-drift")

    def test_change_the_denominator_in_the_lock(self) -> None:
        path = self.dir / "data" / "target-manifest.json"
        doc = json.loads(path.read_text())
        doc["scope_bytes"] -= 100000
        path.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        self.assertRefused(self.sb.promote(TWIN), "target-manifest", "edited")
        doc["target_digest"] = P.digest(P.target_identity(doc))
        path.write_text(json.dumps(doc, indent=1, sort_keys=True) + "\n")
        self.assertRefused(self.sb.promote(TWIN), "target-drift", "scope_bytes")

    def test_change_the_campaign_function_list(self) -> None:
        path = self.dir / "campaign" / "ledger" / "campaign.json"
        led = json.loads(path.read_text())
        mod = next(m for m in led["modules"] if m["name"] == "libfont.prx")
        mod["functions"] = mod["functions"][1:]
        path.write_text(json.dumps(led))
        self.assertRefused(self.sb.promote(TWIN), "target-drift", "libfont.prx inventory")

    def test_replace_the_retail_module(self) -> None:
        sel_path = self.dir / "campaign" / "inputs" / "revision-selection.json"
        sel = json.loads(sel_path.read_text(encoding="utf-8-sig"))
        fake = self.dir / "fake-root"
        shutil.copytree(Path(sel["roots"]["build"]) / "inputs", fake / "inputs",
                        ignore=shutil.ignore_patterns("selected.psp"))
        elf = fake / "inputs" / "selected.elf"
        data = bytearray(elf.read_bytes())
        data[0x54 + 0x0012aa20] ^= 0xFF          # one byte inside a matched function
        elf.write_bytes(bytes(data))
        sel["roots"]["build"] = str(fake)
        sel_path.write_text(json.dumps(sel), encoding="utf-8")
        self.assertRefused(self.sb.promote(TWIN), "target-drift", "EBOOT.BIN")

    def test_retarget_refuses_a_lock_change_that_breaks_a_proof(self) -> None:
        # merge the run's two middle functions in the inventory; a proof ending inside the merged one no longer aligns
        cpath = self.dir / "campaign" / "ledger" / "campaign.json"
        led = json.loads(cpath.read_text())
        mod = next(m for m in led["modules"] if m["name"] == "hstp_trans_main.prx")
        fns = sorted(mod["functions"], key=lambda f: f["start"])
        i = next(k for k, f in enumerate(fns) if f["start"] == 0x0018a3f0)
        fns[i]["end"] = fns[i + 1]["end"]
        del fns[i + 1]
        mod["functions"] = fns
        cpath.write_text(json.dumps(led))
        fpath = self.dir / "data" / "functions-hstp_trans_main.prx.txt"
        head = fpath.read_text().splitlines(keepends=True)[0]
        fpath.write_text(head + "".join(f"{f['start']:08x} {f['end']:08x}\n" for f in fns))
        doc = json.loads((self.dir / "data" / "modules.json").read_text())
        next(m for m in doc["modules"] if m["name"] == "hstp_trans_main.prx")["function_count"] = len(fns)
        (self.dir / "data" / "modules.json").write_text(json.dumps(doc, indent=2) + "\n")
        self.assertRefused(self.sb.promote(TWIN), "target-drift")
        self.assertRefused(self.sb.gate("retarget", "--reason", "test"), "do not hold for the new target")


class LedgerMutations(GateCase):
    def ledger_lines(self) -> list[bytes]:
        return (self.dir / "data" / "accepted-ledger.jsonl").read_bytes().split(b"\n")[:-1]

    def put(self, lines: list[bytes]) -> None:
        (self.dir / "data" / "accepted-ledger.jsonl").write_bytes(b"".join(x + b"\n" for x in lines))

    def test_delete_a_proof_line_by_hand(self) -> None:
        lines = self.ledger_lines()
        self.put(lines[:1] + lines[2:])
        self.before = self.sb.state()
        self.assertRefused(self.sb.promote(TWIN), "ledger")
        self.assertCheckFails("ledger")

    def test_duplicate_a_proof_with_recomputed_digests(self) -> None:
        # a forger copies a proof (a clone counted twice) and recomputes the proof, body and header digests
        lines = self.ledger_lines()
        header = json.loads(lines[0])
        proofs = [json.loads(x) for x in lines[1:]]
        twin = next(p for p in proofs if p["source"] == TWIN)
        dup = dict(twin, source="src/sharing_main/wrap_angles_again_0018a3f0.cpp")
        dup["proof_sha256"] = P.proof_digest(dup)
        proofs.append(dup)
        proofs.sort(key=lambda p: (p["module"], int(p["start"], 16), p["source"]))
        body = P.ledger_body(proofs)
        header.update(count=len(proofs), bytes=P.bytes_by_module(proofs), ranges_sha256=P.ranges_digest(proofs),
                      body_sha256=P.sha256(body))
        (self.dir / "data" / "accepted-ledger.jsonl").write_bytes(P.canon(header) + b"\n" + body)
        self.before = self.sb.state()
        self.assertRefused(self.sb.promote(TWIN), "out of order or duplicates|overlaps")
        self.assertCheckFails("ledger")

    def test_silent_removal_rewritten_through_the_writer(self) -> None:
        # a forger uses the ledger writer itself to drop a proof (a consistent ledger and journal): only the history
        # shows it, and `check --base` refuses it
        r = self.sb.py("import hstproof as P\nled = P.load_ledger()\nproofs = [p for p in led.proofs if p['source'] != "
                       f"{DTOR!r}]\nP.write_ledger(proofs, {{'event': 'promote', 'added': [], 'replaced': [], 'dropped': [], "
                       "'refused': 0}, led.header['toolchain'], led.header['target'])")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertEqual(self.sb.check().returncode, 0)       # consistent on its own (the source is merely unproven)
        self.assertCheckFails("accepted bytes at HEAD are no longer accepted", args=("--base", "HEAD"))

    def test_fabricated_proof_through_the_writer(self) -> None:
        # a forger writes a proof for a source that does not match, with every digest right: `verify --since HEAD`
        # (the push procedure runs it) recompiles it and refuses
        shutil.copyfile(REAL / NEW, self.dir / NEW)
        self.sb.edit(NEW, "{\n}", "{\n  *(volatile int *)0x40 = 1;\n}")     # does not match retail
        code = f"""
import json, hstproof as P
led = P.load_ledger()
src = {NEW!r}
base = next(p for p in led.proofs if p['module'] == 'EBOOT.BIN')
fake = dict(base, source=src, start='0x00000008', end='0x00000010', symbol='FUN_00000008',
            source_sha256=P.file_sha256(P.ROOT / src), cc='mwcc219', lang='c99')
fake['deps'] = dict(base['deps'], source_sha256=fake['source_sha256'], symbols=[])
fake['deps_sha256'] = P.digest(fake['deps'])
fake['compared'] = {{'.text': {{'addr': '0x00000008', 'size': 8, 'sha256': '0' * 64}}}}
fake['proof_sha256'] = P.proof_digest(fake)
P.write_ledger(led.proofs + [fake], {{'event': 'promote', 'added': [], 'replaced': [], 'dropped': [], 'refused': 0}},
               led.header['toolchain'], led.header['target'])
"""
        r = self.sb.py(code)
        self.assertEqual(r.returncode, 0, r.stderr)
        v = self.sb.gate("verify", "--since", "HEAD")
        self.assertEqual(v.returncode, 1, v.stdout + v.stderr)
        self.assertIn(f"DIFFERENT {NEW}", v.stdout)

    def test_widened_range_through_the_writer(self) -> None:
        # a forger widens an accepted proof to the next function and re-chains everything: the proof no longer agrees
        # with its own source's header, so `check` refuses it
        r = self.sb.py("import hstproof as P\nled = P.load_ledger()\nout = []\nfor p in led.proofs:\n"
                       f"    if p['source'] == {TWIN!r}:\n"
                       "        p = {k: v for k, v in p.items() if k != 'proof_sha256'}\n"
                       "        p['end'] = '0x0018a600'\n"
                       "        p['proof_sha256'] = P.proof_digest(p)\n"
                       "    out.append(p)\n"
                       "P.write_ledger(out, {'event': 'promote'}, led.header['toolchain'], led.header['target'])")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertCheckFails("header differs from the proof")

    def test_source_edited_during_the_run(self) -> None:
        code = f"""
import hstpromote as R, hstproof as P
ctx = R.Context()
class A: parallel = 2; keep_run = False
run = R.new_run(ctx, A)
orig = R.Run.run_own_compile
def edited(self, cands):
    out = orig(self, cands)
    p = cands[0].path
    p.write_text(p.read_text() + "\\n/* edited while the run was compiling */\\n")
    return out
R.Run.run_own_compile = edited
proofs, refused = run.verify(R.candidates_for([{TWIN!r}], P.tree_sources()))
print(sorted(proofs), refused)
"""
        r = self.sb.py(code)
        self.assertIn("source-changed", r.stdout, r.stdout + r.stderr)

    def test_rewrite_the_journal(self) -> None:
        j = self.dir / "data" / "accepted-journal.jsonl"
        ev = json.loads(j.read_bytes().splitlines()[0])
        ev["subsumed"] = {}
        j.write_bytes(P.canon(ev) + b"\n")
        self.before = self.sb.state()
        self.assertRefused(self.sb.promote(TWIN), "journal")

    def test_delete_the_journal(self) -> None:
        (self.dir / "data" / "accepted-journal.jsonl").unlink()
        r = self.sb.promote(TWIN)
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("journal", r.stdout)

    def test_truncate_the_ledger(self) -> None:
        p = self.dir / "data" / "accepted-ledger.jsonl"
        p.write_bytes(p.read_bytes()[:-40])
        self.before = self.sb.state()
        self.assertRefused(self.sb.promote(TWIN), "ledger")

    def test_ledger_bound_to_another_toolchain(self) -> None:
        r = self.sb.py("import hstproof as P\nled = P.load_ledger()\nproofs = [dict(p, toolchain='1' * 64) for p in led.proofs]\n"
                       "for p in proofs: p['proof_sha256'] = P.proof_digest(p)\n"
                       "P.write_ledger(proofs, {'event': 'promote'}, '1' * 64, led.header['target'])")
        self.assertEqual(r.returncode, 0, r.stderr)
        self.before = self.sb.state()
        self.assertRefused(self.sb.promote(TWIN), "binding")

    def test_concurrent_promotion_is_locked_out(self) -> None:
        (self.dir / "try" / "promote.lock").mkdir()
        self.assertRefused(self.sb.promote(TWIN, "--lock-wait", "0"), "another promotion holds")

    def test_seed_runs_only_once(self) -> None:
        self.assertRefused(self.sb.gate("seed"), "a ledger exists")


class UnitChecks(unittest.TestCase):
    """Pure functions, no sandbox."""

    def test_ranges(self) -> None:
        self.assertEqual(P.merge([(8, 12), (0, 4), (4, 8)]), [(0, 12)])
        self.assertEqual(P.subtract([(0, 100)], [(10, 20), (30, 40)]), [(0, 10), (20, 30), (40, 100)])
        self.assertEqual(P.subtract([(0, 100)], [(0, 100)]), [])
        self.assertEqual(P.length([(0, 10), (5, 15)]), 15)          # a byte counts once
        self.assertEqual(P.intersect_length([(0, 10)], [(5, 20)]), 5)

    def test_masked_difference(self) -> None:
        obj = bytes.fromhex("0000000c" "00000000")                  # jal 0 ; nop (little-endian words)
        linked = bytes.fromhex("74b0000c" "00000000")               # jal with the target filled in
        self.assertIsNone(P.masked_difference(obj, linked, [P.Reloc(0, 4, 1)]))
        self.assertEqual(P.masked_difference(obj, linked, []), 0)   # without the relocation it is a change
        bad = bytes.fromhex("74b00008" "00000000")                  # j instead of jal: not a relocation fill
        self.assertEqual(P.masked_difference(obj, bad, [P.Reloc(0, 4, 1)]), 3)
        with self.assertRaises(P.ProofError):
            P.masked_difference(obj, linked, [P.Reloc(0, 99, 1)])   # an unknown relocation type is not trusted

    def test_overlapping_proofs_are_invalid(self) -> None:
        base = {"schema": P.PROOF_SCHEMA, "module": "EBOOT.BIN", "symbol": "f", "cc": "mwcc219", "lang": "c99",
                "toolchain": "a" * 64, "target": "b" * 64, "compared": {}, "object_sha256": "c" * 64,
                "status": "EXACT", "gate": {"result": "pass", "idioms": []}, "run": "r", "verified_utc": "t"}

        def proof(src, s, e):
            p = dict(base, source=src, start=P.hx(s), end=P.hx(e), source_sha256="d" * 64)
            p["deps"] = {"source_sha256": "d" * 64, "headers": [], "unresolved_includes": [], "symbols": []}
            p["deps_sha256"] = P.digest(p["deps"])
            p["proof_sha256"] = P.proof_digest(p)
            return p
        P.validate_proofs([proof("a", 0, 8), proof("b", 8, 16)])
        with self.assertRaises(P.ProofError):
            P.validate_proofs([proof("a", 0, 12), proof("b", 8, 16)])
        with self.assertRaises(P.ProofError):
            P.validate_proofs([proof("a", 0, 8), proof("a", 8, 16)])  # one source, two ranges


if __name__ == "__main__":
    unittest.main()
