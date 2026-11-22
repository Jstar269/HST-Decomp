/* HSTMATCH module=EBOOT.BIN start=0x001b6f68 end=0x001b7100 symbol=FUN_001b6f68 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_001b6f68.c: float arguments 0.0f/0.03f, typed globals); exact match verified; names and types await human review. */
struct A { char pad[0xa4]; int fa4; char pada8[8]; unsigned char fb0; };
struct B { char pad[0x40]; int f40; };
extern "C" {
extern A *DAT_00338b44;
extern B *DAT_003386b0;
extern char DAT_0034179c;
extern void FUN_000623f0(int);
extern int FUN_000def0c(A *);
extern int FUN_000def58(A *);
extern void FUN_001b1f84(void *, int *);
extern void FUN_001b2040(float, void *, int *);
extern void FUN_001b2bb8(void *, int);
extern void FUN_001b413c(void *, float, float);
extern void FUN_001b41c8(void *);
extern void FUN_001b4418(void *, float, float);
extern int FUN_001b4458(void *);
extern void FUN_001b871c(void *);
extern void FUN_001b94f8(void *, void *, void *, void *);
}
struct Self {
  char pad00[0x3c];
  int f3c;
  int f40;
  int f44;
  char pad48[0xc];
  int f54;
  char pad58[0x20];
  unsigned char f78;
  char pad79[7];
  int f80;
};

extern "C" void FUN_001b6f68(Self *self)
{
  if (DAT_00338b44->fb0 != 0 && DAT_003386b0->f40 != 7) {
    if (DAT_00338b44->fa4 != 0 && DAT_00338b44->fa4 != 1) {
      FUN_001b2bb8(&DAT_0034179c, 0x99);
      if (self->f54 >= 0) {
        FUN_001b1f84(&DAT_0034179c, &self->f54);
        self->f54 = -1;
      }
      self->f80 = 3;
    }
    if (FUN_000def0c(DAT_00338b44) != 0) {
      FUN_001b413c(&DAT_0034179c, 0.0f, 0.03f);
      if (self->f78 == 0) {
        FUN_001b4418(&DAT_0034179c, 0.0f, 0.03f);
      }
    }
  }
  if (FUN_001b4458(&DAT_0034179c) == 0x25 && DAT_003386b0->f40 != 7) {
    FUN_001b41c8(&DAT_0034179c);
  }
  if (FUN_000def58(DAT_00338b44) != 0) {
    FUN_000623f0(3);
  }
  FUN_001b871c(self);
  FUN_001b2040(0.5f, &DAT_0034179c, (int *)((char *)self + 100));
  int out;
  int b;
  int *a;
  a = &self->f40;
  b = self->f44;
  FUN_001b94f8(&out, (char *)self + 0x3c, &b, &a);
}
