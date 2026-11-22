/* HSTMATCH module=EBOOT.BIN start=0x0027ffa0 end=0x0028007c symbol=FUN_0027ffa0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Node {
  int f0;
  short s4;
  short s6;
  short s8;
  void SetX(short x) {
    short dx = x - s4;
    s4 = x;
    s8 += dx;
  }
};

struct Obj {
  Node *node;
  short pos;
  short pad;
  unsigned int count;
  unsigned char flag;
};

extern "C" extern unsigned char DAT_002caa90[];

extern "C" void FUN_0027ffa0(Obj *o, float scale)
{
  if (o->count >= 21) {
    o->node->SetX(o->pos);
  } else {
    int d = (short)((float)DAT_002caa90[o->count % 21] * scale);
    if (o->flag != 0)
      d = (short)-d;
    o->node->SetX((short)(o->pos + (short)d));
    o->count++;
  }
}
