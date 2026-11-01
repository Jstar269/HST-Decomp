# Contributing

Contributions are welcome: matched functions, near misses with notes, compiler lessons, tool fixes and documentation.
This guide covers what you need, how a function is matched and checked, and what a pull request must contain.
Contributions are licensed as [Licensing](docs/GUIDE.md#licensing) in the project guide says: GPL-3.0-or-later for
`src/`, `wip/` and `symbols/`, MIT for the tools, documentation and headers. By opening a pull request you confirm that
you wrote the change (or name where it came from, below) and that you may submit it under those licences.

**Where does a fix belong?** If the game misbehaves in the recompiled player, the cause is usually general: a missing
or wrong system-call behaviour, an instruction form, a display-list command or a build step. That work belongs in
[Nakagawa Recomp](https://github.com/Jstar269/nakagawa-recomp), where it helps every game and needs no disc to test.
Work belongs here only when it rebuilds a function's source so that the compiled bytes equal the original, and it
needs your own copy of the game. Names and structure learned here can label Nakagawa's generated code, but no decomp
source is copied into it.

## What you need

- **Your own copy of the game** (Hot Shots Tennis: Get a Grip, UCUS98701, North America, version 1.01) and its two
  executables decrypted on your own machine with a tool of your choice. This repository contains no game data and no
  decryption code, and nothing in it downloads the game. [Setup](docs/GUIDE.md#setup) lists the two files and the
  SHA-256 each decrypted file must have; `tools/hstbootstrap.py` refuses any other.
- **Windows with WSL** (Ubuntu, with `libc6-i386` for the 32-bit compilers) and **Python 3.12 or newer** with no
  third-party packages. The tools run on the Windows side and call the compilers inside WSL.
- **The compilers, which you install yourself** (they are proprietary or third-party and are never in this
  repository; [THIRD_PARTY.md](THIRD_PARTY.md) records each one's terms):
  - `mwccpsp` 3.0.1 build 219, the compiler of the game code, from
    [decomp.me's compiler kits](https://github.com/decompme/compilers), run through
    [wibo](https://github.com/decompals/wibo);
  - the decomp.me `psp-gcc-1.3.1` and `psp-gcc-1.7.1` kits, for the SDK and newlib code (`cc=gcc131-*`,
    `cc=gcc171-*` sources);
  - PSPDEV binutils (`psp-ld`, `psp-objcopy`, `psp-objdump`) from a [pspdev release](https://github.com/pspdev/pspdev/releases);
  - only for the few `cc=gcc402x-*` sources: the open-source GCC 4.0.2 PSP build that
    [lbp-psp-decomp](https://github.com/Jstar269/lbp-psp-decomp) builds from GNU and pspdev sources
    (`toolchain/gcc4/` there).

  [Setup](docs/GUIDE.md#setup) gives the directory layout the tools expect and the `hst-config.json` keys
  (`mwcc_native`, `gcc_home`, `gcc4_home`, `pspdev_bin`, `campaign_root`).

## Setting up

Follow [Setup](docs/GUIDE.md#setup) in the project guide, then check that one function builds:

```text
python tools/hstconfig.py check
python tools/hstmatch.py src/eboot/FUN_00000000.c
```

`hstconfig.py check` lists every key; keys that only maintainer tools use (`newlib_home`, `ghidra_headless`,
`ext_root` and the like) show `UNSET`, and that is fine for matching. Before your first change, run
`python tools/hstmatch.py --all --parallel 8` once: every published source should report `EXACT`, which proves your
setup (about an hour on a fast machine).

## Never allowed

These hold for every file, commit, pull request, issue and comment, here and anywhere else:

- **No game data.** No retail or decrypted modules, no disassembly dumps or listings of the game, no hex dumps, no
  data files, images or audio taken from the disc, no keys. Your own inputs stay in your campaign directory, outside
  the repository (`build/` and `try/` are gitignored for the same reason).
- **No fake matches.** No `asm` that emits instructions (except the inline VFPU rule below), no `.word`/`.byte`
  directives, no included binaries, no `extra=` flags or byte masking in `src/`. A match is the compiler's own output
  from C or C++.
- **Nothing derived from the disc on web tools.** Do not post retail bytes, disassembly or decompiler output of the
  game to decomp.me scratches, pastebins, chat services, AI web services or public issues. Work stays on your
  machine; in issues and pull requests, addresses, file names and tool output (`hstmatch.py`, `hstdiff.py` status
  lines) are fine.
- **No Sony SDK reconstructions** (see the compiler notes below), no compiler or SDK header text, and no code from
  another project without its notice and an entry in `THIRD_PARTY.md`.
- **No private material**: no paths that name your user account, no tokens or keys, no personal data.

## Ground rules

- **A source enters `src/` only when `tools/hstmatch.py` reports `EXACT` for it.** Exact means the compiled and linked
  bytes equal the retail bytes of the function, with no masking. There are no near misses in `src/`.
- Code taken from another project (newlib is the case so far) keeps its copyright and licence notices and is listed in
  `THIRD_PARTY.md`; do not remove a notice and do not paste a compiler's or SDK's header text into a source (the
  project-authored headers in `include/` exist for that). Library sources are produced by `tools/hstnewlib.py`.
- Do not commit game data of any kind: no retail modules, no disassembly dumps of whole modules, no keys, no
  compilers. The ignore rules already exclude binaries and key files; keep it that way.
- No machine code in a source (`tools/hstgate.py`, gate 2 of `docs/COMPLETION.md`). The one exception is inline
  VFPU asm where the original SDK or game code used it (owner decision 2026-10-27): the header says `asm=vfpu`,
  a line starting `Original inline VFPU asm:` gives the reason, and every instruction in the asm is a VFPU
  instruction, with loads and stores written `lv.q C000, 16(%0)` (an explicit base register) or `sv.s S000, %0`
  with an `"=m"`/`"m"` operand for a local or parameter (never a global: its address expands to a lui); the SDK's
  `.set push` / `.set noreorder` / `.set pop` wrapper (it emits nothing) is allowed. Anything else in an `asm`
  statement is refused.
- Do not edit existing lines of `symbols/` (appending is allowed, see below), and do not edit `tools/` as part of a
  matching change.
- The tools that write into `src/` (`hstgen`, `hstclone`, `hsttmpl`, `hstperm`, `hstnewlib`) and the one that builds
  batches for matching agents (`hstqueue`) are run deliberately by whoever runs the campaign, not casually while
  matching a function.
- Keep scratch work out of the tracked tree: use `try/<your-name>/` (gitignored). Build products go to
  `build/<module>/<dir>__<stem>__<digest>`; the digest comes from the source's full path, so two sources with
  the same folder and file name never share build products.
- Do not search recursively (`grep -r`) outside `src/`, `wip/`, `docs/`, `symbols/`, `include/` and your own `try/`
  directory. The campaign directory and `build/` can hold gigabytes.

## Method

1. **Pick a function.** `python tools/hstfamilies.py` ranks unmatched clone families (code identical apart from
   addresses): one match unlocks the whole family, because `hstclone.py` propagates it. For single functions,
   `python tools/hstqueue.py` builds batches, listing the Ghidra pseudocode path and, if one exists, the best
   mechanically generated candidate (`best_generated.source`), which is a good starting point.
2. **Read the code.** `python tools/hstdis.py 0xADDR` prints the retail disassembly of any reviewed function, so look at
   the callees and callers too: they tell you types, argument counts and what a call returns. The Ghidra pseudocode
   is advisory and often wrong in detail (see the compiler notes below).
3. **Write the function.** Start from the generated candidate, or write it by hand. Prefer natural code (structs with
   fields, member functions, loops with `break`) over the decompiler's pointer arithmetic.
4. **Iterate.** `python tools/hstmatch.py <file>` compiles, links at the retail address and compares;
   `python tools/hstdiff.py <file>` then prints your disassembly next to retail's, with `**` marking differing words.
   Always pass explicit file names. `--flags` and a header `extra=` are for experiments and never count as a match.
5. **Budget.** About 40 compiles per function is a reasonable limit. When stuck, move on and come back later; if you
   end within a few words, save a near miss (below).
6. **Install** the source once it is EXACT (below), and re-run the matcher on the installed file.

External references: address-named symbols (`FUN_xxxxxxxx`, `DAT_xxxxxxxx` with exactly 8 hex digits) link at their
address automatically. Every other name must be in `symbols/<module>.txt` (C++ runtime helpers, vtables, mangled
member names). If a match needs a new named symbol, verify its address in the disassembly, check with `grep` that the
name is not already there, and **append** one line:

```text
printf '%s\n' 'name = 0xADDR;   # <your source file>' >> symbols/<module>.txt
```

A function whose constants or jump table live in `.rodata` needs `rodata=0xADDR` in the header, the address that
retail's `lui`/`lo` pair loads. The matcher then also requires the placed bytes to equal retail (`data=`, `sdata=`,
`bss=` and `sbss=` work the same way).

### Same-file static helpers

If retail's code depends on a `static` helper compiled earlier in the same file (for example MWCC keeping `a0` live
across a call it knows preserves `a0`, or a store in the `jal` delay slot), define the helper `static` in front of the
target, in retail address order, and list every function of the file other than the target in the header with its
own retail range: `helpers=FUN_0028fb44@0x0028fb44-0x0028fb70` (comma-separated for several). `start=`, `end=` and
`symbol=` stay the target's. The matcher links each function at its own address and compares every one; a helper that
differs is named in the mismatch, and a defined but unlisted function gives `MISPLACED`/`UNPLACED_FUNCTION`. The
source claims only the target's bytes: a helper is counted through its own one-function source, so install one if it
has none. Say in the note line that the helpers are placed by `helpers=`.

Only when the helper is directly adjacent and the run is short does the older form still apply: one source for the
contiguous run (`start=` the first function, `end=` the last, `symbol=` the first), named
`<name>_<first addr8>_run.cpp`, with "run" in the note line. Prefer `helpers=`.

## Source file format

**File name and place.** `src/eboot/` for `EBOOT.BIN`, `src/sharing_main/` for `hstp_trans_main.prx`; name the file
`<short_descriptive_name>_<addr8>.cpp` (or `.c`), where `<addr8>` is the start address in eight hex digits. Before
installing, check that nobody covered the function already (a duplicate must be removed, keeping the reviewed one):

```text
grep -l "start=0x<addr>" src/*/* wip/*
```

**Line 1: the header.**

```c
/* HSTMATCH module=EBOOT.BIN start=0x0011d894 end=0x0011dad8 symbol=FUN_0011d894 lang=c++ */
```

`module`, `start`, `end` and `symbol` are required (`symbol` must be the name your source defines for the function).
`lang` is `c99` or `c++`. Optional: `cc=<profile>` (absent means MWCC build 219; GCC sources say `cc=gcc171-O2`,
`cc=gcc171-Os` or, for unoptimized SDK code, `cc=gcc171-O0`; `cc=gcc402x-O2`/`-Os`/... is a secondary profile for
functions a later Sony compiler built, used only when no gcc131/gcc171 profile matches), the section placement keys
above, `asm=vfpu` (inline
VFPU asm, see the ground rules; the source then carries a line `/* Original inline VFPU asm: <reason> */` after the
note) and the experimental `extra=`.

**Line 2: the note.** Exactly one provenance note, and it must be true. "Hand-written" means a person wrote it; a
source that started from a generated candidate names the generator tag.

- Written from scratch, or heavily restructured:
  `/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */`
- Derived from a generated candidate with small corrections:
  `/* Ghidra pseudocode (hstgen.py tag <tag>) corrected by hand: <what you changed>; exact match verified; names and types await human review. */`
- The tools write the other kinds themselves: `Mechanical translation of Ghidra pseudocode (hstgen.py tag X)`,
  `Clone of src/... (hstclone.py tag X)`, `... (template instance, hsttmpl.py tag X)` and
  `Library source: newlib <release> <dir>/<file>.c function <name>, ...`.

A one-line pointer comment, such as a `docs/DATA-TABLES.md` reference, may sit between the header and the note;
nothing else may. `python tools/hstlint.py` and `python tools/hstprcheck.py` reject a source without a known note kind
there.

## Source quality

The owner reviews every source at the end, so aim for the code the developers wrote.

- Among exact variants, keep the most natural one: real control flow (loops, early returns, `switch`), structs and
  member access instead of `*(int *)(p + 0x14)` when the layout is clear, `bool` and enums where obvious.
- Avoid matching hacks: duplicated conditions, dummy locals or parameters that only steer register allocation,
  `volatile`, self-assignments, gratuitous casts or `goto`. When one of them is the **only** exact form you found,
  keep it, but name it in the note line (`matching idiom: <what>`); `hstlint.py` counts those.
- Names: use a descriptive name only when the evidence supports it (strings, callers, documented research findings);
  otherwise keep the Ghidra address names. Never invent semantics.
- Be consistent: one declaration per symbol across the tree (`hstlint.py` reports symbols declared with different
  types in different sources).

## Near misses

If a function ends within about six differing words but not exact, save your best attempt to
`wip/<short_name>_<addr8>.cpp` (the same line-1 header, once; a near miss needs no provenance note) with trailing `//`
comments: what differs and what you ruled out. Files
in `wip/` are not built by `--all` and are not verified; they are leads for the next person.

## Compiler notes

The detailed, address-referenced findings are in [docs/MWCC-DIGEST.md](docs/MWCC-DIGEST.md) (by topic) and
[docs/MWCC-NOTES.md](docs/MWCC-NOTES.md) (the append-only log). Read the digest before starting; it saves many compiles.
If you learn something new, append **one dated line** to `MWCC-NOTES.md` that names the function address (append only,
never rewrite the file; `tools/hstprcheck.py` refuses a change to an existing line, and its 1 MiB limit applies to what
you append, not to the whole log):

```text
cat >> docs/MWCC-NOTES.md <<'EOF'
- 2026-10-15 (0xADDR): what you found, with the evidence.
EOF
```

The main points, as a start:

- **Game code** is `mwccpsp` 3.0.1 build 219 with `-c -O4,p -sdatathreshold 0`. Some objects were built with
  strength reduction off; those sources carry `#pragma opt_strength_reduction off` themselves. Other per-file pragmas
  seen: `inline_depth(8)` for deep container inline chains, `opt_unroll_loops off`, `pack(1)` (rarely).
- **Undo Ghidra's artefacts.** It inverts `if`/`else` (try both orders), drops parentheses in float chains
  (`a*b*c` may be `a*(b*c)`), prints a signed `x / 2^k` as a shift sequence, hoists repeated subexpressions into
  temporaries, prints `i <= C` as `i < C+1` and `for` loops with `break` as early returns, and drops trailing call
  arguments of callees it typed with fewer parameters. The registers set before a `jal` reveal the real arguments;
  an integer `0` next to a pointer may be a float.
- **Registers follow declaration order.** MWCC gives saved registers in the order locals are declared, the
  earlier-declared getting the higher `s` register. Colouring is global, so a swap in one place can come from code
  elsewhere. Try permuting the declarations of 3 to 6 locals before restructuring.
- **Types decide instructions.** Flags tested `!= 0` that Ghidra types `char` or `undefined4` are usually
  `unsigned char` (`lbu`); `bool` results are often `int` returns; a global used as a struct base must be a pointer;
  numeric module addresses in the pseudocode are symbols (`&DAT_xxxxxxxx`), never integer constants.
- **Stack frames.** Address-taken locals sit in declaration order from the frame top, and each empty class passed by
  value (allocator and tag objects in the container code) reserves a hidden slot.
- **The `hstp_trans_main.prx` ("Sharing") twins** of `EBOOT.BIN` functions are the same source built as a debug build
  with `ASSERT` calls; the `EBOOT.BIN` build drops them, and code after an always-failing assert disappears there.
- **SDK code is GCC.** Sony's SDK objects (most of `EBOOT.BIN` below `0x88000`, and the Sony PRXs) are `psp-gcc`
  output: `ra` saved at the top of the frame, `t` temporaries, `movn`/`movz`, `j` between blocks. Use
  `cc=gcc171-O2` or `cc=gcc171-Os` and plain C, without MWCC pragmas; the declaration order of locals has no effect.
  Unoptimized SDK objects (`move s8,sp` frame pointer, every local homed in the frame) take `cc=gcc171-O0`; there the
  declaration order of locals does decide their frame slots.
  The SDK C library and math library are newlib: report such a function rather than reconstructing it by hand
  (`hstlib.py` and `hstnewlib.py` match it from the library source). Reconstructions of Sony SDK code are not
  accepted into the public repository.
- **Known limit.** Retail uses the Allegrex `ins rd,$zero,pos,size` instruction for field-clearing masks in part of
  the math library, and neither published Sony `psp-gcc` kit emits it. Record such functions in the notes and move
  on.

## Checks before you finish

```text
python tools/hstmatch.py <every file you added or changed>     # EXACT n/n
python tools/hstlint.py --show header                          # none of your files listed
python tools/hstprcheck.py --base origin/main                  # what CI checks on your pull request
```

`hstprcheck.py` needs no game or compiler: it refuses binary files, game data, never-versioned paths (`build/`,
`try/`, `hst-config.json`), personal paths and tokens, disassembly-like listings, changed lines in `symbols/`, sources
without a valid header or note, and anything the fake-match gate refuses. When a change adds files,
`python tools/hstreuse.py check` confirms that every tracked file is covered by the licence map (`REUSE.toml`); new
library sources need `python tools/hstreuse.py newlib --write` first. `ORIGINS.json` is a snapshot made by
`python tools/hstorigin.py`; refresh it before a release (`hstpublish.py` classifies afresh).

For a change that touches many sources, `python tools/hstmatch.py --all` is the full regression (it shares `build/`
with any concurrent run, so run one at a time).

## Pull requests

Fork the repository, make a branch, and open a pull request against `main`. Keep one topic per pull request: one
function, a family of clones of one function, one compiler lesson, or one tool fix.

A pull request contains:

- the sources, each with its header and a true line-2 note (`Hand-written reconstruction ...` when you wrote it;
  name the generator tag when you started from a generated candidate), and any appended `symbols/` lines;
- in the description: the `python tools/hstmatch.py <files>` output showing `EXACT` for each source, what you
  changed from any starting candidate, and, if you used an AI tool, which one (the source must still be work you
  can explain and stand behind);
- for a near miss in `wip/`: the differing words and what you ruled out, as trailing `//` comments.

What happens next: CI runs the checks above on the pull request (a first-time contributor's run waits for the
maintainer's approval). The maintainer reviews the change and **re-verifies every source with the maintainers' tools
before accepting it**: the source is compiled and compared against the retail bytes again and passes the promotion
gate, whatever the pull request's own output said. This repository is published from the maintainer's own tree, so
an accepted pull request is not merged with the button: the change is applied there, credited to you in the commit
and in the next publish commit's message, and the pull request is closed with a link to that commit.

Not accepted: anything under "Never allowed" above, Sony SDK reconstructions, sources that are not `EXACT`, edits to
existing `symbols/` lines, and tool changes mixed into a matching change.
