/* HSTMATCH module=EBOOT.BIN start=0x000629cc end=0x00062a04 symbol=FUN_000629cc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern void FUN_00076454(int self, int arg);
extern void FUN_00062a3c(int self);

void FUN_000629cc(int self)
{
    if (self < 0)
        return;
    FUN_00076454(self, 0);
    FUN_00062a3c(self);
}
