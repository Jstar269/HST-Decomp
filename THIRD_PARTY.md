# Third-party material

This file lists the third-party code that is **in** this repository, the third-party components that are **present in
the retail game** (and so may be reconstructed here), and the third-party tools that are **needed to build and verify
but are not included**. The project's own licences are described in the README (Licensing) and mapped path by path in
`REUSE.toml`; this file records the third-party facts and the terms that apply to them.

## In the repository

### newlib (library function sources)

*newlib* is the C library for embedded systems maintained by Red Hat. Its files carry a mixture of BSD-style and other
permissive licences, stated in the comment at the top of each file and in the headers it includes. Upstream:
<https://sourceware.org/newlib/>. The licence terms and the full list of the sources are in
[`third_party/newlib/`](third_party/newlib/README.md); the licence texts are in `LICENSES/` (`LicenseRef-Newlib-*`).

The SDK part of the game was built from newlib, so 80 sources in `src/` are newlib code: 63 direct sources and 17
address-substituted clones of them (`Clone of src/...`). Of the direct sources, 61 are functions found by
compiled-code signature (`tools/hstlib.py`) and turned into sources by `tools/hstnewlib.py`; the other two are
hand-adapted (`src/eboot/kernel_rem_pio2f_0000a550.c` and `src/sharing_main/kernel_rem_pio2f_0000a568.c`, newlib
`libm/math/kf_rem_pio2.c`, derived from Sun's fdlibm). `tools/hstorigin.py` classifies exactly these files as `newlib`
(plus `wip/ieee754_powf_00008544.c`, a near miss that reconstructs `libm/math/ef_pow.c`).

Each direct and cloned source is the upstream library file run through the preprocessor with **comments kept**
(`-E -C -P -nostdinc`), so the file's own copyright and licence comments and those of the newlib headers it includes
are preserved in the source. The note on line 2 (line 3 for a clone) names the release, the upstream file and the
function.

| Release | Direct sources (`hstnewlib.py`) |
|---|---|
| newlib 1.13.0 | 7 |
| newlib 1.14.0 | 51, plus 17 clones of them |
| newlib 1.15.0 | 3 |

(The release is the closest one whose compiled code matches the retail function, not a claim about the release Sony
used. The tarballs and their SHA-256 are listed in `third_party/newlib/README.md`.)

Licence families that appear in the retained comments, and the identifier of each text in `LICENSES/`:

- The Regents of the University of California: the four-clause BSD form with the advertising clause
  (`LicenseRef-Newlib-UCB-BSD-4-Clause`), the three-clause form (`-UCB-BSD-3-Clause`), and the older Berkeley notice
  "provided that the above copyright notice and this paragraph are duplicated in all such forms"
  (`-UCB-Advertising-Notice`).
- David M. Gay, AT&T 1991, in the floating-point conversion code (`mprec.c`; `LicenseRef-Newlib-Gay-AT-T`).
- Sun Microsystems / SunPro (fdlibm: "Permission to use, copy, modify, and distribute this software is freely
  granted, provided that this notice is preserved"), in the `libm` sources (`LicenseRef-Newlib-Sun-fdlibm`).
- On-Line Applications Research Corporation (OAR, Joel Sherrill) in newlib's system headers
  (`LicenseRef-Newlib-OAR`).
- Martin Birgmeier (rand48 support, in `sys/reent.h`; `LicenseRef-Newlib-Birgmeier`).
- Red Hat Incorporated, the default copyright of newlib for files without a notice of their own
  (`LicenseRef-Newlib-Red-Hat`).

What was checked, against the upstream files the 61 `hstnewlib.py` direct sources come from:

- **The copyright and licence comments are kept.** Comparing every comment of the upstream file that mentions a
  copyright or licence with the installed source, all are present, with one exception family: in the eight sources
  derived from `libc/stdio/vfprintf.c` (`__sprint`, `cvt` and `exponent`, in both modules), the comment "Positional
  argument support ... Redistribution and use ..." is absent. It sits inside `#ifndef _NO_POS_ARGS`, which the
  library build disables, so the preprocessor removed the comment together with the code it governs; no code under it
  remains in those sources.
- 26 of the 78 preprocessed sources (61 direct sources and their 17 clones) come from 14 upstream files that carry **no** copyright or licence comment of
  their own (for example `abort.c`, `bsearch.c`, `memchr.c`, `strcpy.c`); those fall under newlib's default copyright
  in `COPYING.NEWLIB` (Red Hat Incorporated), whose files are in `third_party/newlib/`.
- The Sun Microsystems fdlibm notices are **kept in the files that carry them**: in `atan2f` and `sqrtf` (both
  modules) and in the two `kernel_rem_pio2f` sources. The note line of those two uses the library note format
  ("Library source: newlib libm kf_rem_pio2.c ...") and says "adapted by hand": they were adapted by hand, not run
  through the preprocessor, and the Sun notice is kept in their comment. `wip/ieee754_powf_00008544.c` carries the
  Sun notice in summary form.
- **Hand-adapted sources added since (2026-10-27 onwards).** The counts above describe the preprocessed sources.
  Since then 22 more newlib functions were adapted by hand in `EBOOT.BIN` (C library: `fflush`, `__sfp`, `_fwalk`,
  `_setlocale_r`, `printf`, `srand`, `sprintf`, `sscanf`, `_sscanf_r`, `strtof`, `vsprintf`; math library: `cos`,
  `fabs`, `finite`, `isinf`, `isnan`, `sin`, `__kernel_cosf`, `cosf`, `fabsf`, `sinf`, `tanf`), with 18
  address-substituted clones in the Sharing module; with the two `kernel_rem_pio2f` sources that makes 24
  hand-adapted sources. Their note names the upstream file but not the release,
  and most do not repeat the upstream notice. `tools/hstreuse.py` takes the notice of each from the upstream file
  (`ADAPTED_NOTICES`, derived with `python tools/hstreuse.py adapted` from newlib 1.13.0, 1.14.0 and 1.15.0: the
  licence is the same in all three, and the copyright statements are the union), so `REUSE.toml` gives each of them
  its newlib licence and copyright, and `third_party/newlib/README.md` lists them. Until 2026-11-01 these sources
  were covered only by the generic `src/**` annotation, because the parser could not read their note.

### Compiler headers: GNU CC text is gone

An earlier version of `tools/hstnewlib.py` preprocessed the newlib files with the compiler's default include path, which
inlined the compiler's own freestanding headers (GNU CC's `stddef.h`, `stdarg.h`, `float.h` and `limits.h`) into 74
library sources, comments included. Those headers carry the Free Software Foundation's notice and the GNU General
Public License (version 2 or later) with the GCC header exception.

That text has been removed. `hstnewlib.py` now runs the preprocessor with `-nostdinc`, searches newlib's include
directories and then the project's `include/`, and all 74 sources that existed then were regenerated from their recorded
newlib release, file and function (`hstnewlib.py --regenerate`; the clones with `hstclone.py`'s own address
substitution). Every one of the 80 library sources now in `src/` matches the retail bytes exactly, like every other
source there. Nothing else was removed: apart from that text and the
project headers' own comments, the regenerated sources differ from the old ones only in the position of one
`typedef unsigned int size_t;` line (27 sources) and in the spelling of `ULONG_MAX` and `ULLONG_MAX` in `_strtoul_r`
and `_strtoull_r` (the same values and types as the expressions the GCC header used). No licence notice was
deleted; `grep -l -i "General Public License\|GNU CC\|Free Software Foundation" -r src` returns nothing.

### Project-authored headers

`include/` is project-authored: `stddef.h`, `stdarg.h`, `float.h`, `limits.h` and `stdint.h` are written from the C
standard and the psp-gcc 3.3.3 ABI (type widths, `__builtin_va_list`, IEEE 754 single and double) and each says so in
its first comment. They contain no third-party text. `stddef.h` also implements the partial-include convention
(`__need_size_t` and the like) that newlib's headers use. They are the only headers the MWCC sources see (via
`MWCIncludes`) and they stand in for the compiler's headers when `hstnewlib.py` preprocesses library files. They are
under the project's MIT licence (README, Licensing).

