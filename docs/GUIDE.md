# Project guide

Everything behind the [README](../README.md): what the sources are, how to set up and verify, the repository layout, conventions, tools, the publish filter and licensing.

## What the sources are

The function lists in `data/` are the reviewed boundaries of each module: 19,634 functions, with a few dozen
candidate spans still open. Runtime and static module surveys found no other game code. What counts as done, and
the tiers progress is reported in, are defined in [`COMPLETION.md`](COMPLETION.md).

- Matching proves the *compiled bytes*, not that the source is what the developers wrote. Names are mostly the
  address names from Ghidra (`FUN_0011d894`), and types are provisional. Every note line says "names and types
  await human review" until a person has reviewed the source.
  <!-- hstprogress:begin guide-source-kinds -->
- Of the 12,774 sources (measured 2026-11-02), 7,207 are address-substituted **clones** of another matched source
  (retail contains the same code many times), 1,053 are mechanical translations of Ghidra pseudocode, 1,870 are such
  translations corrected by hand, 2,413 are hand-written, 136 are instances of container templates, and 85 are newlib
  library functions translated from newlib sources (37 more are clones of them, counted among the clones), and 10 have
  no known note kind.
- 122 of the sources are newlib library functions compiled with Sony's `psp-gcc` kits; all others are game code
  compiled with Metrowerks `mwccpsp`.
  <!-- hstprogress:end -->
- Some SDK functions use the Allegrex `ins rd,$zero,...` instruction, which neither published Sony `psp-gcc` kit
  emits (nor does a current GCC in the same way); they cannot be matched until a later kit is available (see
  `docs/MWCC-NOTES.md`).
- `python tools/hstlint.py` reports how the sources read (matching idioms such as `volatile`, `goto`, duplicated
  conditions), and 1,067 symbols are declared with differing types in different sources. Cleaning that up is future
  work.

