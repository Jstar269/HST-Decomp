/* HSTMATCH module=hstp_trans_main.prx start=0x00297820 end=0x00297918 symbol=FUN_00297820 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002918a8();
extern const char DAT_002d0090[], DAT_002d005c[], DAT_002d0050[], DAT_002d10d4[], DAT_002d10d0[], DAT_002d10cc[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002918a8(DAT_002d0090, DAT_002d005c, DAT_002d0050, line); for (;;) {} } } while (0)
typedef struct Hd { char pad[14]; unsigned short flags; } Hd;
typedef struct Info { char pad[16]; Hd **m10; int m14; } Info;
typedef struct G { char pad[84]; Info *m54; char pad2[16]; int m68; } G;
typedef struct Name { char pad[64]; const char *s; } Name;
typedef struct R { char pad[192]; Name *m192; int m196; } R;
typedef struct Q { int pad; void *m4; struct { int pad[2]; int v; } *m8; R *m12; } Q;
extern G *DAT_003ea8ec;
extern Q *FUN_001991d4(int n);
extern void FUN_0011e940(G *g, int a, const char *s, int b, int c);
void FUN_00297820(char *self)
{
    G *g;
    Info *info;
    Q *q;
    const char *s;
    self[197] = 1;
    g = DAT_003ea8ec;
    info = g->m54;
    HST_ASSERT(info->m14 > 0, 1019);
    if (((*info->m10)->flags & 3) == 0 && g->m68 == 6)
        return;
    q = FUN_001991d4(17);
    if (q->m4 == 0)
        s = DAT_002d10d4;
    else if (q->m8->v < 17)
        s = DAT_002d10d0;
    else if (q->m12->m196 < 17)
        s = DAT_002d10cc;
    else
        s = q->m12->m192->s;
    FUN_0011e940(DAT_003ea8ec, 6, s, 0, 0);
}
