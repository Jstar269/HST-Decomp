/* HSTMATCH module=EBOOT.BIN start=0x00296b14 end=0x00296b3c symbol=FUN_00296b14 lang=c99 */
#include "hst/types.h"

void FUN_00296b14(u8 *dst, const u8 *src, int count)
{
    if (!count) return;
    do {
        *dst++ = *src++;
    } while (--count);
}
// Near miss: 8/10 words match (2 differ).
// Retail emits:
//   addiu a1, a1, 1
//   sb v1, 0(a0)
// Whereas MWCC emits:
//   sb v1, 0(a0)
//   addiu a1, a1, 1
// All instructions, registers, branches, and delay slots match.
// Tried: comma operator, explicit temporary sequencing, indexed access, restrict keyword, unsigned loop counter.
// near miss from lane dsn7 (unreviewed)