### Reconstructed game and SDK code

The sources in `src/`, the symbol maps in `symbols/` and the function boundary lists in `ghidra/` are derived from
analysis of the game's machine code. For `EBOOT.BIN` and `hstp_trans_main.prx` that is the game's own code plus the SDK
objects linked into it; `libfont.prx`, `scePsmfP_library.prx` and `scePsmf_library.prx` are SDK library modules. No
game or SDK file is included. The licence the project grants for these reconstructions (GPL-3.0-or-later for `src/`,
`wip/` and `symbols/`, README, Licensing) covers only the project's own contributions: it grants no rights in the
original work of Sony or of the game's developer, and the question of rights in the reconstructions themselves is
separate and not settled by this file. `tools/hstorigin.py` classifies every source by origin (`game`, `newlib`,
`mw-runtime`, `sony-prx`, `sony-middleware-sgx`, `gcc-unattributed`) and `tools/hstpublish.py` keeps the Sony SDK classes out of a would-be
public tree.

## Third-party components present in the retail game

Found by scanning the two retail modules that contain library code, `EBOOT.BIN` and `hstp_trans_main.prx` (all bytes,
as read through `hstmatch.module_bytes`), on 2026-10-15, so that every open-source component linked into the game is
known and attributed.

**Copyright and library banners: none.** No string of either module matches: zlib (`inflate `, `deflate `,
`Mark Adler`, `Jean-loup Gailly`, `zlib`, and the inflate error messages), libpng, newlib, Red Hat, Cygnus, Sun
Microsystems, `Regents of the University`, FreeType, Xiph, Ogg or Vorbis, `Copyright`, `(C)`, `GNU`, `MIT`, a
licence or warranty text, the usual compression, image and audio libraries (JPEG, LZO, mad, FLAC, Speex) or a
version banner. The ASCII and UTF-16 strings were searched. The only hits are file names (`ICON0.PNG`, `PIC1.PNG`:
the save-data icon and background, not a PNG library) and Sony SDK identifiers listed below. A statically linked
library can be stripped of its banner, so the absence of a banner is not proof of the absence of the library; the
list below is what the evidence supports.

