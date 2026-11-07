/* HSTMATCH module=hstp_trans_main.prx start=0x0023be14 end=0x0023bf04 symbol=FUN_0023be14 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef struct Rec { char pad0[16]; unsigned short id; char pad1[29]; signed char lvl; char pad2[6]; short x; short y; } Rec;
extern Rec *FUN_0019f928(int a);
extern int FUN_0019f9ac(void);
extern Rec *FUN_0019f8d4(int i);
extern int FUN_0023a3c0(int a, int b, int c);
extern void FUN_00239f10(const char *fmt, int a, int b, int c);
extern const char DAT_0039b258[];
int FUN_0023be14(int a)
{
    int ok = 1;
    Rec *ref = a ? FUN_0019f928(a) : 0;
    int i;
    for (i = 0; i < FUN_0019f9ac(); i++) {
        Rec *r = FUN_0019f8d4(i);
        if (ref && r->id == ref->id)
            continue;
        if (r->lvl > 0 && r->lvl < 11 && FUN_0023a3c0(r->x, r->y, 0) < 100) {
            FUN_00239f10(DAT_0039b258, r->id, r->x, r->y);
            ok = 0;
            break;
        }
    }
    return ok;
}
