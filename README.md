<div align="center">

<picture>
  <source media="(prefers-color-scheme: dark)" srcset="docs/images/brand/hst-decomp-mark-dark.svg">
  <img src="docs/images/brand/hst-decomp-mark-light.svg" width="96" height="96" alt="hst-decomp mark">
</picture>

# hst-decomp

**Matching decompilation of Hot Shots Tennis: Get a Grip for the PSP.**

**[Progress](docs/PROGRESS.md) · [Guide](docs/GUIDE.md) · [Contributing](CONTRIBUTING.md) · [Nakagawa Recomp](https://github.com/Jstar269/nakagawa-recomp) · [LittleBigPlanet decomp](https://github.com/Jstar269/lbp-psp-decomp)**

[![Code matched](https://bytematch.dev/Jstar269/hst-decomp.svg?label=code%20matched&measure=matched_code_percent)](https://bytematch.dev/Jstar269/hst-decomp)
[![Functions matched](https://bytematch.dev/Jstar269/hst-decomp.svg?label=functions&measure=matched_functions)](https://bytematch.dev/Jstar269/hst-decomp)
[![License: GPL-3.0-or-later](https://img.shields.io/badge/license-GPL--3.0--or--later-blue.svg)](#license)
[![REUSE status](https://api.reuse.software/badge/github.com/Jstar269/hst-decomp)](https://api.reuse.software/info/github.com/Jstar269/hst-decomp)
[![Platform: PSP (UCUS98701)](https://img.shields.io/badge/PSP-UCUS98701-555555.svg)](#hst-decomp)

</div>

This project rebuilds the code of *Hot Shots Tennis: Get a Grip* (PSP, UCUS98701), also known as *Everybody's Tennis
Portable*, as C and C++. A function counts as matched only when the original compiler turns its source into exactly
the bytes in the game.

It covers both of the game's program files: the main game (`EBOOT.BIN`) and the Sharing Version
(`hstp_trans_main.prx`), the trial copy that one PSP sends to another over wireless Game Sharing.

The repository holds source only and doesn't build a playable game on its own. To play on a PC, use
[Nakagawa Recomp](https://github.com/Jstar269/nakagawa-recomp) with your own copy of the game. The screenshot below
is from that build. It is the only game imagery in this repository; the logo, artwork and on-screen text belong to
their owners, are shown only to identify the game, and are not covered by this project's licences.

![Title screen of Hot Shots Tennis: Get a Grip running natively in Nakagawa Recomp](docs/media/hst-title-screen.png)

<div align="center"><sub>Hot Shots Tennis: Get a Grip belongs to its publisher. This repository contains no game data: you need your own copy of the game, and nothing here downloads one.</sub></div>

## Progress at a glance

<!-- hstprogress:begin readme-glance -->

| | Progress | What it counts |
|---|---|---|
| **Code matched** (T1) | **38.1%** | 2,198,032 of the 5,763,080 code bytes of the game's functions come from C or C++ that compiles, function by function, to the game's exact bytes. |
| **Functions matched** (T1) | **65.1%** | 12,780 of the 19,634 game functions. |
| **Built from source** (T2) | **30.9%** | The share of the game's program files that the whole build reproduces, byte for byte, from this source. |

Code and functions matched are the figures the trackers ([bytematch.dev](https://bytematch.dev/Jstar269/hst-decomp), [recomp.fyi](https://recomp.fyi)) show. The project's own finish line is stricter: the whole build must also work when moved to another address, and every source must be reviewed by a person. That figure is 0.0%. Measured 2026-11-02 from the maintainer's private ledger of accepted proofs by `tools/hstprogress.py`, which needs that ledger.

<!-- hstprogress:end -->

T1 counts each function matched on its own; T2 counts what the whole executables, rebuilt from this source, reproduce
in place. Decompilation projects usually count a game as done when both reach 100%. Charts and history are in the
[progress report](docs/PROGRESS.md), and [`docs/COMPLETION.md`](docs/COMPLETION.md) defines every tier.

## Contribute, or check the matches yourself

Contributions are welcome: matched functions, near misses with notes, compiler lessons, tool fixes and documentation.
[CONTRIBUTING.md](CONTRIBUTING.md) says what you need and what a pull request contains, and issues labelled
[good first issue](https://github.com/Jstar269/hst-decomp/labels/good%20first%20issue) are small functions to start
with. Every task here needs your own copy of the game. You can also re-check every published match on your own
machine:

1. Bring your own copy of the game. The tools need its two executables in decrypted form; this repository contains
   no decryption code. You can decrypt your own disc on your own machine with a tool of your choice, for example
   Nakagawa Recomp's `nk_decrypt` ([how to run it](https://github.com/Jstar269/nakagawa-recomp/blob/main/docs/SETUP.md#encrypted-executables)).
2. Install the compilers in WSL and set up the campaign directory: [Setup](docs/GUIDE.md#setup).
3. Run `python tools/hstmatch.py --all --parallel 4`.

**Where does a fix belong?** If the game misbehaves in the recompiled player, the cause is usually general: a missing
or wrong system-call behaviour, an instruction form, a display-list command or a build step. That work belongs in
[Nakagawa Recomp](https://github.com/Jstar269/nakagawa-recomp), where it helps every game and needs no disc to test.
Work belongs here only when it rebuilds a function's source so that the compiled bytes equal the original, and it
needs your own copy of the game. Names and structure learned here can label Nakagawa's generated code, but no decomp
source is copied into it.

The compiler lessons are in [`docs/MWCC-NOTES.md`](docs/MWCC-NOTES.md). Never post bytes, disassembly or decompiler
output of the game to web tools such as decomp.me, pastebins or public issues.

## Progress in detail

<!-- hstprogress:begin readme-progress -->

**12,777 exact sources** (12,780 functions) and 1,287 near misses, measured 2026-11-02 with `python tools/hstcov.py`
and counted in code bytes:

| Module | Matched / functions | Matched / code bytes | Share of bytes |
|---|---|---|---|
| `EBOOT.BIN` | 6,523 / 9,804 | 1,146,392 / 2,849,076 | 40.237 % |
| `hstp_trans_main.prx` (Sharing Version) | 6,257 / 9,830 | 1,051,640 / 2,914,004 | 36.089 % |
| **Both** | 12,780 / 19,634 | 2,198,032 / 5,763,080 | 38.140 % |

<!-- hstprogress:end -->

Every source is re-verified with `python tools/hstmatch.py` before it is published. These figures count each function
matched on its own. The stricter tiers (the whole executables rebuilt from source, data included, relocatable and
reviewed) are defined in [`docs/COMPLETION.md`](docs/COMPLETION.md), and [`docs/PROGRESS.md`](docs/PROGRESS.md)
charts them.

## How the sources are made

Tools produce most of the sources. Where the game contains the same code more than once, a tool reuses a function that
already matches; other tools translate decompiler output into C. AI agents work on the functions the tools can't
match, under the maintainer's direction.

Every source passes the same checks. An automatic checker compiles it with the original compiler and accepts it only
if it produces exactly the original bytes, and a second gate rejects fake matches, such as machine code pasted in as
assembly instead of written as C. [What the sources are](docs/GUIDE.md#what-the-sources-are) breaks them down by how
they were made.

## Legal

No game assets, executables, keys, decryption code or compilers are in this repository, except the presentation
screenshot shown above, and nothing in it lets you obtain the game. You install the compilers yourself from
[decomp.me's kits](https://github.com/decompme/compilers) and [wibo](https://github.com/decompals/wibo), under their
vendors' licences ([THIRD_PARTY.md](THIRD_PARTY.md)).

This is an unofficial fan project, not affiliated with or endorsed by Sony Interactive Entertainment or Clap Hanz.
Hot Shots Tennis, Everybody's Tennis, PlayStation and PSP are trademarks of Sony Interactive Entertainment Inc. or its
affiliates, used here only to identify the game and platform.

Rights holders with a concern about anything in this repository can contact the maintainer privately through
[GitHub private reporting](https://github.com/Jstar269/hst-decomp/security/advisories/new) or the contact on the
[maintainer's profile](https://github.com/Jstar269). We aim to respond promptly and will remove material where
appropriate.

## License

Licensed by path ([REUSE](https://reuse.software/)): the reconstructed code (`src/`, `wip/`, `symbols/`) is
**GPL-3.0-or-later**, the tools and docs are **MIT** (except the title-screen image, which belongs to its owners), and
newlib-derived code keeps its own notices. The GPL covers only the project's own work, not the original game. Sony SDK
reconstructions are not published here. Full table: [Licensing](docs/GUIDE.md#licensing).

## Learn more

[Progress report](docs/PROGRESS.md) · [Project guide](docs/GUIDE.md) · [Contributing](CONTRIBUTING.md) ·
[Compiler notes](docs/MWCC-NOTES.md) · [Security](SECURITY.md) · [Code of conduct](CODE_OF_CONDUCT.md) ·
[bytematch.dev](https://bytematch.dev/Jstar269/hst-decomp)

One family of projects by the same maintainer, with the same rules: [Nakagawa Recomp](https://github.com/Jstar269/nakagawa-recomp)
runs PSP games natively on a PC, and the matching decompilations are hst-decomp (this one) and
[lbp-psp-decomp](https://github.com/Jstar269/lbp-psp-decomp) (LittleBigPlanet).
