/* HSTMATCH module=hstp_trans_main.prx start=0x00048d44 end=0x00048e38 symbol=FUN_00048d44 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00048d18();
extern const char DAT_00375e94[], DAT_00375ec0[], DAT_00375ecc[], DAT_00375ed8[], DAT_00048c58[];
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_00048d18(DAT_00375e94, txt, DAT_00375ecc, line); for (;;) {} } } while (0)
extern int FUN_00048e38(int a);
extern void FUN_000143b0(char *buf, const char *a, const char *b, int c);
extern int FUN_002c76c0(char *buf, const char *a, int b, int c, int d, int e);
extern void FUN_002c7678(int h, int a, int *p);
extern int FUN_002c76c8(int h, int a, int b);
extern int DAT_00375e90, DAT_00375e8c, DAT_003ba484;
extern char DAT_00375e88, DAT_003ba480;
int FUN_00048d44(int a, int b)
{
    char buf[64];
    int r;
    if (FUN_00048e38(0)) {
        FUN_000143b0(buf, DAT_00375ed8, DAT_00375ecc, 115);
        r = FUN_002c76c0(buf, DAT_00048c58, 111, 0x10000, 16384, 0);
        if (r > 0) {
            DAT_00375e90 = r;
            DAT_003ba480 = 1;
            DAT_00375e88 = 0;
            DAT_003ba484 = a;
            FUN_002c7678(r, 4, &b);
            r = FUN_002c76c8(DAT_00375e8c, 1, 0);
            HST_ASSERT(r >= 0, DAT_00375ec0, 132);
            return 1;
        }
    }
    return 0;
}
