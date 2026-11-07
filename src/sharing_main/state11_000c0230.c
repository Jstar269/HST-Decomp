/* HSTMATCH module=hstp_trans_main.prx start=0x000c0230 end=0x000c02d8 symbol=FUN_000c0230 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: MWCC tests the cases of a small switch in reverse source order, so case 4/8 are written before case 10. */
extern void FUN_000bd6f0();
extern const char DAT_0037b7e4[], DAT_0037b810[], DAT_0037b818[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_000bd6f0(DAT_0037b7e4, DAT_0037b810, DAT_0037b818, line); for (;;) {} } } while (0)
struct G { char pad[56]; char blk[1]; };
extern struct G *DAT_003e1e04;
extern unsigned char FUN_000c0fbc(struct G *g, void *blk, int id, int n, int z);
struct S { char pad[16]; unsigned short id; char pad2[40 - 18]; int state; char pad3[380 - 44]; int ok; char pad4[400 - 384]; int a; int b; };
void FUN_000c0230(struct S *s)
{
    s->b = s->a;
    s->state = 11;
    switch (s->state) {
    case 4:
    case 8:
        s->ok = FUN_000c0fbc(DAT_003e1e04, DAT_003e1e04->blk, s->id, 1444, 0);
        break;
    case 10:
        HST_ASSERT(0, 313);
        break;
    }
}
