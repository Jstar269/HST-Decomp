/* HSTMATCH module=hstp_trans_main.prx start=0x0020330c end=0x002033f0 symbol=FUN_0020330c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001fbc18();
extern const char DAT_003963d4[], DAT_00396884[], DAT_00396428[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001fbc18(DAT_003963d4, DAT_00396884, DAT_00396428, line); for (;;) {} } } while (0)
extern int *DAT_003e19f0;
extern int DAT_002d93a4[];
extern void FUN_001c3418(int *a, int b, int c);
void FUN_0020330c(int *self)
{
    int *s2 = (int *)DAT_003e19f0[42];
    int i;
    int *p;
    for (i = 0, p = self; i < 4; i++, p++) {
        int *o = (int *)p[232];
        int x = o ? o[22] : 0;
        if (x) {
            if (s2 && i == self[243]) {
                int idx = self[329];
                HST_ASSERT(idx >= 0 && idx < 6, 777);
                FUN_001c3418(s2, i, DAT_002d93a4[idx]);
            }
        }
    }
}
