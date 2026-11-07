/* HSTMATCH module=hstp_trans_main.prx start=0x000b6e80 end=0x000b6f00 symbol=FUN_000b6e80 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the bounds check is an inline operator[] with its own assert (the C spelling with an explicit local index misses). */
extern "C" void FUN_00097e3c(...);
extern "C" const char DAT_002d0154[], DAT_002d012c[], DAT_002d011c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00097e3c(DAT_002d0154, DAT_002d012c, DAT_002d011c, line); for (;;) {} } } while (0)
struct E_000b6e80 { char b[556]; };
struct V_000b6e80 {
    E_000b6e80 *data; unsigned n; int pad[4];
    E_000b6e80 &operator[](int i) { HST_ASSERT(i >= 0 && (unsigned)i < n, 236); return data[i]; }
};
extern "C" V_000b6e80 *DAT_003ea848;
struct S_000b6e80 { char pad[344]; int k; int i; };
extern "C" E_000b6e80 *FUN_000b6e80(S_000b6e80 *s)
{
    return &DAT_003ea848[s->k][s->i];
}
