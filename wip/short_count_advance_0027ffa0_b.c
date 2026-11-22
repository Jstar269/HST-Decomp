/* HSTMATCH module=EBOOT.BIN start=0x0027ffa0 end=0x0028007c symbol=FUN_0027ffa0 lang=c99 */
typedef struct {
  int f0;
  short s4;
  short s6;
  short s8;
} Node;

typedef struct {
  Node *node;
  short pos;
  short pad;
  unsigned int count;
  unsigned char flag;
} Obj;

extern unsigned char DAT_002caa90[];

void FUN_0027ffa0(Obj *o, float scale)
{
  short v;
  short old;
  Node *n;
  int d;
  if (o->count >= 21) {
    n = o->node;
    v = o->pos;
    old = n->s4;
    n->s4 = v;
    n->s8 += (short)(v - old);
  }
  else {
    d = (short)((float)DAT_002caa90[o->count % 21] * scale);
    if (o->flag)
      d = (short)-d;
    v = o->pos + (short)d;
    n = o->node;
    old = n->s4;
    n->s4 = v;
    n->s8 += (short)(v - old);
    o->count++;
  }
}

// NEAR, 3 words differ (matcher: 55/55 words; 3 differ; first at +0x14), lane api-hste2-20261121: only the position register of the count>=21 block (retail a2, ours a0:
// `lh a2,4(a0); subu v1,a2,v1; sh a2,4(a1)`). Keys: `int d = (short)(..); if (flag) d = (short)-d; v = pos + (short)d;` gives retail's seh/negu/seh and extra `seh v1,a1`.
// Ruled out for the a0/a2 residue: shared function-level locals in 5 declaration orders, int v/old, reading pos/node in other order, a delta local, `s8 = s8 + ..`, swapping the block order (51 words).
// near miss from lane api-hste2-20261121 (unreviewed)