Not published here: reconstructions of Sony SDK code, that is, the `psp-gcc`-built SDK runtime of the two
executables and the Sony system-library modules the game loads (`libfont`, `scePsmf`, `scePsmfP`). Those functions
count as unmatched in the table above; see [Origin classes and the publish filter](#origin-classes-and-the-publish-filter).

## Requirements

The tools run on a **Windows host and drive the compilers inside WSL** (they call `wsl.exe`).

- Python 3.12 or newer (checked with 3.14; no third-party packages).
- WSL with an Ubuntu distribution.
- `libc6-i386` in WSL (`sudo apt install libc6-i386`): the Sony `psp-gcc` kits are 32-bit Linux programs.
- [wibo](https://github.com/decompals/wibo) 1.2.0, to run the Windows compiler under Linux.
- `mwccpsp.exe` 3.0.1 build 219 (the decomp.me build `219`).
- The decomp.me Sony `psp-gcc` 1.3.1 and 1.7.1 kits.
- PSPDEV binutils (`psp-ld`, `psp-objcopy`, `psp-objdump`) for linking at the retail address and for disassembly.
- Your own decrypted copies of the two executables (see [Setup](#setup)); `tools/hstbootstrap.py` turns them into the
  campaign directory the tools read.

Optional: newlib source trees (library reconstruction, `hstnewlib.py`), and Ghidra 12.x with a JDK 21
(`ghidra/run_commit_decompile.ps1`).

## Setup

1. Install the compilers in WSL, on the **native Linux filesystem** (the i386 `psp-gcc` cannot stat files on
   `/mnt/c`). The layout the tools expect:

   ```text
   <mwcc_native>/wibo
   <mwcc_native>/builds/219/mwccpsp.exe
   <gcc_home>/psp-gcc-1.3.1/bin/psp-gcc
   <gcc_home>/psp-gcc-1.7.1/bin/psp-gcc
   <pspdev_bin>/psp-ld   (and psp-objcopy, psp-objdump)
   ```

   `pspdev_bin` may live on `/mnt/c`.

2. Create the campaign directory, `<campaign_root>`, from your own decrypted copies of the two executables this
   repository reconstructs (the disc is Hot Shots Tennis: Get a Grip, UCUS98701, North America, version 1.01):

   ```text
   python tools/hstbootstrap.py --out <campaign_root> --eboot <decrypted EBOOT.BIN> --sharing <decrypted DATA.PSP>
   ```

   | Module | On the disc | Decrypted size | SHA-256 of the decrypted module |
   |---|---|---|---|
   | `EBOOT.BIN` | `PSP_GAME/SYSDIR/EBOOT.BIN` | 4,178,820 | `ee39c40ee70af6bb5c76a0789898a9dc8c0a6ddcd0bd4d9d76f6200537f34bbe` |
   | `hstp_trans_main.prx` (Sharing Version) | `DATA.PSP` inside `PSP_GAME/USRDIR/bundle_data/trial/hstp_trans.pbp` | 4,952,948 | `5418712bbe4a4339cf30e5c78b486924f1d24fe313d8bf8b0d56fdc3132fe6ec` |

   The tool checks each file against these digests (a gzip-compressed image is inflated first), copies it into
   `<campaign_root>/inputs/`, and writes `ledger/campaign.json` (load segment and reviewed function boundaries,
   from `data/`) and `inputs/revision-selection.json`. It decrypts nothing, and this repository contains no
   decryption code: decrypt your own disc on your own machine with a tool of your choice, for example Nakagawa
   Recomp's `nk_decrypt` ([how to run it](https://github.com/Jstar269/nakagawa-recomp/blob/main/docs/SETUP.md#encrypted-executables)). A module you leave out is skipped, and `hstmatch.py` then cannot check that module's sources.

3. Describe your machine to the tools:

   ```text
   copy hst-config.example.json hst-config.json
   notepad hst-config.json
   python tools/hstconfig.py check
   ```

   `hst-config.json` is gitignored. Every key can instead be set with the environment variable `HST_<KEY>`
   (for example `HST_CAMPAIGN_ROOT`), which overrides the file. Keys (all documented in
   `hst-config.example.json`):

   | Key | Kind | Meaning |
   |---|---|---|
   | `campaign_root` | host | `<campaign_root>`: ledger and revision selection (needed by almost every tool) |
   | `pspdev_bin` | WSL | PSPDEV binutils directory |
   | `mwcc_native` | WSL | wibo and `builds/<build>/mwccpsp.exe` |
   | `gcc_home` | WSL | the `psp-gcc-1.3.1` and `psp-gcc-1.7.1` kits |
   | `newlib_home` | WSL | newlib source trees (`hstnewlib.py` only) |
   | `newlib_obj_root` | host | built newlib objects (`hstlib.py index` only) |
   | `ext_root` | host | workspaces for external agent lanes (maintainers only; not needed to build or verify) |
   | `mwcc_builds_dir`, `wibo_exe` | host | host copies, for the digests `hstsample.py` records |
   | `ghidra_headless`, `java_home` | host | `ghidra/run_commit_decompile.ps1` only |

   A tool asks for a key only when it needs it, so `--help` and the local tools work without a configuration; an
   unset key stops the tool with a message that names the key.

4. Create the scratch directory the report-writing tools use: `mkdir try`. (`build/` is created on demand. Both are
   gitignored.)

## Verify

```text
python tools/hstmatch.py --all                   # rebuild and compare every source under src/
python tools/hstmatch.py --all --parallel 4      # concurrent compiles (default 6; 4 recommended, AGENTS.md)
python tools/hstmatch.py src/eboot/FUN_00000000.c    # specific files
```

The result is one line per source (`--quiet` prints only the failures) and a total such as `EXACT 7761/7761`; the exit
status is 0 only when everything is EXACT. Statuses: `EXACT`, `MISMATCH` (with the number of differing words),
`COMPILE_FAILED`, `LINK_FAILED`, `MISSING_SYMBOLS`, `UNPLACED_SECTION`, `SECTION_MISMATCH`, `NO_SYMBOL`,
`MISPLACED`, `EXACT_EXTRA_FLAGS` (an experiment with `--flags` or a header `extra=`, which never counts as a
match) and `EXACT_FAILS_GATE` (the bytes match but the source fails the fake-match gate of `hstgate.py`: `asm` that emits
instructions (other than VFPU-only inline asm under the `asm=vfpu` rule), `.word`/`.byte`/`.incbin`, code-emitting pragmas, or `extra=` in `src/`; it never counts either), and `UNPLACED_FUNCTION` (a `helpers=` source defines a function its header does not place). `--json out.json` writes the results (with `words_differ` per source). A source whose target depends on a `static` helper defined earlier in the same file lists it in the header, `helpers=NAME@0xSTART-0xEND[,...]`: mwccpsp emits every function in its own `.text` section, so each function is linked and compared at its own retail address, and the file is EXACT only when all of them match (docs/AGENT-BRIEF.md, "Same-file static helpers"); the source claims only the target's range. Build products go to `build/<module>/<dir>__<stem>__<digest>/` (the digest is a short hash of the source's path, so equally named sources in different trees never share them); a full rebuild
shares `build/` with any concurrent run, so run one at a time.

## Whole-module build

`hstmatch.py` proves one function at a time. `python tools/hstbuild.py <module>` (a module name, or `all`) is the step
toward rebuilding a module from source: it takes the objects `hstmatch.py` compiled (the same profile and flags), keeps
those whose source is EXACT, and links them in place into the module's layout. Every retail byte that no source covers is
*scaffolding*: assembly (one `.word` section per unmatched function) and blobs generated from your own decrypted module
into `build/whole/<module>/`; scaffolding is tier 0 and never counts as progress. The default is the *public* build:
sources of the origin classes the public tree leaves out (the Sony SDK reconstructions) are not compiled; their bytes are
supplied from your own executable like any other scaffolding and counted separately (`scaffolding.owner_supplied_bytes`);
`--private` compiles them too.

The scaffolding is relocatable: every word a retail relocation covers has its field cleared and a `.reloc` of the same
type against a symbol defined as tile + offset, so the link writes it from the layout. The linker script places every
tile at its retail address in the retail output sections, `psp-ld -q` links, and the build then *generates* the
relocation tables (a compiled source's entries from its own object, in its own order) and the container (ELF header,
program header, section header table, `.shstrtab`), assembles the whole file and compares it with the retail module by
SHA-256. Only `.comment` is copied from the retail file (`container.bytes_from_retail`). The four Sony library modules
keep their retail container and format-B relocation table. The format and the order of the tables are described in
[RELOCATION.md](RELOCATION.md). `hstbuild.py` runs `hstmatch.py` on the sources first (`--reuse-isolation
hstmatch.json` reuses a `hstmatch.py --json` run and its `build/` directories); one run per module at a time
(`try/build/<module>.lock`).

```text
python tools/hstbuild.py EBOOT.BIN                  # one module
python tools/hstbuild.py EBOOT.BIN --shift 0x1000   # and the shifted build (gate 5, static half)
python tools/hstbuild.py all --parallel 4           # all six
```

`--shift N` links the same objects again with N bytes of padding ahead of the code and N more ahead of the data (a
multiple of 128 for the game modules) and compares every byte with an expectation computed from the retail image and its
relocation tables alone. The report then says `shifted_build_passes`, `shifted_runtime: "not run"` (the runtime half is
for Nakagawa Recomp and the PSP), the numeric-address census and `t3_source_bytes`. `--out-dir` and `--report-dir` keep
an experiment away from the build's own files; `--mutate flip|drop|swap|addend` builds with one deliberately wrong
scaffolding relocation record (a check that the comparisons catch it). For compiled-object mutation checks,
`--mutate-scope compiled --mutate addend` changes a relocation site's in-place addend, while
`--mutate-scope compiled --mutate instruction` changes a non-relocated text word; both report the source path and
address.

Each run writes `try/build/<module>-report.json`: per section whether the link equals retail, per relocation table
whether the generated table equals retail, the container parts, the sources reproduced in place (their bytes, with
their relocation-table entries, are tier 2 for `hstprogress.py`), the sources that are EXACT alone but differ in the full
link, the sources left out and why.

## Repository layout

| Path | Contents |
|---|---|
| `src/` | The exact-matched sources, by module: `eboot/` (`EBOOT.BIN`), `sharing_main/` (`hstp_trans_main.prx`). Do not edit by hand without re-running the matcher. |
| `wip/` | Near misses (within a few words of exact) with comments on what differs. Not built by `--all`, not verified. |
| `data/` | `modules.json` (each module's description, SHA-256 and load segment) and `functions-<module>.txt` (its reviewed function boundaries). `tools/hstbootstrap.py` reads both. `blocked_functions.json` lists, by address only, the reviewed functions that a documented search found unmatchable with the available toolchain; `tools/hstattempts.py` and `tools/hstqueue.py` read it. |
| `symbols/` | `symbols/<module>.txt`: `name = 0xaddr;` maps for names that are not address-named (C++ runtime, vtables, library functions). Append-only in practice. |
| `include/` | Project-authored headers: `hst/` (shared types and structs of the game sources), `stddef.h`, `stdint.h` (the only C library headers the MWCC sources see, via `MWCIncludes`) and `stdarg.h`, `float.h`, `limits.h`, which stand in for the compiler's own headers when `hstnewlib.py` preprocesses newlib sources (`-nostdinc`). |
| `tools/` | The matcher and the other tools, below. |
| `toolchain/drivers/` | `mwcc_wibo_link.py`, the build driver the private campaign pins (compile with wibo, link at the retail address, extract `.text`), and its tests. |
| `ghidra/` | The headless pseudocode export (`HstCommitDecompile.java`, `run_commit_decompile.ps1`) and the reviewed function boundaries of the two large modules. |
| `docs/` | `MWCC-NOTES.md` (append-only log of verified compiler behaviour) and `MWCC-DIGEST.md` (the same, by topic), and `OBJDIFF.md` (the objdiff project, see `hstobjdiff.py`). |
| `third_party/` | `newlib/`: newlib's own licence lists (`newlib-<release>-COPYING.NEWLIB`, verbatim) and the table of every newlib-derived source with its release, upstream file, terms and tarball SHA-256. |
| `LICENSE`, `LICENSES/`, `REUSE.toml` | The licence map: `LICENSE` is the GPL-3.0 text, `LICENSES/` holds every licence text, `REUSE.toml` assigns a licence and copyright to each path (see [Licensing](#licensing)). |
| `ORIGINS.json` | Origin class of every source (`python tools/hstorigin.py`), a snapshot: `hstpublish.py` classifies afresh. |
| `hst-config.example.json` | Template for the gitignored `hst-config.json`. |

Generated: `build/` (matcher output) and `try/` (scratch and reports), both gitignored; `reviews/decisions.jsonl`
(written by `hstreview.py`).

Sony SDK reconstructions and a few maintainer-only files are kept out of the public tree (see
[Origin classes and the publish filter](#origin-classes-and-the-publish-filter)).

## Source conventions

See [CONTRIBUTING.md](../CONTRIBUTING.md) for the full rules; the format in short.

**Header (line 1).**

```c
/* HSTMATCH module=EBOOT.BIN start=0x0011d894 end=0x0011dad8 symbol=FUN_0011d894 lang=c++ */
```

- `module`: the module the function is in (`EBOOT.BIN`, `hstp_trans_main.prx`, `libfont.prx`,
  `scePsmfP_library.prx`); `start` and `end`: the retail span, module-relative; `symbol`: the name the source
  defines for the function. All four are required.
- `lang=c99|c++` (default from the file extension).
- `cc=<profile>` selects the compiler: absent means `mwcc219` (mwccpsp build 219, `-c -O4,p -sdatathreshold 0`);
  `gcc131-` or `gcc171-` plus `O1`, `O2`, `O3` or `Os` select a Sony `psp-gcc` kit (`gcc171x-*` adds `-mext`);
  `gcc131-O0` and `gcc171-O0` select the unoptimized builds (Sony SDK objects with a `move s8,sp` frame pointer).
  `gcc402x-O0`/`O1`/`O2`/`O3`/`Os` is a SECONDARY profile (owner decision 2026-10-27): open-source GCC 4.0.2 with the
  pspdev Allegrex patch and lbp-psp-decomp's measured Allegrex tuning (`-mpreferred-stack-boundary=4`), the kit shared
  with lbp-psp-decomp under the `gcc4_home` setting (provenance: the maintainers' `compilers/lbp-gcc4/PROVENANCE.json`,
  hashed by the toolchain lock). Use it only for functions no gcc131/gcc171 profile reproduces (later-compiler
  shapes: duplicated epilogues, tail calls, word loads where GCC 3.3 narrows).
  <!-- hstprogress:begin guide-profiles -->
  Measured 2026-11-02, the public sources use `mwcc219` (12,652 sources), `gcc171-O2` (103 sources), `gcc171x-O2` (17
  sources) and `gcc171x-O1` (2 sources).
  <!-- hstprogress:end -->
- `rodata=0xADDR`, `data=`, `sdata=`, `bss=`, `sbss=`: where the object's sections go when the function refers to
  them; the matcher then also requires the placed `.rodata`/`.data`/`.sdata` bytes to equal retail.
- `extra="..."`: experimental extra compiler flags; such a source is never counted as a match (none are installed).

**Note line (line 2)** records where the source came from. Exactly one of:

| Kind | Starts with | Meaning |
|---|---|---|
| hand-written | `Hand-written reconstruction from the disassembly;` | Written by a person or agent from the disassembly. |
| generated | `Mechanical translation of Ghidra pseudocode (hstgen.py tag X);` | Output of `hstgen.py`, kept only because it matched. |
| corrected | `Ghidra pseudocode (hstgen.py tag X) corrected by hand: ...` | A generated source with small, listed corrections. |
| clone | `Clone of src/... (hstclone.py tag X);` | An exemplar with addresses substituted, for code that is identical apart from addresses. |
| template | `... (template instance, hsttmpl.py tag X)` | An instance of a project-authored container template (element size and offsets read from retail). |
| library | `Library source: newlib <release> <dir>/<file>.c function <name>, ...` | Upstream newlib source, preprocessed with its comments (licence headers) kept. |

`python tools/hstlint.py` rejects a missing or unknown note kind.

**Shared headers.** `include/hst/types.h` gives the fixed-width names (`u8`..`u64`, `s8`..`s64`; `bool`, `true`,
`false` in C) that replace Ghidra's per-file typedef block (`undefined4`, `uint`, ...). Structs that several sources
use live in `include/hst/<topic>.h` (include guard `HST_<TOPIC>_H`); unknown fields are named by offset (`unk14`).
A header is part of the toolchain lock, so a changed header reverifies every source that includes it.
`python tools/hstread.py rank` lists the sources that read least like real source (address names, Ghidra locals,
raw offset casts).

## Tools

| Tool | Purpose |
|---|---|
| `hstmatch.py` | The matcher: compile, link at the retail address, compare bytes exactly. |
| `hstxverify.py` | Parse ELF artifacts with a second implementation, enforce reviewed function spans, and optionally rebuild with the pinned compiler and linker. |
| `hstbuild.py` | The whole-module build: compiles the sources as `hstmatch.py` does, scaffolds every byte no source covers from the retail module, links in retail order and compares section by section and as a whole image (`docs/COMPLETION.md`, tier 2). See [Whole-module build](#whole-module-build). |
| `hstdiff.py` | After a matcher run, print our disassembly next to retail's. |
| `hstdis.py` | Disassemble retail functions by address (`hstdis.py 0xADDR`). |
| `hstcc.py` | Compile one scratch C/C++ file with the retail MWCC flags and print its disassembly with relocations (quick probes; no header, no link; `--build` for experiments with other mwccpsp builds). |
| `hstconfig.py` | Load, list and check `hst-config.json`; the other tools read their paths through it. |
| `hstcov.py` | Exact-match coverage per module against the campaign ledger. |
| `hstobjdiff.py` | Generate an [objdiff](https://github.com/encounter/objdiff) project (`objdiff.json`, objects under `build/objdiff/`) for per-function diffs against retail and a progress report; `check` verifies it against `hstmatch.py` and `hstcov.py` (see `docs/OBJDIFF.md`). |
| `hstgate.py` | The fake-match gate (`docs/COMPLETION.md`, gate 2): refuses a source with `asm`/`__asm__` that emits instructions, assembler data directives, `#pragma asm`, hand-steered `.text` data or a non-header `#include`; a `register T x asm("$N")` binding, an empty barrier and VFPU-only inline asm under the project rule (header `asm=vfpu`, an `Original inline VFPU asm:` reason line, VFPU instructions only; owner decision 2026-10-27) are allowed and tagged as idioms. `hstmatch.py`, `hstlint.py` and the maintainer's `hstlane.py harvest` (not in the public tree) all call it. |
| `hstlint.py` | Quality report over `src/` (note kinds, matching idioms, conflicting declarations). |
| `hstread.py` | Readability work: `rank` (least readable sources and address pages), `tidy` (Ghidra typedef block to `hst/types.h`), `twins` and `harvest` (port a readability edit to the source's twins; copy a lane's edits back). |
| `hstgen.py` | Translate Ghidra pseudocode into candidate C/C++ variants and install the exact ones. |
| `hstcat.py` | Classify why generated candidates mismatched (frame, register allocation, ...). |
| `hstperm.py` | Random-mutation search that turns near-miss candidates into exact ones; the operators include loop forms (for/while/do-while), compound assignment, index versus pointer arithmetic, temporaries, casts, declaration scope, `register` and field-read order. |
| `hstflagsweep.py` | Compile near-miss sources under every mwccpsp build and a set of option combinations (`--flags`, so never a profile match) and tabulate the words that still differ; a 0 is a finding about the translation unit's flags, to be checked against its neighbours. |
| `hstclone.py` | Propagate an exact source to every function whose code differs only in addresses. |
| `hstfamilies.py` | Rank unmatched clone families by total bytes (one match unlocks a whole family). |
| `hsttmpl.py` | Instantiate known container templates for every element size found in retail. |
| `hstlib.py` | Find newlib functions in retail by compiled-code signature (`index`, `report`). |
| `hstnewlib.py` | Turn `hstlib.py` hits into exact library sources (preprocessed with `-nostdinc`: newlib's headers plus `include/`, no compiler header text); `--regenerate` rebuilds installed library sources and their clones from the release, file and function their notes record. |
| `hstqueue.py` | Build work batches for matching agents: near misses first (ranked by code bytes per word left), then functions an earlier lane never reached, then fresh ones; never a matched or blocked function. Near-miss items carry their lever, and `--lever` offers only near misses with given levers. |
| `hstnearmiss.py` | Re-measure the near misses in `wip/` and give each a word class (which words are wrong) and a lever: the kind of work it needs, decided from its calls, then its control flow, then whether only relocated fields differ (`body`, `structure`, `referent`, `permute`, `types`, `frame`, `expression`), with the `MWCC-DIGEST.md` sections to read. Writes `try/nearmiss-report.json` and `.md`; `hstnearperm.py` seeds only the levers a permuter can fix. |
| `hstattempts.py` | Attempt ledger (`try/attempts.json`): one status per function (`exact`, `near`, `skip`, `untouched`, `blocked`) from the batches, the lanes' progress files, `src/`, `wip/` and `data/blocked_functions.json`; `blocked` validates the blocked list. |
| `hstfamily.py` | Compiler-family predictor (Metrowerks game code versus Sony psp-gcc SDK code) for functions that have no source yet; `evaluate` reports its cross-validated accuracy, `predict` writes `try/family-predictions.json`, which `hstqueue.py --public-first` uses. |
| `hstreview.py` | Local web app (127.0.0.1) for the owner's human review of matched sources. |
| `hstsample.py` | Write the reference-function sample manifest (digests of sources, targets and toolchain). |
| `hstorigin.py` | Classify every source by origin (`game`, `newlib`, `mw-runtime`, `sony-prx`, `sony-middleware-sgx`, `gcc-unattributed`) and write `ORIGINS.json`; prints counts and bytes per class. |
| `hstbootstrap.py` | `--out DIR --eboot FILE --sharing FILE`: check your decrypted executables against `data/modules.json` and write the campaign directory (ledger and input selection) the other tools read. |
| `hstpublish.py` | `--out DIR`: copy the would-be public tree, without the Sony SDK classes and without anything gitignored, and print what was left out. Never touches a git remote. |
| `hstpublic.py` | Maintainers: `sync` mirrors the `hstpublish.py` tree into the public-branch worktree and commits it; `check COMMIT` (run by the pre-push hook) refuses a commit that contains a non-publishable source. |
| `hstprogress.py` | Progress against `docs/COMPLETION.md`: the tier ladder T1-T4 and the strict headline over all six modules' bytes, the public-eligible figures, toolchain-blocked and awaiting-review counts. `--write` refreshes `progress.json` (shields.io endpoint), `docs/progress.svg`, `docs/progress.csv`, `docs/objdiff-report.json` (see `hstreport.py`) and the generated figure blocks (`<!-- hstprogress:begin NAME -->` ... `<!-- hstprogress:end -->`) in `README.md`, `docs/COMPLETION.md`, `docs/PROGRESS.md` and this guide; never edit those by hand. `--check-readme` checks every block. |
| `hstrelease.py` | Maintainers: the whole publish in one command. Dry run by default; `--apply -m MSG` runs push preparation and commits on `full`, refreshes the figures in a clean worktree (with the git-ignored build and port reports copied in), refuses if any published figure would go down, syncs the public worktree and re-matches its new sources; `--push` also pushes and verifies the remote. |
| `hsttu.py` | Recovers the game's translation units (its original source files) into `data/translation-units.json`: a function that loads a file-name string (the Sharing module is a debug build whose assertions name their file), its EBOOT.BIN twin (unique signatures in the same order in both modules), the functions between two of the same unit (link order), and the functions of a C++ class whose other known functions are in one unit (its exclusive virtuals and the constructors and destructors that store its vtable; never across another unit). SDK library functions never join a unit; a unit split by another refuses the write. `--check` compares the committed file. Needs your decrypted modules. |
| `hstreport.py` | Writes `docs/objdiff-report.json`, an objdiff progress report (`report.proto` version 2, JSON form) of the two game modules, from the same figures as the README table; `--check` compares it with the figures measured now. `.github/workflows/progress-report.yml` uploads it as the artifact `UCUS98701_report` for bytematch.dev and recomp.fyi. Units, in this order: the game's own source files recovered by `hsttu.py` (`<module>/<file>.cpp`, all their functions, complete when every one is matched); one unit per published source for its other matched functions (with `source_path`); one `<module>/not-yet-recovered/FUN_<address>` unit per remaining function; and one `<module>/sdk-library-not-published` unit per module that counts the SDK library code without listing it. Every function of `data/functions-<module>.txt` is in exactly one unit, so the units add up to the module figures. It carries what the tree already publishes (addresses, sizes, source paths, header names, the recovered file names), never game bytes or disassembly; data is reported per module, as unmatched. |
| `hstport.py` | `python tools/hstport.py` compiles each EBOOT and Sharing source for Windows x64/x86 and Android arm64/armv7/x64 with a host ABI overlay; writes public/private byte-weighted reports under `try/`. `--targets` narrows iteration, `--parallel` sets compiler concurrency, and `--json` prints a summary. |
| `hstreuse.py` | `check`: every tracked file is covered by exactly one effective `REUSE.toml` annotation; `newlib [--write]`: regenerate the newlib annotations and the table in `third_party/newlib/README.md`. |

Tools that install sources (`hstgen`, `hstclone`, `hsttmpl`, `hstperm`, `hstnewlib`) write into `src/`, and `hstqueue`
prepares batches for automated agents. A tool installs a source only after the matcher reports EXACT. The maintainers
also run external AI agent lanes with orchestration scripts that are kept out of the public tree.

## Origin classes and the publish filter

`python tools/hstorigin.py` gives every source under `src/` and `wip/` one origin class and writes them to
`ORIGINS.json` (path to class); a clone inherits the class of its exemplar.

| Class | Meaning |
|---|---|
| `game` | built with `mwccpsp`, not a runtime helper: the game's own code |
| `newlib` | a newlib library function (`Library source: newlib ...` note), under newlib's own terms |
| `mw-runtime` | built with `mwccpsp`, but the symbol is a Metrowerks runtime name (`__ptmf_scall`, `__destroy_*`, `__construct_*`) |
| `sony-prx` | code of a Sony system-library module (`libfont`, `scePsmf`, `scePsmfP`), kept in `src/libfont`, `src/psmf`, `src/psmfp` and `src/sharing_font` in the maintainers' tree |
| `sony-middleware-sgx` | the SGX sound library, Sony middleware linked into both modules as one block: treated like the Sony SDK |
| `gcc-unattributed` | `psp-gcc`-built code of `EBOOT.BIN` or the Sharing module that is not newlib: treated as Sony SDK until it is attributed |

`python tools/hstorigin.py` prints the current number of sources, source bytes and retail code bytes of each class.

`python tools/hstpublish.py --out DIR` copies the tracked files into a would-be public tree in `DIR` (a new or empty
directory), leaving out the `sony-prx`, `sony-middleware-sgx` and `gcc-unattributed` sources, the maintainer-only paths (below), anything git
tracks although `.gitignore` says it is ignored, and the names of the left-out sources in the copied `ORIGINS.json`.
It prints what it left out (`--list` names every file) and runs only the local, read-only `git ls-files`: it never
touches a git remote. The classes to leave out are the one setting `EXCLUDED_CLASSES` at the top of
`tools/hstpublish.py`. **Why:** the reconstructions of Sony SDK and system-library code are not ours to release, so
they are excluded from public releases; in a public build those functions come from the user's own copy of the game.
(Sony SDK is excluded at publish time for now; the symbol maps in `symbols/`, the function lists in `data/` and the
boundary lists in `ghidra/` are not filtered.)

`PRIVATE_PATHS`, next to `EXCLUDED_CLASSES`, lists the maintainer-only files that are never published: working research notes,
and the scripts that run external agent lanes.

## The private campaign pipeline

The exact matches here are also recorded in a private *campaign*: a hash-bound ledger that pins the toolchain
(wibo, `mwccpsp`, the PSPDEV binutils and the Sony `psp-gcc` kits), records each module and reviewed function
boundary, and rebuilds every source in `src/` with a pinned driver (`toolchain/drivers/` holds the first one). The
campaign's scripts, toolkit and handoff notes depend on workstation paths and private evidence, so they are kept
outside this repository. The tools in `tools/` need only the two campaign files named in `hst-config.json`, which `tools/hstbootstrap.py`
writes from your own decrypted executables.

## Licensing

The repository is licensed by path, following the [REUSE](https://reuse.software/) specification: `REUSE.toml` maps
path patterns to an SPDX licence identifier and a copyright statement, and the licence texts are in
[`LICENSES/`](../LICENSES/). `python tools/hstreuse.py check` verifies that every tracked file is covered by exactly one
effective annotation.

| Paths | Licence |
|---|---|
| `src/` (except the newlib-derived sources below), `wip/`, `symbols/`: the reconstructed code | **GPL-3.0-or-later** ([`LICENSE`](../LICENSE)) |
| `tools/`, `toolchain/`, `ghidra/scripts/`, `ghidra/run_commit_decompile.ps1`, `ghidra/boundaries-*.txt`, `include/`, `docs/`, `README.md`, `CONTRIBUTING.md`, `THIRD_PARTY.md`, `hst-config.example.json`, `.gitignore`, `.gitattributes`, `REUSE.toml`, `ORIGINS.json` | **MIT** |
| The newlib-derived sources in `src/` (exactly the files `tools/hstorigin.py` classifies as `newlib`) and `third_party/` | **their own terms** (BSD-style and similar notices kept in the files) |

- The GPL grant covers **only the project's own contributions**. It grants no rights in the original work of Sony
  Computer Entertainment, Clap Hanz or anyone else in *Hot Shots Tennis: Get a Grip*. The sources reconstruct a
  copyrighted program; they are provided for research, preservation and interoperability, and you need your own
  legally obtained copy of the game.
- Third-party code is under its own terms: the newlib-derived sources keep newlib's notices (each file's licence is
  recorded in `REUSE.toml`, the texts are `LicenseRef-Newlib-*` in `LICENSES/`), and the tools that are needed but not
  included have their vendors' licences. See [THIRD_PARTY.md](../THIRD_PARTY.md) and [`REUSE.toml`](../REUSE.toml).
  `wip/ieee754_powf_00008544.c` reconstructs newlib's `ef_pow.c` and carries Sun's fdlibm notice as well as the GPL.
- Sony SDK and system-library reconstructions are kept out of public releases (see
  [Origin classes and the publish filter](#origin-classes-and-the-publish-filter)).
- This is not legal advice.
