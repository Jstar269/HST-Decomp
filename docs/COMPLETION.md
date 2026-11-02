# What 100% means

The goal of this project is a source tree from which the game's executables can be rebuilt, byte for byte, with
the original compilers, and then changed, relocated and rebuilt into whatever the owner wants. "100%" is defined here
once, at its strictest, so that no later requirement can lower a published percentage: every gate below is part of
the definition from the start, and the scope never shrinks.

## Scope (the denominator)

- **Modules:** every code module of the disc that the campaign ledger tracks: `EBOOT.BIN`, `hstp_trans_main.prx`
  (Sharing Version) and the four Sony modules the game ships and loads (`libfont.prx`, `scePsmf_library.prx`,
  `scePsmfP_library.prx`, `hstp_trans_sceFont_Library.prx`).
- **Bytes:** every byte of each decrypted module: code, initialised data, read-only data, small data, the size and
  placement of zero-initialised data, relocation tables, module info, and the import and export tables. The
  container encryption of the retail files is not part of the program and is not a target.
- **Nothing is removed from the denominator** because it is hard: a function the available toolchain cannot
  reproduce (`data/blocked_functions.json`) stays counted as not done until a toolchain or an explanation is found.
- Game data files on the disc (textures, models, audio, scripts) are inputs the build reads from the owner's own
  copy; they are not reconstructed here.

## Gates (a byte counts only when all of them hold)

1. **Exact.** The full module build reproduces the byte; a module is complete only when its whole rebuilt image has
   the same SHA-256 as the decrypted retail module.
2. **From source, with the original toolchain.** The byte comes from C or C++ compiled with the recorded profile
   (`mwccpsp` 3.0.1 build 219, or the identified Sony `psp-gcc` kit; for code a later Sony compiler built, the
   secondary `gcc402x` profile adopted by the owner on 2026-10-27, an open-source GCC 4.0.2 build with the pspdev
   Allegrex patch and measured Allegrex tuning whose provenance the toolchain lock records) and recorded per-file
   settings, or from
   hand-written assembly only where the original was assembly. Never: retail bytes included as binary, machine code
   pasted into a source (`asm` statements that emit instructions, `.word`/`.byte` emission), object or byte patching,
   post-link fix-ups, changes to the verifier, skipped comparisons, or experimental flags (`extra=`).
   One exception (owner decision 2026-10-27, the rule lbp-psp-decomp also adopted): inline VFPU assembly where the
   original SDK or game code used it. The PSP compilers emit almost no VFPU instructions, so that code was
   written as inline `asm` and is reconstructed as such. It is allowed only in a source whose header says
   `asm=vfpu`, with a line starting `Original inline VFPU asm:` giving the reason, and only when every
   instruction in the asm is a VFPU instruction with explicit `offset(base)` memory operands or `"m"` operands
   for locals and parameters (plus the SDK's
   `.set push`/`noreorder`/`pop` mode switches, which emit nothing; the owner approved both of these extensions on
   2026-10-27); `hstgate.py`
   reports it as the idiom `vfpu-inline-asm` and refuses anything else. All other code around it is C.
3. **In its place in the whole module.** The source is compiled in its translation unit and linked in link order as
   part of the module build. Today's function-at-a-time matches (`tools/hstmatch.py`) are a necessary step, counted
   in tier 1 below, not the goal.
4. **Data is source too.** Initialised and read-only data are defined in source (or are the pools the compiler
   generates from it), not binary blobs.
5. **Relocatable.** No numeric addresses in source: every reference is symbolic. A shifted build (padding inserted
   ahead of the code and the data, then rebuilt) still loads and runs a documented scenario, which proves the tree
   can be modified.
6. **Reproducible.** A clean checkout plus the owner's own disc builds every module with documented commands; no
   Ghidra project or private cache is needed.
7. **Reviewed.** Every source has passed human review (names and types); no placeholder `FUN_`/`DAT_` names remain
   in reviewed code, and no symbol is declared with conflicting types in different files.

## Tiers (how progress is reported)

The headline figure is **strict**: bytes passing all seven gates over all in-scope bytes. Because the gates and the
scope are fixed, it can only rise. The tiers below explain where the remaining work is; each is a share of the same
denominator.

