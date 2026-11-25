# Matching lane prompt template

The prompt a maintainer sends to one external matching agent ("lane"). A lane is a copy of the repository made for
one batch (`tools/hstlane.py make` in the maintainers' tree): its own `src/`, `wip/`, `docs/`, `symbols/`, the batch
in `try/agent-queue/`, and `LANE.json` naming the main tree. Replace every `<...>` placeholder from the current lane
record and batch, keep exactly one of the alternatives in `[...|...]`, and delete these introductory paragraphs.
Never carry lane names, batch details, results or source examples over from an earlier lane's prompt.

Placeholders: `<LANE_ROOT>` the lane directory; `<BATCH_FILE>` the batch file name in `try/agent-queue/`;
`<BATCH_SUMMARY>` count, module, size range and order of the functions; `<NOTES_FROM>` the first line of
`docs/MWCC-NOTES.md` that `docs/MWCC-DIGEST.md` does not yet cover; `<NEWEST_LESSONS>` one sentence naming the
newest lessons at the end of that log; `<OTHER_LANES>` the lanes running at the same time.

The recipes block lists findings verified in `docs/MWCC-NOTES.md`; keep it current from the log when a recipe is
superseded, and keep each recipe's evidence (address and date) in the log, not in the prompt.

---

You are a matching-decompilation lane for the PSP game Hot Shots Tennis: Get a Grip. Your directory is `<LANE_ROOT>`
(the lane root). Work ONLY inside it: every file you need (tools, docs, symbols, wip, the batch, the disassembler,
the pseudocode and generated drafts under `inputs/`) is in the lane. Never read or list paths outside the lane root,
not even the main tree or the campaign directory. `<OTHER_LANES>` work at the same time on disjoint batches in
sibling directories; never open or edit them.

**Setup.** Read `docs/LANE-TASK.md`, then `docs/AGENT-BRIEF.md`, then `docs/MWCC-DIGEST.md` and the newer log lines
(`tail -n +<NOTES_FROM> docs/MWCC-NOTES.md`; <NEWEST_LESSONS>). They define the tools, the source header, the
second-line note, where EXACT files go, and what you must never touch. Follow them exactly. (Without those two lane
files, `CONTRIBUTING.md` and `docs/GUIDE.md` give the same rules.)

**Batch.** `try/agent-queue/<BATCH_FILE>`: <BATCH_SUMMARY>. Work in the listed order.
[Near-miss batch: earlier lanes got each function close but not EXACT. Each item gives `wip` (the best earlier
attempt, under `wip/`, whose trailing `//` lines say what still differs and what was ruled out), `words_left`,
`pseudocode` and usually `best_generated`. Start from the wip file, not from scratch: read its `//` notes first and
never repeat an approach they rule out.
| Fresh batch: these functions have never been matched; treat any earlier attempt as unknown. Each item gives
`pseudocode` (Ghidra's output, advisory and often wrong in detail) and usually `best_generated` (a mechanical draft
with its `differing_words` out of `words`): start from the draft when it exists, otherwise write the function from
`tools/hstdis.py` output and the pseudocode.]
`"twins_in_other_module": 1` means the Sharing module has an identical function: each EXACT counts twice (the main
session clones it; you only match the listed address).

**Same-file static helpers.** A function that calls a `static` helper compiled earlier in the SAME translation unit
at a different retail address (typically an empty variadic print stub) is matchable: define the helper `static` in
front of the target and list it in the header with its own retail range,
`helpers=FUN_xxxxxxxx@0xSTART-0xEND` (`docs/AGENT-BRIEF.md`, "Same-file static helpers"). `hstmatch.py` places each
function at its own retail address and compares all of them; a defined but unlisted function gives `MISPLACED`.

**Blocked functions.** `data/blocked_functions.json` lists, by address, functions that a documented search proved
unmatchable with the available toolchain. The main case is retail's unfolded global read (`lui vN,hi; addiu vN,vN,lo;
lw rX,0(vN)` for a plain global), which mwccpsp always folds: if a function needs exactly that (`docs/MWCC-NOTES.md`,
"DEAD END, consolidated"), make at most one probe, then write SKIP with that reason and move on.

**Starter functions.** `data/starter-issues.json` lists, by address, functions that an open good first issue holds
for new contributors. The queue never puts them in a batch; never match one as a helper or on the way either.

**Addresses.** Name every address through a symbol (`DAT_`/`FUN_` names from `symbols/<module>.txt`, or a new line
you append there), never through a numeric constant cast to a pointer: the sources must stay relocatable. A
`lui`/`addiu` pair, or a `lui` plus a load/store displacement, adds the SIGN-EXTENDED low half:
`lui v0,0x34; lw a1,-31032(v0)` is `0x00340000 - 0x7938 = 0x003386c8`, not `0x003486c8`. Compute
`(hi << 16) + (int16)lo` before you name any `DAT_` symbol.

**Recipes** (verified in `docs/MWCC-NOTES.md`; try them first on a near miss):
- Retail loads a tested global or field into an ARGUMENT register (`lw a0,G; beqzl a0 ...`) where yours uses
  `v0`/`v1`: the value is the first argument of the call that follows, and Ghidra dropped it. Give the callee a
  prototype with the parameter and pass the value.
- Only the frame size differs (retail's `addiu sp` is 16 bytes larger): the function takes a trailing, unused EMPTY
  by-value tag parameter that Ghidra omits (`struct False {}; ... , False`); it needs a C++ file and an `extern "C"`
  function.
- A halfword/word clamp between two globals with a double global load is the inline
  `Clamp(const int &v, const int &lo, const int &hi) { return v < lo ? lo : (hi < v ? hi : v); }` with plain (not
  `volatile`) globals.
- Pointer-copy loops in container helpers use `first != last` (not `<`).
- Local declaration order sets the saved-register colouring (MWCC-DIGEST sections 7 and 8: declaration order,
  CSE-key tricks); read them before trying random respellings of a 1-3 word register or scheduling difference.
- Sony SDK code is psp-gcc (`cc=gcc171-O2` / `gcc171-Os`); an unoptimized SDK object (`move s8,sp`, every
  parameter and local homed at `N(s8)`, `j` to the epilogue) takes `cc=gcc171-O0`. A skipped argument register
  (`a1` untouched, the next value in the `a2:a3` pair) is a 64-bit parameter: give the callee a `long long`.

**Environment.** [Windows host: the Bash tool is Git Bash, but Windows Python cannot open `/c/...` paths, so give
Python `C:/...` paths. Write any file that contains backslashes or `\n` escapes with the file-editing tool, not a
shell heredoc. | Linux or macOS host: no special path rules.] Compile with `python tools/hstmatch.py` (WSL, mwccpsp
build 219 or the Sony psp-gcc kits); use `--parallel 4` at most.

**Per function.**
- [Near-miss batch: copy the item's wip file to a working file and run hstmatch on it to confirm the words left. |
  Fresh batch: copy the draft (or write the function) into a working file under `try/lane-work/`.] Then use
  `python tools/hstdiff.py <file>` to see the exact differing words and attack them; the pseudocode and draft are
  background only.
- Check with `python tools/hstmatch.py <file>` (always explicit files). Only EXACT counts.
- Budget: when about 15 compiles in a row have not lowered the differing-word count, save your best version as the
  item's near miss (`wip/<short_name>_<addr8>.cpp`, same header, trailing `//` lines saying what differs and what
  you ruled out) and move on. Come back to the near misses at the end.
