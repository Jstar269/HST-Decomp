/* HSTMATCH module=EBOOT.BIN start=0x00051714 end=0x00051740 symbol=FUN_00051714 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int DAT_0032b244;
extern void FUN_00051740(void *ctx, int val);

void FUN_00051714(void *ctx)
{
    if (DAT_0032b244 != 0)
        FUN_00051740(ctx, DAT_0032b244);
}
