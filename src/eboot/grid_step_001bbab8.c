/* HSTMATCH module=EBOOT.BIN start=0x001bbab8 end=0x001bbc58 symbol=FUN_001bbab8 lang=c99 */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_001bbab8.c); exact match verified; names and types await human review. */
typedef struct Ent { unsigned char st; char pad; } Ent;
typedef struct Sub { char pad[0x34c]; Ent tab[640]; } Sub;
typedef struct Grid { char pad[0x80]; Sub sub; } Grid;
typedef struct Cur { unsigned char kind; char pad1; short idx; float f4; } Cur;

extern Grid *DAT_003415a8;
extern int DAT_0034a77c;
extern int FUN_0024dd40(int, float);
extern unsigned char FUN_0024de88(int, int, int);

static inline Ent *Get(Sub *s, int i)
{
  if (i >= 0 && i < 640) return &s->tab[i];
  return 0;
}

void FUN_001bbab8(Cur *c)
{
  int idx;
  if (DAT_003415a8 != 0 && (unsigned)(c->kind - 4) < 2 && (idx = c->idx, idx != 0) && idx != 50) {
    int h = FUN_0024dd40(DAT_0034a77c, c->f4);
    int a = FUN_0024de88(DAT_0034a77c, 5, h);
    int b = FUN_0024de88(DAT_0034a77c, 16, h);
    int base = (idx / 10) * 10;
    int nxt = base + 1;
    Sub *s = &DAT_003415a8->sub;
    int ok = (unsigned)(Get(s, nxt)->st - 4) < 2;
    int e = (unsigned)(Get(s, base)->st - 4) < 2;
    if (b != 0) {
      if (e && base < idx) {
        c->idx = base;
      } else if (ok && nxt < idx) {
        c->idx = nxt;
      }
    } else {
      if (a != 0 && ok && nxt < idx) c->idx = nxt;
    }
  }
}
