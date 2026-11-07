/* HSTMATCH module=hstp_trans_main.prx start=0x000dca08 end=0x000dca74 symbol=FUN_000dca08 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_000dbf00();
extern const char DAT_0037d37c[], DAT_0037d61c[], DAT_0037d3c0[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000dbf00(DAT_0037d37c, DAT_0037d61c, DAT_0037d3c0, line); for (;;) {} } } while (0)
struct G { int f0; void **data; unsigned n; };
struct S { struct G g[6]; };
unsigned FUN_000dca08(struct S *s, int k)
{
    HST_ASSERT(k >= 0 && k < 6, 408);
    if (k == 4)
        k = 3;
    return s->g[k].n;
}
