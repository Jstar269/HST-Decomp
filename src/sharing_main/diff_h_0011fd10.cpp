/* HSTMATCH module=hstp_trans_main.prx start=0x0011fd10 end=0x0011fda0 symbol=FUN_0011fd10 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: a bounds-checked inline accessor at(i) with its own assert gives the two assert blocks (6 < n compiles as !(n < 7)). */
extern "C" void FUN_00110f4c(...);
extern "C" const char DAT_002d0090[], DAT_002d005c[], DAT_002d0050[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00110f4c(DAT_002d0090, DAT_002d005c, DAT_002d0050, line); for (;;) {} } } while (0)
struct H_0011fd10 { char pad[6]; short y; short pad2; short y2; };
struct Arr_0011fd10 {
    H_0011fd10 **data; int n;
    H_0011fd10 *at(int i) const { HST_ASSERT(i < n, 1019); return data[i]; }
};
struct S_0011fd10 { char pad[0x10]; Arr_0011fd10 a; };
extern "C" short FUN_0011fd10(S_0011fd10 *s)
{
    return s->a.at(6)->y2 - s->a.at(0)->y;
}
