# GCC 3.3.3 with an Allegrex patch (parked)

**Status, 2026-11-10: parked.** The owner's priority is now the code the public repository can carry, and the block
this compiler was for (psp-gcc code with `ins rt,$zero` in the Sony SDK units of EBOOT.BIN and the Sharing module) is
Sony SDK library code that `tools/hstpublish.py` keeps private (class `gcc-unattributed`); only about 16 newlib
functions (6.6 kB) of the unmatched psp-gcc code are publishable. No proof uses this compiler; it is not a matching
profile. How far it got: both builds exist (hashes in `PROVENANCE.json`); round 1 measured unpatched FSF 3.3.3
against the Sony kits on all 2,204 proven gcc131/gcc171 sources: 1,120 identical assembly, 1,052 different, 32 failed
to compile (`PROVENANCE.json` "rounds"). The patched build (`patches/10-allegrex.diff`, draft 1) was never measured.
To resume: `sh build.sh psp`, then `python -I toolchain/gcc33/cmp.py --variant psp --jobs 4 --show 6` (round 2),
then the five gcc1 near misses and the ten getters 0x2d498..0x2db68 through a scratch `gcc333x-*` profile.

Owner approval 2026-11-09 (relayed by the coordinator): fetch `gcc-3.3.3.tar.bz2` and its signature from
ftp.gnu.org, verify with `gpgv`, and build a PSP cross compiler with this project's own Allegrex patch. Purpose: a
GCC 3.3 code generator that reproduces the Sony SDK kit's output (`psp-gcc` 3.3.3+allegrex, the gcc131/gcc171
profiles) and adds the `ins rt,$zero,pos,size` bit clear the retail SDK objects use, which the 1.7.1 kit never emits.

Acceptance (coordinator's test): every one of the 2,258 gcc131/gcc171 proofs stays EXACT under the new build, or the
build becomes a separate SDK-only profile adopted only after a full migration with none lost.

## Files

- `PROVENANCE.json`: the source tarball (URL, size, SHA-256), the signature, the signing key, the keyring used; the
  built compilers' hashes once a build is adopted.
- `build.sh`: unpack, patch, configure, `make all-gcc`, install. `sh build.sh stock` builds unpatched FSF 3.3.3 for
  `mipsel-elf` (the measured baseline); `sh build.sh psp` applies `patches/*.diff` and builds
  `mipsallegrexel-psp-elf`.
- `patches/host/`: host-compatibility fixes for building 2004 sources with a current GCC; no effect on generated
  code (`01-obstack-lvalue-casts.diff`: GCC 4+ rejects casts as lvalues in `include/obstack.h`).
- `patches/10-allegrex.diff`: the Allegrex target (GPL, written for this project): `mipsallegrex*` triplets and the
  `psp` alias, `-march=allegrex` (MIPS II), single-float EABI default, `seb`/`seh`, `min`/`max`, `abs` as
  `negu`+`max`, `movz`/`movn`, `-mext` (`ext`/`ins`), `-mmadd` (`madd`/`msub`), and the Sony option names
  (`-m[no-]ext`, `-m[no-]madd`, `-m[no-]ulsw`).

What the patch should contain was measured, not copied: the Sony kit's option table and instruction templates
(`strings` of its `cc1`) and its output on probe sources fix the instruction set and the switches; each measurement
round compares the candidate's assembly with the Sony kit's for every proven gcc131/gcc171 source
(`cmp.py`).

## Host

Ubuntu (WSL), gcc 13, x86_64. Sony's `cc1` is an i386 program, so its `HOST_WIDE_INT` is 32 bits for this target;
GCC 3.3.3 does not force a 64-bit one for MIPS. A 32-bit host build needs the `gcc-multilib` package; until a
round shows a difference that traces to `HOST_WIDE_INT` width, the x86_64 build is used.
