/* HSTMATCH module=hstp_trans_main.prx start=0x00263ee8 end=0x00263fa0 symbol=FUN_00263ee8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0026396c();
extern const char DAT_003a5740[], DAT_003a5818[], DAT_003a5798[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0026396c(DAT_003a5740, DAT_003a5818, DAT_003a5798, line); for (;;) {} } } while (0)
struct Ent { int a; int b; };
extern struct Ent DAT_002dac80[];
struct Elem { int a; int f4; int c; };
struct Rec { char pad[12]; struct Elem *arr; };
extern struct Rec *FUN_001991d4(int);
int FUN_00263ee8(int k)
{
    struct Rec *r = FUN_001991d4(DAT_002dac80[k].a);
    int x = r->arr[DAT_002dac80[k].b].f4;
    switch (k) {
    case 2:
        HST_ASSERT(0, 296);
        break;
    case 5:
    case 6:
        x -= 1;
        break;
    }
    return x;
}
