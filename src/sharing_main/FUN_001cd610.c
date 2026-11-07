/* HSTMATCH module=hstp_trans_main.prx start=0x001cd610 end=0x001cd708 symbol=FUN_001cd610 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_001c9440();
extern const char DAT_00391910[], DAT_00391c24[], DAT_00391964[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001c9440(DAT_00391910, DAT_00391c24, DAT_00391964, line); for (;;) {} } } while (0)
typedef struct E2 { char a, b; } E2;
typedef struct Ent { int pad; char sub[52]; } Ent;
typedef struct G { char pad[128]; Ent ent[15]; } G;
extern G *DAT_003ea8e8;
extern void FUN_001c98a0(void *p);
void FUN_001cd610(char *self, int idx, int val)
{
    E2 *e;
    if (idx >= 0 && idx < 640)
        e = (E2 *)(idx * 2 + (int)self + 844);
    else
        e = 0;
    if (e != 0) {
        HST_ASSERT(val >= 0 && val < 7, 1926);
        e->a = val;
        if ((unsigned)((unsigned char)val - 4) < 2) {
            if (DAT_003ea8e8 != 0) {
                int i;
                for (i = 0; i < 15; i++) {
                    char *b = (char *)DAT_003ea8e8 + 128;
                    char *q;
                    if (i >= 0 && i < 15)
                        q = (char *)((Ent *)b + i) + 4;
                    else
                        q = 0;
                    FUN_001c98a0(q);
                }
            }
        }
    }
}
