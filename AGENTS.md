# Agent guide

Routing and integrity rules for coding agents (and people) working in this repository: a matching decompilation of
the PSP game Hot Shots Tennis: Get a Grip. The detailed rules live in the documents below; this file says which one
to read for which task and lists the rules that are never relaxed.

## Read first

| Task | Read |
| --- | --- |
| Setting up, the tools, the source header keys | [docs/GUIDE.md](docs/GUIDE.md) (Setup, Verify, Source conventions, Tools) |
| Matching a function: method, file format, note line, source quality, near misses | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Compiler behaviour (saves many compiles) | [docs/MWCC-DIGEST.md](docs/MWCC-DIGEST.md), then the newest lines of the append-only log [docs/MWCC-NOTES.md](docs/MWCC-NOTES.md) |
| What counts as done, and the progress tiers | [docs/COMPLETION.md](docs/COMPLETION.md), [docs/PROGRESS.md](docs/PROGRESS.md) |
| Prompting a matching agent for one batch | [docs/LANE-PROMPT-TEMPLATE.md](docs/LANE-PROMPT-TEMPLATE.md) |

The maintainers' tree also holds the external-lane brief (`docs/LANE-TASK.md`, `docs/AGENT-BRIEF.md`) and the lane
and push scripts; `tools/hstpublish.py` (`PRIVATE_PATHS`) keeps those, and the accepted ledger, out of the public
repository. Where the brief and this file disagree, the stricter rule applies.

## Kinds of work

- **Matching** (most work): one function at a time, by the method in CONTRIBUTING.md. Experiments go in your own
  directory under `try/` (gitignored); an EXACT source goes to `src/<module dir>/`; a close attempt goes to `wip/`.
  Agents running as a lane stay inside their lane directory and follow their prompt.
- **Tools.** The verdict depends on the comparison code (`tools/hstmatch.py`, `hstpromote.py`, `hstproof.py`,
  `hstgate.py`, `hstconfig.py`), the compiler profiles and `include/`. The toolchain lock
  (`data/toolchain-manifest.json`) hashes all of them, so a change there is finished only when
  `python tools/hstpromote.py migrate-toolchain --reason "..."` has reverified every accepted proof under the new
  toolchain and written the lock; if any proof fails, the change is wrong, not the proof. Add synthetic, game-free
  tests (`tools/test_*.py`, each runnable as `python tools/test_<name>.py`).
- **Accepted ledger and publishing** are maintainer work: `tools/hstpromote.py` is the only writer of
  `data/accepted-ledger.jsonl`, `data/accepted-journal.jsonl` and the two lock files; the public tree is built by
  the maintainers' release tooling. Agents never push, publish or rewrite history.

## Integrity rules (never relaxed)

- **Only EXACT counts.** A source is a match only when `python tools/hstmatch.py <file>` reports `EXACT` for the
  installed file: the compiled and linked bytes equal retail with no masking. A maintainer re-verifies every
  harvested source; a claim without matcher output is not a result.
- **Real source only.** No machine code in a source: no `asm` statements that emit instructions, no
  `.word`/`.byte`/`.incbin`, no code-emitting pragmas, no data steered into `.text`, no `extra=` header or `--flags`
  result in `src/`. `tools/hstgate.py` refuses such a source (`EXACT_FAILS_GATE`). Allowed `asm`, reported as
  idioms: a `register T x asm("$N")` binding, an empty `asm("" ::: "memory")` barrier, and **inline VFPU asm
  where the original SDK or game code used it** (owner decision 2026-10-27, the LittleBigPlanet rule): the header
  says `asm=vfpu`, a line starting `Original inline VFPU asm:` gives the reason, and every instruction in the
  asm is a VFPU instruction (no other instruction, label or VFPU branch; loads/stores as `offset(%N)`, or a bare
  `%N` whose constraint is `"m"` for a local or parameter; the only directives are the SDK's
  `.set push`/`noreorder`/`pop` style mode switches; both extensions approved by the owner 2026-10-27). Write
  everything else in C; one non-VFPU instruction makes it `asm-emit` again.
- **Compiler profiles.** MWCC build 219 and the Sony gcc131/gcc171 kits are the defaults; `cc=gcc402x-*` (GCC 4.0.2 +
  pspdev patch + Allegrex tuning, owner decision 2026-10-27) is secondary, only for functions the defaults cannot
  reproduce.
- **No steering tricks presented as code.** Avoid duplicated conditions, dummy locals or parameters, `volatile`,
  self-assignments and gratuitous casts or `goto`; when one is the only exact form found, keep it and name it in the
  note line ("matching idiom: ..."). Note lines must be true.
- **Relocatable.** Name every address through a symbol (`FUN_`/`DAT_` address names, or a line appended to
  `symbols/<module>.txt`), never through a numeric constant cast to a pointer. Decode `lui`/low-half pairs with the
  sign-extended low half before naming a symbol.
- **Never edit the verdict.** Do not edit `tools/` as part of a matching change, do not edit or delete existing
  lines of `symbols/` (append only), and never hand-edit the ledger, journal or lock files.
- **Never lower a figure.** A change that would drop accepted bytes or lower a progress figure is refused, not
  worked around; `hstpromote.py` refuses a promotion that loses coverage, and only an explicit `retract` with a
  reason removes accepted bytes.
- **No game material.** Never commit or send retail modules, decrypted assets, disassembly dumps of whole modules,
  keys, saves or private captures. Never upload retail bytes, disassembly or pseudocode to a public or community
  hosted tool (decomp.me, paste sites, public trackers); the model providers the owner already uses for matching
  lanes may receive the function-level disassembly and pseudocode a lane needs (owner decision 2026-11-10). This
  permits no other hosted tool. Keep machine-specific paths in the gitignored `hst-config.json`.
- **Shared resources.** Use `--parallel 4` at most for `hstmatch.py`; do not run the tools that write into `src/`
  (`hstgen`, `hstclone`, `hsttmpl`, `hstperm`, `hstnewlib`) or build batches (`hstqueue`) unless the maintainer asked;
  do not search recursively outside `src/`, `wip/`, `docs/`, `symbols/`, `include/` and your own `try/` directory.
