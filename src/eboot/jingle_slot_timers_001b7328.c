/* HSTMATCH module=EBOOT.BIN start=0x001b7328 end=0x001b7564 symbol=FUN_001b7328 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Self {
  char pad0[0x3c];
  int f3c;
  int f40;
  char pad1[8];
  int f4c;
  int f50;
  int f54;
  int f58;
  char pad2[0x78 - 0x5c];
  unsigned char b78;
  unsigned char b79;
  char pad3[0x7c - 0x7a];
  int f7c;
  int f80;
};

struct Msg {
  int a, b, c, d;
};

struct R {
  int x;
};

extern int FUN_001b1f7c(void *, int);
extern void FUN_001b2bb8(void *, int);
extern void FUN_001b9608(struct R *, void *, int *, struct Msg *);
extern void FUN_001b413c(float, float, void *);
extern void FUN_001b5c44(const char *);
extern void FUN_001b3f1c(void *);
extern void FUN_001b4418(float, float, void *);
extern int FUN_001b2af0(void *, int);
extern char DAT_0034179c;
struct Game { char pad[0x3c]; int mode; };
extern struct Game *DAT_003386b0;
extern char DAT_002e99d8[];

void FUN_001b7328(struct Self *self)
{
  if (self->f4c >= 0) {
    if (FUN_001b1f7c(&DAT_0034179c, self->f4c) == 0) {
      self->f4c = -1;
      if (DAT_003386b0->mode == 2) {
        struct Msg m;
        struct R res;
        int arg;
        FUN_001b2bb8(&DAT_0034179c, 0xbc);
        m.a = 0xb6;
        m.b = 0xf;
        arg = (int)((char *)self + 0x40);
        m.c = 0;
        m.d = 0;
        FUN_001b9608(&res, (char *)self + 0x3c, &arg, &m);
      }
    }
  }
  if (self->f54 >= 0) {
    if (FUN_001b1f7c(&DAT_0034179c, self->f54) == 0) {
      if (DAT_003386b0->mode == 4 && self->f50 < 0) {
        FUN_001b413c(1.0f, 0.5f, &DAT_0034179c);
        self->f54 = -1;
      }
    }
  }
  if (self->f50 >= 0) {
    if (FUN_001b1f7c(&DAT_0034179c, self->f50) == 0) {
      if (DAT_003386b0->mode != 3) {
        FUN_001b5c44(DAT_002e99d8);
        if (DAT_003386b0->mode != 9 && self->b78 != 0) {
          if (self->b79 != 0) {
            FUN_001b3f1c(&DAT_0034179c);
            self->b79 = 0;
          } else {
            FUN_001b413c(1.0f, 0.5f, &DAT_0034179c);
          }
        }
        self->f50 = -1;
      }
    }
  }
  if (self->f58 >= 0) {
    if (FUN_001b1f7c(&DAT_0034179c, self->f58) == 0) {
      self->f58 = -1;
    }
  }
  if (self->f7c > 0) {
    self->f7c--;
    if (self->f7c <= 0) {
      FUN_001b4418(0.5f, 7.0f, &DAT_0034179c);
      self->f7c = 0;
    }
  }
  if (self->f80 > 0) {
    self->f80--;
    if (self->f80 <= 0) {
      self->f50 = FUN_001b2af0(&DAT_0034179c, 0x16);
    }
  }
}
