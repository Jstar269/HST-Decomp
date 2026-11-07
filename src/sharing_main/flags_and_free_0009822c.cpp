/* HSTMATCH module=hstp_trans_main.prx start=0x0009822c end=0x000982b0 symbol=FUN_0009822c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the same bounds-checked inline accessor at(i) as 0x11fd10 gives the assert block. */
extern "C" void FUN_00097e3c(...);
extern "C" const char DAT_002d0090[], DAT_002d005c[], DAT_002d0050[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00097e3c(DAT_002d0090, DAT_002d005c, DAT_002d0050, line); for (;;) {} } } while (0)
struct H_0009822c { char pad[14]; unsigned short flags; };
struct Arr_0009822c {
    char pad[16]; H_0009822c **data; int n;
    H_0009822c *at(int i) const { HST_ASSERT(i < n, 1019); return data[i]; }
};
struct G_0009822c { char pad[84]; Arr_0009822c *arr; };
extern "C" G_0009822c *DAT_003ea8ec;
extern "C" int FUN_00100ef4(int h);
extern "C" int DAT_003e1f60;
extern "C" bool FUN_0009822c(void)
{
    return (DAT_003ea8ec->arr->at(0)->flags & 3) != 0 && !FUN_00100ef4(DAT_003e1f60);
}
