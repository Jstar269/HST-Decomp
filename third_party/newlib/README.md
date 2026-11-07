# newlib

Third-party code in this repository: the sources in `src/` that are newlib library functions (`libc` and `libm`
of *newlib*, the C library the Sony SDK part of *Hot Shots Tennis: Get a Grip* was built from). They keep newlib's
own copyright and licence notices and are **not** under the project's licences (see `../../REUSE.toml`,
`../../THIRD_PARTY.md` and the README, "Licensing").

Upstream: <https://sourceware.org/newlib/>, release tarballs from <https://sourceware.org/pub/newlib/>.

## Releases used

The release recorded in a source's note is the closest one whose compiled code matches the retail function; it is
not a claim about the release Sony used.

| Release | Tarball | SHA-256 (from `PROVENANCE.json` beside the tarballs) |
|---|---|---|
| newlib 1.13.0 | `newlib-1.13.0.tar.gz` | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| newlib 1.14.0 | `newlib-1.14.0.tar.gz` | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| newlib 1.15.0 | `newlib-1.15.0.tar.gz` | `c4496102d38c59d1a47ddd5481af35caa1f65b76e2a94d9607737e17fd9e4465` |

The tarballs are not redistributed here.

## What is in this directory

- `newlib-1.13.0-COPYING.NEWLIB`, `newlib-1.14.0-COPYING.NEWLIB`, `newlib-1.15.0-COPYING.NEWLIB`: newlib's
  `COPYING.NEWLIB` from each tarball, byte for byte (renamed with the release in front, so that tools which skip files
  named `COPYING*` still see them as files covered by `REUSE.toml`). It is newlib's list of the licences in the library
  and the default copyright for files that do not state one.
- No other licence file is relied on. The tarballs hold no per-component licence files for the `libc` and `libm`
  directories used here; the per-file notices are inside the source files and headers, and the sources in `src/`
  keep them. (The tarballs' other top-level licence files, `COPYING`, `COPYING.LIB` and `COPYING.LIBGLOSS`, cover shared build
  infrastructure and the board-support library, which are not used here.)

Differences between the three `COPYING.NEWLIB` files:

- 1.13.0 and 1.14.0 have the same structure and the same entries. The difference is the Free Software Foundation
  postal address in the entries for other targets (59 Temple Place in 1.13.0, 51 Franklin Street in 1.14.0) and the
  years of the default Red Hat copyright (1994 to 2004 in 1.13.0, plus 2005 in 1.14.0). The entries the sources
  rely on (the University of California, David M. Gay at AT&T, Sun Microsystems, the Red Hat default) read the same
  in both.
- 1.15.0 is reorganised: the Red Hat default comes first and is now "Copyright (c) 1994-2006 Red Hat, Inc." under
  the BSD licence, the University of California entry is shortened, the David M. Gay entry gains a second (Lucent
  Technologies 1998-2001) notice, the Sun Microsystems entry is joined onto fewer lines, and an entry for IBM, Sony
  and Toshiba (spu targets) is added. Only three sources come from 1.15.0 (functions of `libc/stdlib/mprec.c`); each
  carries David M. Gay's AT&T notice in the file itself, so none relies on the changed entries.
- `libm/math/kf_rem_pio2.c` and `libm/math/ef_pow.c` are byte-identical in all three tarballs.

## How the sources came from newlib

Each library source is the upstream newlib file run through the C preprocessor with its **comments kept**
(`psp-gcc -E -C -P -nostdinc`, `tools/hstnewlib.py`), so the licence comments of the upstream file and of the newlib
headers it includes are kept in the source. The compiler's own headers (`stddef.h`, `stdarg.h`, `float.h`,
`limits.h`) are replaced by the project-authored headers in `include/`: no GNU CC header text remains in any source.
The note on line 2 (or line 3 for a clone) names the release, the upstream file and the function.

