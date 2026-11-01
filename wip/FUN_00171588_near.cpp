/* HSTMATCH module=EBOOT.BIN start=0x00171588 end=0x0017168c symbol=FUN_00171588 lang=c++ */
extern "C" int FUN_0024dd40(void *p, float f);
extern "C" void FUN_000d7d80(void *p);
extern "C" void FUN_002045e8(void *p, int a, int b, int c, int d, int e, int f, int g, int h);
extern "C" void FUN_000d8160(void *p, void *q);
extern "C" void FUN_000d7e30(void *p, int flag);
extern "C" void FUN_0017168c(void *self);
extern char *DAT_0034A77C;

struct Rec { char b[200]; };

struct Self {
  char pad0[0x3e8];
  void *(*fn3e8)(int);
  char pad3ec[0x15c];
  int (*fn548)(int);
  char pad54c[0xc];
  void *(*fn558)();
  char pad55c[0x2c];
  char buf588[0x18];
};

extern "C" void FUN_00171588(Self *self, int flag)
{
  int id = (int)self->fn558();
  int sub;
  if (id == 0x7fff) {
    sub = 0;
    goto merge;
  }
  if (id == 0x7ffe) {
    sub = 0;
  } else {
    sub = self->fn548(id);
  }
merge:
  unsigned short *q = (unsigned short *)((char *)self + 0x59e);
  void *p = self->fn3e8(id);
  int h = FUN_0024dd40(DAT_0034A77C, *(float *)((char *)p + 4));
  Rec tmp;
  FUN_000d7d80(&tmp);
  FUN_002045e8(&tmp, id, q[0], q[1], q[2], q[3], sub, h, 0);
  FUN_000d8160(&tmp, self->buf588);
  if (flag != 0)
    FUN_0017168c(self);
  FUN_000d7e30(&tmp, -1);
}
// NEAR MISS: 55/65 words (10 differ). Reduced from 29 differ.
// Register coloring s0..s5 matches retail perfectly (s0=q, s1=sub, s2=id, s3=flag, s4=self, s5=h).
// What differs:
// Retail emits beql s2, 0x7fff (delay slot: move s1, zero) jumping directly to merge,
// and beq s2, 0x7ffe jumping to an unmerged move s1, zero tail.
// Ours inverts the branches to bnel.
// improved by lane dsn5 (unreviewed): 29 -> 10 differing words
