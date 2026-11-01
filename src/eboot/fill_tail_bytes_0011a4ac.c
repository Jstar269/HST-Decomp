/* HSTMATCH module=EBOOT.BIN start=0x0011a4ac end=0x0011a4e8 symbol=FUN_0011a4ac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Fills eleven consecutive bytes at offset 0x264 with 1. The field is 11 bytes, not the 6 the earlier park
   declared: retail stores 612..616 from a base of `a0 + i` with `i += 5` and `i < 6`, then peels one more
   store at 612(a0+10) into the jr delay, which only covers 0x264..0x26e when the counted range is 11. With
   the correct extent MWCC's own unfolder emits that 5+1 shape from the plain loop, so no hand unrolling. */
struct S { char pad[612]; unsigned char b[11]; };

void FUN_0011a4ac(struct S *s)
{
    int i;
    for (i = 0; i < 11; i++)
        s->b[i] = 1;
}