| Tier | A byte is in this tier when | Measured <!-- hstprogress:begin completion-date -->2026-11-02<!-- hstprogress:end --> (public sources) |
| --- | --- | --- |
| T1 | its function matches exactly in isolation (`hstmatch.py`) | <!-- hstprogress:begin completion-t1 -->23.688% (2,197,260 B<!-- hstprogress:end -->; code only, matched one function at a time; data is not counted in this tier) |
| T2 | it is reproduced by the whole-module build from source (gates 1-4, 6) | <!-- hstprogress:begin completion-t2 -->30.895% (2,865,766 B<!-- hstprogress:end -->; code, data and relocation-table bytes the whole-module build produces from source, so it can exceed T1; retail-derived scaffolding is never counted; current public build reports, both game-module images identical and all generated relocation tables equal) |
| T3 | T2, and the shifted build passes (gate 5) | <!-- hstprogress:begin completion-t3 -->30.869% (2,863,366 B<!-- hstprogress:end -->; shifted static checks and numeric-address scans passed for counted sources; shifted runtime not run) |
| T4 = strict | T3, and the source is reviewed (gate 7) | <!-- hstprogress:begin completion-t4 -->0.000% (0 B)<!-- hstprogress:end --> |

Current figures are drawn in [`progress.svg`](progress.svg) and [`progress-map.svg`](progress-map.svg) and computed by
`python tools/hstprogress.py` (the denominator is every byte of the six decrypted module images).

The public repository excludes the Sony SDK reconstructions (see `README.md`), so it reports its own public-eligible
figures beside these; they are never presented as the strict figure.

## Milestones (capabilities on the way)

The strict figure is the goal; the milestones say what the source tree can already do. Each one is reached only when
its own criterion fully holds; `data/milestones.json` defines them, `tools/hstmilestones.py` measures them, and the
first date each reached 100% is kept in `milestones.json` (a later drop is shown as REGRESSED, never hidden).
[`progress-milestones.svg`](progress-milestones.svg) draws them.
[`PORTING.md`](PORTING.md) records the measured host-source portability milestone.

| # | Milestone | Reached when |
| --- | --- | --- |
| 1 | Every byte classified | every function boundary and region of the six modules is reviewed; no open candidate span |
| 2 | Game code matched | every function of `EBOOT.BIN` and the Sharing Version matches (tier 1) |
| 3 | All code matched | every function of all six modules matches (tier 1) |
| 4 | PSP rebuild | the two executables are rebuilt byte for byte from source; Sony's library modules come from the owner's disc |
| 5 | Moddable PSP rebuild | the same builds still load and run when relocated (tier 3) |
| 6 | Every module rebuilt | all six modules are rebuilt from source; no original binary is needed |
| 7 | Portable source | every source of the two executables compiles for 32- and 64-bit Windows and Android with a host compiler |
| 8 | Platform layer | every PSP system function the game imports has a complete host implementation in Nakagawa Recomp's runtime |
| 9 | Plays on Windows, hybrid | a native Windows build (matched functions from source, the rest from Nakagawa Recomp) completes the documented scenario |
| 10 | Plays on Windows from source | the same, with every function from this source |
| 11 | Plays on Android, hybrid | as 9, on Android |
| 12 | Plays on Android from source | as 10, on Android |
| 13 | Strict 100% | every gate above holds for every byte of the six modules |

Nakagawa Recomp covers what this source cannot yet (owner decision, 2026-10-21): its runtime is the platform layer and
its recompiled code fills unmatched functions in the hybrid builds. What either project learns is shared as verified
facts; game-derived code never moves into Nakagawa Recomp.

Public figures follow the public tree: Sony code kept out of this repository is not counted in the public milestones'
denominators (it stays in the strict scope), and a public PSP rebuild may take that code from the owner's own
executable. The work order puts the files that belong in this repository first.

## What 100% cannot mean

The original source text, comments and most names are not in the executables. A complete reconstruction compiles to
the same bytes and behaves the same; its names, types and structure are the project's best reconstruction, which is
why gate 7 requires human review.
