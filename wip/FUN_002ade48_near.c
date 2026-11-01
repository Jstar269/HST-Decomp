/* HSTMATCH module=EBOOT.BIN start=0x002ade48 end=0x002ade6c symbol=FUN_002ade48 lang=c99 */
struct S { int f0, f4; };

int FUN_002ade48(struct S *a, struct S *b)
{
    return (unsigned)((a->f4 - b->f4) | (a->f0 - b->f0)) < 1;
}
// improved by lane dsn3 (unreviewed): 11 -> 7 differing words
