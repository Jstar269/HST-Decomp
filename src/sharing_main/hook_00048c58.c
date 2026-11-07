/* HSTMATCH module=hstp_trans_main.prx start=0x00048c58 end=0x00048d18 symbol=FUN_00048c58 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00048d18();
extern const char DAT_00375e94[], DAT_00375ec0[], DAT_00375ecc[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00048d18(DAT_00375e94, DAT_00375ec0, DAT_00375ecc, line); for (;;) {} } } while (0)
extern void FUN_0002c554(void *);
extern int FUN_002c76b0(int, int);
extern void FUN_002c7750(int);
extern int DAT_00375e8c;
extern void (*DAT_003ba484)(int);
int FUN_00048c58(void *a, int *p)
{
    int v;
    FUN_0002c554(a);
    v = *p;
    HST_ASSERT(FUN_002c76b0(DAT_00375e8c, 1) >= 0, 95);
    DAT_003ba484(v);
    HST_ASSERT(FUN_002c76b0(DAT_00375e8c, 1) >= 0, 100);
    FUN_002c7750(0);
    return 0;
}
