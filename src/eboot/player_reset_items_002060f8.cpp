/* HSTMATCH module=EBOOT.BIN start=0x002060f8 end=0x00206290 symbol=FUN_002060f8 lang=c++ helpers=FUN_002059ec@0x002059ec-0x00205a18 */
/* Hand-written reconstruction from the disassembly (derived from the near miss wip/near_miss_002060f8.c: Vec clear via inline shrink(n, Alloc()), same-file static log stub FUN_002059ec placed by helpers=); exact match verified; names and types await human review. */
extern "C" {
extern const int DAT_002c93b0[6];
extern const char DAT_002ef6d4[];
extern const char DAT_002ef700[];
extern const char DAT_002ef724[];
extern const char DAT_002ef734[];
extern int FUN_00049094(int);
extern void FUN_00055e48(int, int);
extern void FUN_00092c38(int, int);
static void FUN_002059ec(...)
{
}
extern void FUN_00204f18(void);
extern void *FUN_00011090(void *dest, const void *src, int n);
extern void FUN_00048fc8(void *fn, void *obj);
}
struct Alloc {};
struct Vec {
  int **data;
  unsigned size;
  void shrink(int n, Alloc) { size -= n; }
};
struct PlayerObj {
  int f_0;
  char pad4[8];
  Vec items;
  char pad14[0x54 - 0x14];
  char f_54[24];
  char flag_6c;
  char pad6d[0x74 - 0x6d];
  unsigned char flag_74;
  char f_75;
  char f_76;
  char f_77;
  char f_78;
};

extern "C" void FUN_002060f8(PlayerObj *self, int unused, const void *src, char p4, char p5, char p6, char p7)
{
  self->flag_74 = 0;
  if (FUN_00049094(0) == 0) {
    FUN_002059ec(DAT_002ef6d4);
    FUN_002059ec(DAT_002ef700, DAT_002ef724, 1113);
    FUN_002059ec(DAT_002ef734);
  }
  FUN_00049094(1);
  if (self->flag_74 == 0) {
    int i = 0;
    do {
      int a = self->f_0;
      int val;
      if (i >= 0 && i < 6) val = DAT_002c93b0[i];
      else val = 0;
      FUN_00092c38(a, val);
      i++;
    } while (i < 6);
  }
  unsigned u = 0;
  if (self->items.size != 0) {
    int off = 0;
    do {
      int *item = *(int **)((char *)self->items.data + off);
      if (item != 0) {
        FUN_00055e48((int)item, 1);
        *(int **)((char *)self->items.data + off) = 0;
      }
      u++;
      off += 4;
    } while (u < self->items.size);
  }
  {
    int n = ((Vec *)&self->items)->size;
    ((Vec *)&self->items)->shrink(n, Alloc());
  }
  self->flag_6c = 0;
  FUN_00011090(self->f_54, src, 24);
  self->f_75 = p4;
  self->f_76 = p5;
  self->f_77 = p6;
  self->f_78 = p7;
  FUN_00048fc8((void *)FUN_00204f18, self);
}
