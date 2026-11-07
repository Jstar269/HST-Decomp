/* HSTMATCH module=EBOOT.BIN start=0x0001bfc0 end=0x0001c008 symbol=FUN_0001bfc0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void *FUN_00000bcc(unsigned n);
extern void *FUN_0001b584(void *p, int b);
extern void *DAT_0030ebcc;
void FUN_0001bfc0(void *self, int b)
{
    void *p = FUN_00000bcc(8);
    if (p)
        p = FUN_0001b584(p, b);
    DAT_0030ebcc = p;
}
