/* HSTMATCH module=hstp_trans_main.prx start=0x001c946c end=0x001c949c symbol=FUN_001c946c lang=c99 */
/* Clone of src/eboot/set_packed_bytes_001bb6b0.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

void FUN_001c946c(u8 *dst, u8 a1, u8 a2, u8 a3, u8 a4, u8 a5, u8 a6, u8 a7, int a8, u8 a9)
{
    dst[0] = a1;
    dst[1] = a2;
    dst[2] = a3;
    dst[3] = a4;
    dst[9] = a5;
    dst[10] = a6;
    dst[7] = a7;
    dst[8] = (u8)a8;
    dst[4] = a9;
}
