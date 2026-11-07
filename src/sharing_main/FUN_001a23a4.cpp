/* HSTMATCH module=hstp_trans_main.prx start=0x001a23a4 end=0x001a248c symbol=FUN_001a23a4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: object built with a C++ constructor whose body runs the base and member init calls; int-returning init tested with (unsigned char) cast */
extern "C" {
extern void FUN_001a2288(...);
extern const char DAT_00388c84[], DAT_00388cdc[], DAT_00388cb8[], DAT_00388cf0[], DAT_00388d14[], DAT_00388d18[];
extern void FUN_00065884(void *p);
extern void FUN_0008d97c(void *p);
extern char DAT_003b0388;
}
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_001a2288(DAT_00388c84, txt, DAT_00388cb8, line); for (;;) {} } } while (0)
struct Sub { char d[16]; };
struct Mgr {
    void *vt; char pad[12]; Sub a; Sub b;
    Mgr() { FUN_00065884(this); vt = &DAT_003b0388; FUN_0008d97c(&a); FUN_0008d97c(&b); }
    unsigned char init(const char *s, int x, int y, const char *t);
};
extern "C" int FUN_00066260(Mgr *p, const char *a, int b, int c, const char *d);
extern Mgr *DAT_003eaa8c;
extern "C" void FUN_001a23a4(void)
{
    if (DAT_003eaa8c == 0) {
        Mgr *p = new Mgr;
        DAT_003eaa8c = p;
        HST_ASSERT(p != 0, DAT_00388cdc, 2269);
        HST_ASSERT((unsigned char)FUN_00066260(p, DAT_00388cf0, 0, 0, DAT_00388d14), DAT_00388d18, 2271);
    }
}
