/* HSTMATCH module=EBOOT.BIN start=0x001f9354 end=0x001f9390 symbol=FUN_001f9354 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct S { char pad[64]; int f40; char pad2[1384 - 68]; unsigned char f568; };

int FUN_001f9354(struct S *s)
{
    int m = s->f40;
    int r = 0;
    switch (m) {
    case 5:
    case 6:
        if (s->f568 != 0) r = 1;
        break;
    }
    return r;
}
