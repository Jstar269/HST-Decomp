/* HSTMATCH module=hstp_trans_main.prx start=0x001f5f40 end=0x001f5fb8 symbol=FUN_001f5f40 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001eb3b8();
extern const char DAT_003938fc[], DAT_003940c8[], DAT_00393934[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001eb3b8(DAT_003938fc, DAT_003940c8, DAT_00393934, line); for (;;) {} } } while (0)
extern void FUN_001f08e8(void *s);
extern void FUN_00108f54(void *p, int a);
extern void FUN_001fc5c8(void);
extern void FUN_001f5fb8(void *s, int a, int b);
struct S { char pad[9656]; void *p; };
void FUN_001f5f40(struct S *s)
{
    FUN_001f08e8(s);
    HST_ASSERT(s->p != 0, 5691);
    FUN_00108f54(s->p, 0);
    FUN_001fc5c8();
    FUN_001f5fb8(s, 0, 0);
}
