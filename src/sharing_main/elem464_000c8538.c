/* HSTMATCH module=hstp_trans_main.prx start=0x000c8538 end=0x000c85a8 symbol=FUN_000c8538 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_000c7fa4();
extern const char DAT_0037c478[], DAT_0037c548[], DAT_0037c4b8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000c7fa4(DAT_0037c478, DAT_0037c548, DAT_0037c4b8, line); for (;;) {} } } while (0)
extern int DAT_0037c4d8[];
struct E { char b[464]; };
struct S { char pad[64]; struct E e[1]; };
struct E *FUN_000c8538(struct S *s, int i)
{
    int k = DAT_0037c4d8[i];
    HST_ASSERT(k >= 0, 220);
    return &s->e[k];
}