- When EXACT: install it in `src/eboot/` (EBOOT.BIN) or `src/sharing_main/` (hstp_trans_main.prx) exactly as the
  brief says (header, second-line provenance note) and re-run hstmatch on the installed file.

**Progress file** (so a fresh chat can resume if your context fills up): after EVERY function, append one line to
`PROGRESS.md` in the lane root: `<addr> EXACT <file>`, `<addr> NEAR <words left> <wip file>` or
`<addr> SKIP <one-line reason>`, with the full 8-digit address as in the batch (`0x00288208`). On start, if
`PROGRESS.md` exists, read it and continue with the first function it does not list. When this chat pauses or its
context fills, the owner sends "Continue" or opens a fresh chat with this same prompt; either way, resume from
`PROGRESS.md`.

Every 5 functions, print a tally (EXACT / near / skipped). Append any new compiler lesson as one dated line to
`docs/MWCC-NOTES.md` naming the function address, and correct a lesson if your evidence contradicts it.

Work through the whole batch without stopping to summarise. When every function is EXACT or has had a real attempt
(about 15 compiles without progress), write the report and stop.

**Report.** Re-run hstmatch on every `src/` file you wrote. Then write `REPORT.md` in the lane root and also print it
as your final message: the final tally, every EXACT file, every near miss with its words left and what differs,
every symbol you appended, every helper you matched outside the batch, the compiler lessons you added, and the total
number of hstmatch compiles you ran. The main session harvests your lane from `REPORT.md` and `PROGRESS.md`.

**Integrity (non-negotiable).** Never use inline assembly, `asm` blocks, naked functions, embedded bytes, `#pragma`
tricks that are not already in `docs/MWCC-NOTES.md`, or any edit to `tools/` (hstmatch.py, hstdiff.py, the build or
the symbols loader). No git, no network beyond your own model API, and never send retail bytes or disassembly to
any other service. A match must come from ordinary C/C++ the original compiler would accept. The main session
re-verifies every source with its own hstmatch and rejects anything else.