Of the 80 newlib-derived sources in `src/`, 61 are direct, 17 are address-substituted clones of direct ones
(`Clone of src/...`, `tools/hstclone.py`), and 2 are hand-adapted (`kernel_rem_pio2f_*.c`, from `libm/math/kf_rem_pio2.c`,
fdlibm-derived, with the Sun Microsystems notice kept in the file). `wip/ieee754_powf_00008544.c` (a near miss, not
part of `src/`) is a reconstruction of `libm/math/ef_pow.c` and carries the Sun notice in summary form.

## Licences that apply

The texts are in `../../LICENSES/`. Those named `LicenseRef-Newlib-*` are taken from the tarballs: the entries of
`COPYING.NEWLIB` (1.14.0) or, for the two that it does not list, the comment in the upstream header that carries the
notice, with only the comment decoration (and a CVS `$Id` line or a title line) removed.

| Identifier | Notice | Where the text comes from | Sources |
|---|---|---|---|
| `LicenseRef-Newlib-UCB-BSD-4-Clause` | University of California, Berkeley: BSD with the advertising clause | `COPYING.NEWLIB` entry [1c] | 14 sources: the `vfprintf.c`, `qsort.c`, `strtol.c` and `strtoul.c` families |
| `LicenseRef-Newlib-UCB-BSD-3-Clause` | University of California, Berkeley: BSD without the advertising clause | `COPYING.NEWLIB` entry [1d] | 4 sources: `strtok_r.c` |
| `LicenseRef-Newlib-UCB-Advertising-Notice` | University of California, Berkeley: "provided that the above copyright notice and this paragraph are duplicated" with an acknowledgement clause | `COPYING.NEWLIB` entry [1b] | 26 sources: those that include `stdio.h` (which carries it) or have it in their own upstream file |
| `LicenseRef-Newlib-Gay-AT-T` | David M. Gay, AT&T 1991 | `COPYING.NEWLIB` entry (3) | 21 sources: `mprec.c` |
| `LicenseRef-Newlib-Sun-fdlibm` | Sun Microsystems (SunPro), fdlibm | `COPYING.NEWLIB` entry (7) | 6 sources in `src/` (`atan2f`, `sqrtf`, `kernel_rem_pio2f`, in both modules) and `wip/ieee754_powf_00008544.c` |
| `LicenseRef-Newlib-Red-Hat` | Red Hat Incorporated, the default for files without a notice of their own | `COPYING.NEWLIB` entry (28) of 1.14.0 | 26 sources, from the 14 upstream files below |
| `LicenseRef-Newlib-OAR` | On-Line Applications Research Corporation (Joel Sherrill) | upstream header `libc/include/sys/features.h` | 44 sources, through newlib's system headers |
| `LicenseRef-Newlib-Birgmeier` | Martin Birgmeier, rand48 support | upstream header `libc/include/sys/reent.h` | 78 sources (all but the two `kernel_rem_pio2f` ones), through newlib's system headers |
| `LicenseRef-Newlib-COPYING-NEWLIB` | the licence list of newlib (1.14.0) | `COPYING.NEWLIB` | the `newlib-*-COPYING.NEWLIB` files here |

Upstream files without a copyright or licence comment of their own (found by scanning each upstream file for
"copyright", "permission to use" and "redistribution"), which therefore fall under the default Red Hat copyright of
`COPYING.NEWLIB`: `libc/search/bsearch.c`, `libc/stdlib/abort.c`, `libc/stdlib/mbrtowc.c`, `libc/stdlib/mbtowc_r.c`,
`libc/stdlib/wcrtomb.c`, `libc/stdlib/wcsrtombs.c`, `libc/string/memchr.c`, `libc/string/memcmp.c`,
`libc/string/strcat.c`, `libc/string/strchr.c`, `libc/string/strcpy.c`, `libc/string/strncmp.c`,
`libc/string/strrchr.c` (all 1.14.0) and `libc/string/strcasecmp.c` (1.13.0). The Red Hat default of 1.13.0 is
"Copyright (c) 1994, 1997, 2001, 2002, 2003, 2004 Red Hat Incorporated" and of 1.14.0 the same with 2005 added.

