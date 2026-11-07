/* HSTMATCH module=hstp_trans_main.prx start=0x000df60c end=0x000df718 symbol=FUN_000df60c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_000df718();
extern const char DAT_0037d738[], DAT_0037d764[], DAT_0037d774[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000df718(DAT_0037d738, DAT_0037d764, DAT_0037d774, line); for (;;) {} } } while (0)
extern unsigned char DAT_003b08c4;
extern unsigned char DAT_003e19f8;
extern char *DAT_003e1e04;
struct Obj;
extern struct Obj *DAT_003e1e84;
extern unsigned char DAT_003e1e88;
extern unsigned char DAT_003eaaf0, DAT_003eaaf1, DAT_003eaaf2;
extern void FUN_0001ec44(struct Obj *);
extern void FUN_000df7a8(struct Obj *);
struct Obj { void *vt; char pad[0x38]; int f3c; int f40; unsigned char f44; unsigned char f45; char pad2[0x6e]; int fb4; char pad3[8]; int fc0; char pad4[8]; int fcc; };
struct Obj *FUN_000df60c(struct Obj *p)
{
    FUN_0001ec44(p);
    p->vt = &DAT_003b08c4;
    p->f3c = 5; p->f40 = 0; p->f44 = 0; p->f45 = 1;
    p->fb4 = 3; p->fc0 = 2; p->fcc = 4;
    DAT_003e1e88 = 0;
    p->f45 = DAT_003eaaf0 == 0;
    p->fb4 = DAT_003eaaf1;
    p->fc0 = DAT_003eaaf2;
    if (DAT_003e19f8 != 0 && *(int *)(DAT_003e1e04 + 0x194) > 2) p->f45 = 0;
    FUN_000df7a8(p);
    HST_ASSERT(DAT_003e1e84 == 0, 84);
    DAT_003e1e84 = p;
    return p;
}
