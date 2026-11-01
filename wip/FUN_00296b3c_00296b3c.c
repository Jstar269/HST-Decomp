/* HSTMATCH module=EBOOT.BIN start=0x00296b3c end=0x00296b5c symbol=FUN_00296b3c lang=c99 */
#include "hst/types.h"

void FUN_00296b3c(u8 *dst, u8 val, int count)
{
    if (count == 0) return;
    do {
        *dst++ = val;
    } while (--count);
}
// Near miss: 6/8 words match (2 differ).
// Retail emits:
//   addiu a2, a2, -1
//   sb a1, 0(a0)
// Whereas MWCC emits:
//   sb a1, 0(a0)
//   addiu a2, a2, -1
// All instructions, registers, branches, and delay slots match.
// Sibling of FUN_00296b14 (memcpy/memset pair in the same translation unit).
// near miss from lane dsn7 (unreviewed)