The licence of a source file in `REUSE.toml` is the combination (`AND`) of the notices that appear inside it, derived
from the file itself by `python tools/hstreuse.py newlib`; every notice found was compared with the text in
`LICENSES/` (the terms are equal apart from the specific copyright years and holders, which `REUSE.toml` records
per file).

## Every newlib-derived source

Generated by `python tools/hstreuse.py newlib --write`; `python tools/hstreuse.py check` fails when it is stale.

<!-- BEGIN generated source table -->

| Source | Kind | Newlib | Upstream file | Function | Terms (LICENSES/) | Tarball SHA-256 |
|---|---|---|---|---|---|---|
| `src/eboot/FUN_0000a430.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/kf_cos.c` | `__kernel_cosf` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000b060.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_cos.c` | `cos` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000b16c.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_fabs.c` | `fabs` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000b178.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_finite.c` | `finite` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000b388.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_isinf.c` | `isinf` | Sun-fdlibm, Red-Hat | all three |
| `src/eboot/FUN_0000b3bc.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_isnan.c` | `isnan` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000b79c.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_sin.c` | `sin` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000bc34.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_cos.c` | `cosf` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000bcfc.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_fabs.c` | `fabsf` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000bf34.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_sin.c` | `sinf` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000c000.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_tan.c` | `tanf` | Sun-fdlibm | all three |
| `src/eboot/FUN_0000f040.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/fflush.c` | `fflush` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_0000f208.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/findfp.c` | `__sfp` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_0000fd34.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/fwalk.c` | `_fwalk` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_0000fea4.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/locale/locale.c` | `_setlocale_r` | Red-Hat | all three |
| `src/eboot/FUN_00012ad0.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/printf.c` | `printf` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_000133f4.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdlib/rand.c` | `srand` | Red-Hat | all three |
| `src/eboot/FUN_000143b0.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sprintf.c` | `sprintf` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_00014438.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sscanf.c` | `sscanf` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_000144b8.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sscanf.c` | `_sscanf_r` | UCB-Advertising-Notice | all three |
| `src/eboot/FUN_00015e54.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdlib/strtod.c` | `strtof` | Gay-AT-T | all three |
| `src/eboot/FUN_0001aca4.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/vsprintf.c` | `vsprintf` | UCB-Advertising-Notice | all three |
| `src/eboot/_Balloc_00011a80_00011a80.c` | clone | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_Balloc` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/__lo0bits_00011e1c.c` | direct | 1.15.0 | `newlib/libc/stdlib/mprec.c` | `__lo0bits` | Gay-AT-T, OAR, Birgmeier | `c4496102d38c59d1a47ddd5481af35caa1f65b76e2a94d9607737e17fd9e4465` |
| `src/eboot/__mcmp_00012370.c` | direct | 1.13.0 | `newlib/libc/stdlib/mprec.c` | `__mcmp` | Gay-AT-T, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/eboot/__s2b_00011c74.c` | direct | 1.15.0 | `newlib/libc/stdlib/mprec.c` | `__s2b` | Gay-AT-T, OAR, Birgmeier | `c4496102d38c59d1a47ddd5481af35caa1f65b76e2a94d9607737e17fd9e4465` |
| `src/eboot/__sprint_00016be4_00016be4.c` | clone | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `__sprint` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/__sprint_00017cd8.c` | clone | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `__sprint` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/__strtok_r_00015ee8.c` | direct | 1.14.0 | `newlib/libc/string/strtok_r.c` | `__strtok_r` | UCB-BSD-3-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/__ulp_00012554.c` | direct | 1.15.0 | `newlib/libc/stdlib/mprec.c` | `__ulp` | Gay-AT-T, OAR, Birgmeier | `c4496102d38c59d1a47ddd5481af35caa1f65b76e2a94d9607737e17fd9e4465` |
| `src/eboot/_b2d_000125c4_000125c4.c` | clone | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_b2d` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_cleanup_0000f30c_0000f30c.c` | clone | 1.14.0 | `newlib/libc/stdio/findfp.c` | `_cleanup` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_iprintf_r_00012a90_00012a90.c` | clone | 1.14.0 | `newlib/libc/stdio/iprintf.c` | `_iprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_mbrtowc_r_00010e10.c` | clone | 1.14.0 | `newlib/libc/stdlib/mbrtowc.c` | `_mbrtowc_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_mbtowc_r_00010ee4.c` | direct | 1.14.0 | `newlib/libc/stdlib/mbtowc_r.c` | `_mbtowc_r` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_mprec_log10_000128ec.c` | clone | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_mprec_log10` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_multadd_00011b64_00011b64.c` | clone | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_multadd` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_ratio_0001284c_0001284c.c` | clone | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_ratio` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_siprintf_r_00014348_00014348.c` | clone | 1.14.0 | `newlib/libc/stdio/siprintf.c` | `_siprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_vsiprintf_r_0001ad18_0001ad18.c` | clone | 1.14.0 | `newlib/libc/stdio/vsiprintf.c` | `_vsiprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_wcrtomb_r_0001ad68_0001ad68.c` | clone | 1.14.0 | `newlib/libc/stdlib/wcrtomb.c` | `_wcrtomb_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/_wcsrtombs_r_0001ae24_0001ae24.c` | clone | 1.14.0 | `newlib/libc/stdlib/wcsrtombs.c` | `_wcsrtombs_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/abort_0000d518.c` | direct | 1.14.0 | `newlib/libc/stdlib/abort.c` | `abort` | Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/atan2f_0000c6dc.c` | direct | 1.14.0 | `newlib/libm/math/wf_atan2.c` | `atan2f` | Sun-fdlibm, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/bsearch_0000d66c.c` | direct | 1.14.0 | `newlib/libc/search/bsearch.c` | `bsearch` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/cvt_00019860_00019860.c` | clone | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `cvt` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/exponent_00019a28.c` | direct | 1.13.0 | `newlib/libc/stdio/vfprintf.c` | `exponent` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/eboot/fiprintf_0000f3b4.c` | direct | 1.14.0 | `newlib/libc/stdio/fiprintf.c` | `fiprintf` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/kernel_rem_pio2f_0000a550.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/kf_rem_pio2.c` | `__kernel_rem_pio2f` | Sun-fdlibm | all three |
| `src/eboot/lflush_00013d88_00013d88.c` | clone | 1.14.0 | `newlib/libc/stdio/refill.c` | `lflush` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/memchr_00010f24.c` | direct | 1.14.0 | `newlib/libc/string/memchr.c` | `memchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/memcmp_0001100c.c` | direct | 1.14.0 | `newlib/libc/string/memcmp.c` | `memcmp` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/qsort_00012b20.c` | direct | 1.14.0 | `newlib/libc/search/qsort.c` | `qsort` | UCB-BSD-4-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/sqrtf_0000d3e8.c` | direct | 1.14.0 | `newlib/libm/math/wf_sqrt.c` | `sqrtf` | Sun-fdlibm, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/std_0000f13c_0000f13c.c` | clone | 1.14.0 | `newlib/libc/stdio/findfp.c` | `std` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strcat_00014788_00014788.c` | clone | 1.14.0 | `newlib/libc/string/strcat.c` | `strcat` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strchr_00014834.c` | direct | 1.14.0 | `newlib/libc/string/strchr.c` | `strchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strcpy_000149a8.c` | direct | 1.14.0 | `newlib/libc/string/strcpy.c` | `strcpy` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strncmp_00014a58.c` | direct | 1.14.0 | `newlib/libc/string/strncmp.c` | `strncmp` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strrchr_00014cfc_00014cfc.c` | clone | 1.14.0 | `newlib/libc/string/strrchr.c` | `strrchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/eboot/strtok_r_00015f98.c` | direct | 1.14.0 | `newlib/libc/string/strtok_r.c` | `strtok_r` | UCB-BSD-3-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/FUN_0000a448.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/kf_cos.c` | `__kernel_cosf` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000b078.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_cos.c` | `cos` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000b184.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_fabs.c` | `fabs` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000b190.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_finite.c` | `finite` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000b3a0.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_isinf.c` | `isinf` | Sun-fdlibm, Red-Hat | all three |
| `src/sharing_main/FUN_0000b3d4.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/common/s_isnan.c` | `isnan` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000b7b4.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/s_sin.c` | `sin` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000bc4c.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_cos.c` | `cosf` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000bd14.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_fabs.c` | `fabsf` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000bf4c.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_sin.c` | `sinf` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000c018.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/sf_tan.c` | `tanf` | Sun-fdlibm | all three |
| `src/sharing_main/FUN_0000f040_0000f040.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/fflush.c` | `fflush` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_0000f208.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/findfp.c` | `__sfp` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_0000fd34_0000fd34.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/fwalk.c` | `_fwalk` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_0000fea4_0000fea4.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/locale/locale.c` | `_setlocale_r` | Red-Hat | all three |
| `src/sharing_main/FUN_00012ad0_00012ad0.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/printf.c` | `printf` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_000133f4_000133f4.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdlib/rand.c` | `srand` | Red-Hat | all three |
| `src/sharing_main/FUN_000143b0_000143b0.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sprintf.c` | `sprintf` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_00014438_00014438.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sscanf.c` | `sscanf` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_000144b8_000144b8.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/sscanf.c` | `_sscanf_r` | UCB-Advertising-Notice | all three |
| `src/sharing_main/FUN_00015e54_00015e54.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdlib/strtod.c` | `strtof` | Gay-AT-T | all three |
| `src/sharing_main/FUN_0001aca4_0001aca4.c` | clone | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libc/stdio/vsprintf.c` | `vsprintf` | UCB-Advertising-Notice | all three |
| `src/sharing_main/_Balloc_00011a80.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_Balloc` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/__mcmp_00012370.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `__mcmp` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/__sprint_00016be4.c` | direct | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `__sprint` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/__sprint_00017cd8.c` | direct | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `__sprint` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/__strtok_r_00015ee8.c` | direct | 1.14.0 | `newlib/libc/string/strtok_r.c` | `__strtok_r` | UCB-BSD-3-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_b2d_000125c4.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_b2d` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_cleanup_0000f30c.c` | direct | 1.14.0 | `newlib/libc/stdio/findfp.c` | `_cleanup` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_iprintf_r_00012a90.c` | direct | 1.14.0 | `newlib/libc/stdio/iprintf.c` | `_iprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_lo0bits_00011e1c.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_lo0bits` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_mbrtowc_r_00010e10.c` | direct | 1.14.0 | `newlib/libc/stdlib/mbrtowc.c` | `_mbrtowc_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_mbtowc_r_00010ee4.c` | direct | 1.14.0 | `newlib/libc/stdlib/mbtowc_r.c` | `_mbtowc_r` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_mprec_log10_000128ec.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_mprec_log10` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_multadd_00011b64.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_multadd` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_ratio_0001284c.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_ratio` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_s2b_00011c74.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_s2b` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_siprintf_r_00014348.c` | direct | 1.14.0 | `newlib/libc/stdio/siprintf.c` | `_siprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_strtol_r_00015fb4.c` | direct | 1.13.0 | `newlib/libc/stdlib/strtol.c` | `_strtol_r` | UCB-BSD-4-Clause, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/sharing_main/_strtoll_r_000161c8.c` | direct | 1.13.0 | `newlib/libc/stdlib/strtoll_r.c` | `_strtoll_r` | UCB-BSD-4-Clause, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/sharing_main/_strtoul_r_00016498.c` | direct | 1.13.0 | `newlib/libc/stdlib/strtoul.c` | `_strtoul_r` | UCB-BSD-4-Clause, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/sharing_main/_strtoull_r_00016694.c` | direct | 1.13.0 | `newlib/libc/stdlib/strtoull_r.c` | `_strtoull_r` | UCB-BSD-4-Clause, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/sharing_main/_ulp_00012554.c` | direct | 1.14.0 | `newlib/libc/stdlib/mprec.c` | `_ulp` | Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_vsiprintf_r_0001ad18.c` | direct | 1.14.0 | `newlib/libc/stdio/vsiprintf.c` | `_vsiprintf_r` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_wcrtomb_r_0001ad68.c` | direct | 1.14.0 | `newlib/libc/stdlib/wcrtomb.c` | `_wcrtomb_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/_wcsrtombs_r_0001ae24.c` | direct | 1.14.0 | `newlib/libc/stdlib/wcsrtombs.c` | `_wcsrtombs_r` | UCB-Advertising-Notice, Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/abort_0000d530.c` | direct | 1.14.0 | `newlib/libc/stdlib/abort.c` | `abort` | Red-Hat, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/atan2f_0000c6f4.c` | direct | 1.14.0 | `newlib/libm/math/wf_atan2.c` | `atan2f` | Sun-fdlibm, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/bsearch_0000d684.c` | direct | 1.14.0 | `newlib/libc/search/bsearch.c` | `bsearch` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/cvt_00019860.c` | direct | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `cvt` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/exponent_00019a28.c` | direct | 1.14.0 | `newlib/libc/stdio/vfprintf.c` | `exponent` | UCB-BSD-4-Clause, UCB-Advertising-Notice, Gay-AT-T, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/kernel_rem_pio2f_0000a568.c` | hand-adapted | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/kf_rem_pio2.c` | `__kernel_rem_pio2f` | Sun-fdlibm | all three |
| `src/sharing_main/lflush_00013d88.c` | direct | 1.14.0 | `newlib/libc/stdio/refill.c` | `lflush` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/memchr_00010f24.c` | direct | 1.14.0 | `newlib/libc/string/memchr.c` | `memchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/memcmp_0001100c.c` | direct | 1.14.0 | `newlib/libc/string/memcmp.c` | `memcmp` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/qsort_00012b20.c` | direct | 1.14.0 | `newlib/libc/search/qsort.c` | `qsort` | UCB-BSD-4-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/sqrtf_0000d400.c` | direct | 1.14.0 | `newlib/libm/math/wf_sqrt.c` | `sqrtf` | Sun-fdlibm, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/std_0000f13c.c` | direct | 1.14.0 | `newlib/libc/stdio/findfp.c` | `std` | UCB-Advertising-Notice, OAR, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strcasecmp_000146ec.c` | direct | 1.13.0 | `newlib/libc/string/strcasecmp.c` | `strcasecmp` | Red-Hat, Birgmeier | `c8562ef73ced13940829faa5f6a8ce4fd1d44e45388b3043ce58baedf03830d9` |
| `src/sharing_main/strcat_00014788.c` | direct | 1.14.0 | `newlib/libc/string/strcat.c` | `strcat` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strchr_00014834.c` | direct | 1.14.0 | `newlib/libc/string/strchr.c` | `strchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strcpy_000149a8.c` | direct | 1.14.0 | `newlib/libc/string/strcpy.c` | `strcpy` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strncmp_00014a58.c` | direct | 1.14.0 | `newlib/libc/string/strncmp.c` | `strncmp` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strrchr_00014cfc.c` | direct | 1.14.0 | `newlib/libc/string/strrchr.c` | `strrchr` | Red-Hat, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `src/sharing_main/strtok_r_00015f98.c` | direct | 1.14.0 | `newlib/libc/string/strtok_r.c` | `strtok_r` | UCB-BSD-3-Clause, Birgmeier | `7db55671132c2943dbbdadfaa954915c23dbc329168575a42bb940ef4bf50a23` |
| `wip/ieee754_powf_00008544.c` | near miss | not recorded (notices of 1.13.0, 1.14.0, 1.15.0) | `newlib/libm/math/ef_pow.c` | `__ieee754_powf` | Sun-fdlibm | all three |

<!-- END generated source table -->
