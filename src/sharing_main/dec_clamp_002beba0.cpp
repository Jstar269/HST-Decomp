/* HSTMATCH module=hstp_trans_main.prx start=0x002beba0 end=0x002bec54 symbol=FUN_002beba0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the clamp is an inline taking the bounds by const reference (`v < lo ? lo : (hi < v ? hi : v)`), which reloads each bound for the result as retail does (cf. 0x170c70). */
extern "C" void FUN_002be7c0(...);
extern "C" const char DAT_003abef4[], DAT_003abf58[], DAT_003abf44[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002be7c0(DAT_003abef4, DAT_003abf58, DAT_003abf44, line); for (;;) {} } } while (0)
static inline int Clamp(int v, const int &lo, const int &hi) { return v < lo ? lo : (hi < v ? hi : v); }
struct E_002beba0 { char b[176]; };
struct S_002beba0 { char pad[19316]; int n; E_002beba0 e[1]; };
extern "C" void FUN_002befc0(E_002beba0 *e, int i);
extern "C" int DAT_003abf7c, DAT_003abf80;
extern "C" void FUN_002beba0(S_002beba0 *s, int i)
{
    HST_ASSERT(i >= 0 && i < s->n, 809);
    FUN_002befc0(s->e, i);
    s->n = Clamp(s->n - 1, DAT_003abf7c, DAT_003abf80);
}
