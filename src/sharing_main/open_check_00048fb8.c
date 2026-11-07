/* HSTMATCH module=hstp_trans_main.prx start=0x00048fb8 end=0x00049020 symbol=FUN_00048fb8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00048d18();
extern const char DAT_00375e94[], DAT_00375ec0[], DAT_00375ecc[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00048d18(DAT_00375e94, DAT_00375ec0, DAT_00375ecc, line); for (;;) {} } } while (0)
extern int FUN_00049084(void);
extern int FUN_002c76c8(int h, int a, int b);
extern int DAT_00375e84;
void FUN_00048fb8(void)
{
    if (FUN_00049084()) {
        int r = FUN_002c76c8(DAT_00375e84, 1, 0);
        HST_ASSERT(r >= 0, 196);
    }
}
