/* HSTMATCH module=hstp_trans_main.prx start=0x001c4fe8 end=0x001c5224 symbol=FUN_001c4fe8 lang=c99 */
/* Clone of src/eboot/jingle_slot_timers_001b7328.c (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
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

extern int FUN_001c0010(void *, int);
extern void FUN_001c0ce8(void *, int);
extern void FUN_001c72f4(struct R *, void *, int *, struct Msg *);
extern void FUN_001c2280(float, float, void *);
extern void FUN_001c328c(const char *);
extern void FUN_001c21a8(void *);
extern void FUN_001c2404(float, float, void *);
extern int FUN_001c0bb0(void *, int);
extern char DAT_003eaad8;
struct Game { char pad[0x3c]; int mode; };
extern struct Game *DAT_003e19f0;
extern char DAT_00391594[];

void FUN_001c4fe8(struct Self *self)
{
  if (self->f4c >= 0) {
    if (FUN_001c0010(&DAT_003eaad8, self->f4c) == 0) {
      self->f4c = -1;
      if (DAT_003e19f0->mode == 2) {
        struct Msg m;
        struct R res;
        int arg;
        FUN_001c0ce8(&DAT_003eaad8, 0xbc);
        m.a = 0xb6;
        m.b = 0xf;
        arg = (int)((char *)self + 0x40);
        m.c = 0;
        m.d = 0;
        FUN_001c72f4(&res, (char *)self + 0x3c, &arg, &m);
      }
    }
  }
  if (self->f54 >= 0) {
    if (FUN_001c0010(&DAT_003eaad8, self->f54) == 0) {
      if (DAT_003e19f0->mode == 4 && self->f50 < 0) {
        FUN_001c2280(1.0f, 0.5f, &DAT_003eaad8);
        self->f54 = -1;
      }
    }
  }
  if (self->f50 >= 0) {
    if (FUN_001c0010(&DAT_003eaad8, self->f50) == 0) {
      if (DAT_003e19f0->mode != 3) {
        FUN_001c328c(DAT_00391594);
        if (DAT_003e19f0->mode != 9 && self->b78 != 0) {
          if (self->b79 != 0) {
            FUN_001c21a8(&DAT_003eaad8);
            self->b79 = 0;
          } else {
            FUN_001c2280(1.0f, 0.5f, &DAT_003eaad8);
          }
        }
        self->f50 = -1;
      }
    }
  }
  if (self->f58 >= 0) {
    if (FUN_001c0010(&DAT_003eaad8, self->f58) == 0) {
      self->f58 = -1;
    }
  }
  if (self->f7c > 0) {
    self->f7c--;
    if (self->f7c <= 0) {
      FUN_001c2404(0.5f, 7.0f, &DAT_003eaad8);
      self->f7c = 0;
    }
  }
  if (self->f80 > 0) {
    self->f80--;
    if (self->f80 <= 0) {
      self->f50 = FUN_001c0bb0(&DAT_003eaad8, 0x16);
    }
  }
}
