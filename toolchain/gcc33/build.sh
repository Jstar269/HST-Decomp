#!/bin/sh
# Build an FSF GCC 3.3.3 cross compiler (cc1 + driver only) for the PSP SDK-code profile under evaluation.
#
#   sh build.sh <variant> [jobs]
#
# variant  stock   : unpatched FSF 3.3.3, --target=mipsel-elf (the measured baseline)
#          psp     : FSF 3.3.3 + patches/*.diff in name order, --target=mipsallegrexel-psp-elf
#
# Inputs: $HOME/hst-gcc33/dl/gcc-3.3.3.tar.bz2, verified with gpgv against the GNU keyring before first use
# (see PROVENANCE.json). Sources are unpacked fresh per variant under $HOME/hst-gcc33/work/<variant>/src; the
# build goes to .../build and the install to $HOME/hst-gcc33/<variant>. The assembler is the Sony 1.3.1 kit's
# (through lbp-decomp's sony-as wrapper, which drops -mno-pdr), the linker pspdev's; only cc1 output (-S) and
# the assembler are used by the matching tools. Host: Ubuntu (WSL) gcc 13, x86_64. Sony's cc1 is an i386
# program (32-bit HOST_WIDE_INT for this target; 3.3.3 sets no need_64bit_hwint for mips); a 32-bit host build
# needs the gcc-multilib package, which is not installed, so differences that trace to HOST_WIDE_INT width are
# recorded as such in the measurement rounds.
set -eu
VARIANT=${1:?variant: stock or psp}
JOBS=${2:-4}
HERE=$(cd "$(dirname "$0")" && pwd)
TOP=$HOME/hst-gcc33
TARBALL=$TOP/dl/gcc-3.3.3.tar.bz2
SHA=9ac6618495b7dd2ff0cd50abd76fe3c51287efcc724340687407f9fc250bdc6a
SONY_AS=$HOME/lbp-gcc4/sony-as
PSP_LD=${PSP_LD:-/usr/local/pspdev/bin/psp-ld}

echo "$SHA  $TARBALL" | sha256sum -c -
gpgv --keyring "$TOP/gnu-keyring.gpg" "$TARBALL.sig" "$TARBALL"

case $VARIANT in
  stock) TARGET=mipsel-elf ;;
  psp)   TARGET=mipsallegrexel-psp-elf ;;
  *) echo "unknown variant $VARIANT" >&2; exit 2 ;;
esac

W=$TOP/work/$VARIANT
PREFIX=$TOP/$VARIANT
rm -rf "$W" "$PREFIX"
mkdir -p "$W/src" "$W/build"
tar xjf "$TARBALL" -C "$W/src"
S=$W/src/gcc-3.3.3

# Host-compatibility fixes (no effect on generated code): patches/host/*.diff.
for p in "$HERE"/patches/host/*.diff; do
  [ -e "$p" ] || continue
  echo "host patch $(basename "$p")"; patch -d "$S" -p1 --no-backup-if-mismatch < "$p"
done
if [ "$VARIANT" = psp ]; then
  for p in "$HERE"/patches/*.diff; do
    [ -e "$p" ] || continue
    echo "patch $(basename "$p")"; patch -d "$S" -p1 --no-backup-if-mismatch < "$p"
  done
fi

HF="-O1 -g0 -std=gnu89 -fcommon -fpermissive -fgnu89-inline -w -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0"
cd "$W/build"
CC=gcc CFLAGS="$HF" "$S/configure" \
  --target=$TARGET --prefix="$PREFIX" --program-prefix=psp- \
  --enable-languages=c --disable-nls --with-newlib --without-headers --disable-shared --disable-threads \
  --disable-multilib --with-as="$SONY_AS" --with-ld="$PSP_LD" > configure.log 2>&1
# Only the compiler proper and the driver: libgcc is not needed (the matching tools compile single objects), and
# the Sony assembler rejects the MIPS16 double-float helpers libgcc would build.
make -j"$JOBS" all-libiberty CFLAGS="$HF" > make.log 2>&1 || { tail -n 40 make.log; exit 1; }
make -C gcc -j"$JOBS" cc1 xgcc specs CFLAGS="$HF" >> make.log 2>&1 || { tail -n 40 make.log; exit 1; }
LIBDIR=$PREFIX/lib/gcc-lib/$TARGET/3.3.3
mkdir -p "$PREFIX/bin" "$LIBDIR"
cp gcc/xgcc "$PREFIX/bin/psp-gcc"
cp gcc/cc1 gcc/specs "$LIBDIR/"
echo "built $VARIANT"
sha256sum "$PREFIX/bin/psp-gcc" "$LIBDIR/cc1"