Components identified:

| Component | Evidence | Reconstructed here |
|---|---|---|
| newlib (libc and libm, closest releases 1.13.0 to 1.15.0) | compiled-code signatures (`tools/hstlib.py`) in both modules; and one source string, "bug in vfprintf: bad base", the error text of newlib's (BSD) `vfprintf.c`, at `EBOOT.BIN` file offsets 0x2bacfc and 0x2baecc (addresses 0x002baca8 and 0x002bae78) and `hstp_trans_main.prx` file offsets 0x2caa88 and 0x2cac58 (addresses 0x002caa28 and 0x002cabf8) | 80 sources in `src/`; most of the library is not matched yet |
| Sony SDK libc glue | strings "libc:%s: should be called from main thread" and similar (Sony's reentrancy wrappers around newlib), `EBOOT.BIN` file offset 0x2b90cc (address 0x002b9078) and `hstp_trans_main.prx` file offset 0x2c8e58 (address 0x002c8df8) | no (closed, Sony) |
| Sony system libraries (closed source) | import library names in both modules: sceAtrac3plus, sceAudio, sceCtrl, sceDisplay, sceGe_user, sceHeap, sceImpose, sceLibFont, sceMpeg, sceNet, sceNetAdhoc, sceNetAdhocMatching, sceNetAdhocctl, sceOpenPSID, scePower, scePsmfPlayer, scePspNpDrm_user, sceRtc, sceSasCore, sceSuspendForUser, sceUmdUser, sceUtility, sceWlanDrv (and sceFont_Library in the Sharing module) | the PRX modules `libfont.prx`, `scePsmfP_library.prx`, `scePsmf_library.prx` (class `sony-prx`) | none published: kept out of the public tree |
| Sony's SGX sound library (middleware) linked into the game modules | its assertions name ten `sgx*.c` files, and its strings name SAS, AT3, AT3+ and PCM settings and "SGX system initialize/terminate"; one block of 592 psp-gcc functions in each module (`SGX_BLOCKS` in `tools/hstorigin.py`), the `sony-middleware-sgx` sources | none published: kept out of the public tree and counted like the Sony SDK (owner decision 2026-11-01) |
| Sony SDK code compiled into the game modules (psp-gcc) | the `gcc-unattributed` sources: GCC-built code that is not newlib, treated as Sony SDK until it is attributed | none published: kept out of the public tree |
| Metrowerks C++ runtime and library templates (mwccpsp) | runtime helpers compiled into the game: three functions in each module are likely runtime code (array destruction, static-initialiser and callback iteration at 0x000006e8, 0x00000f98 and 0x00000788), and container template code (deque, tree and vector operations) is instantiated throughout the game's own sources | the six helper sources `src/{eboot,sharing_main}/{destroy_new_array_000006e8,call_init_funcs_00000f98,iterate_callback_desc_00000788}*.c`, currently classed `game` (the classifier matches runtime functions by name only); template code inside game sources is not separated |

No open-source component other than newlib was found. Newlib's 4-clause BSD files carry the University of
California advertising clause, which the University rescinded in 1999; their notices are kept as found.

## Needed, not included

None of these is redistributed by this repository. Obtain each from its own source and follow its licence.

| Component | Used for | Where it comes from |
|---|---|---|
| Metrowerks `mwccpsp` 3.0.1 build 219 | Compiling the game code (`cc` absent or `mwcc219`) | decomp.me's compiler kits, <https://github.com/decompme/compilers> (proprietary compiler) |
| Sony `psp-gcc` 1.3.1 and 1.7.1 kits | Compiling SDK code (`cc=gcc131-*`, `gcc171-*`) and, as the preprocessor, library sources (without its own headers, `-nostdinc`) | decomp.me's compiler kits (i386 Linux; need `libc6-i386`) |
| wibo 1.2.0 | Running the Windows compiler on Linux (WSL) | <https://github.com/decompals/wibo>; check its licence before use |
| PSPDEV binutils (`psp-ld`, `psp-objcopy`, `psp-objdump`) | Linking at the retail address, extracting `.text`, disassembly | the PSPDEV toolchain (GNU binutils for the Allegrex target) |
| newlib 1.13.0 / 1.14.0 / 1.15.0 source trees | Only for `hstnewlib.py` (reproducing the library sources) | upstream releases, <https://sourceware.org/pub/newlib/> (SHA-256 in `third_party/newlib/README.md`) |
| Ghidra 12.x and a JDK 21 | Only for `ghidra/run_commit_decompile.ps1` and `ghidra/scripts/HstCommitDecompile.java` | <https://ghidra-sre.org/> (Apache-2.0); a JDK distribution of your choice |
| Python 3.12 or newer | All tools; standard library only | <https://www.python.org/> |
| `reuse` (optional) | `reuse lint`, an independent check of the licence map; `tools/hstreuse.py check` is the built-in check | <https://reuse.software/> (Apache-2.0 and others) |

The retail game modules are also not included: the tools read decrypted executables from your own copy of the game,
and this repository contains no decryption code (see the README).
