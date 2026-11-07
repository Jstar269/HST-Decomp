/* HSTMATCH module=hstp_trans_main.prx start=0x0011fda0 end=0x0011fe30 symbol=FUN_0011fda0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: a bounds-checked inline accessor at(i) with its own assert gives the two assert blocks (2 < n compiles as !(n < 3)). */
extern "C" void FUN_00110f4c(...);
extern "C" const char DAT_002d0090[], DAT_002d005c[], DAT_002d0050[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00110f4c(DAT_002d0090, DAT_002d005c, DAT_002d0050, line); for (;;) {} } } while (0)
struct H_0011fda0 { char pad[4]; short x; short pad2; short x2; };
struct Arr_0011fda0 {
    H_0011fda0 **data; int n;
    H_0011fda0 *at(int i) const { HST_ASSERT(i < n, 1019); return data[i]; }
};
struct S_0011fda0 { char pad[0x10]; Arr_0011fda0 a; };
extern "C" short FUN_0011fda0(S_0011fda0 *s)
{
    return s->a.at(2)->x2 - s->a.at(0)->x;
}
