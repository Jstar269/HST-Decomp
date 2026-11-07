/* HSTMATCH module=hstp_trans_main.prx start=0x00094a98 end=0x00094ba4 symbol=FUN_00094a98 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern void FUN_00094494(...), FUN_0005e634(...), FUN_00093ef0(...);
extern const char DAT_00378b80[], DAT_00378bac[], DAT_00378bd0[];
}
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00094494(DAT_00378b80, DAT_00378bac, DAT_00378bd0, line); for (;;) {} } } while (0)
struct VC_0 { virtual ~VC_0(); };
extern "C" void FUN_00094a98(int p, int n)
{
    HST_ASSERT(n >= 0 && n < 6, 715);
    {
        int t = (n << 2) + p;
        int **sp = (int **)(t + 0x50);
        int *o = *sp;
        if (o != 0) {
            if (o != 0) {
                delete (VC_0 *)o;
                *sp = 0;
            }
            if (n == 0 && *(int **)(p + 0x70) != 0) {
                FUN_0005e634(*(int **)(p + 0x70), 1);
                *(int *)(p + 0x70) = 0;
            }
            if (n == 2) {
                unsigned u = 0;
                int q = p;
                do {
                    u = u + 1;
                    *(int *)(q + 4) = -1;
                    q = q + 4;
                } while (u < 7);
            }
            if (n == 5) *(int *)(p + 0x20) = -1;
            FUN_00093ef0(p);
        }
    }
}
