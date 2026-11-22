/* HSTMATCH module=EBOOT.BIN start=0x00290da4 end=0x00290f68 symbol=FUN_00290da4 lang=c++ helpers=FUN_0028fb44@0x0028fb44-0x0028fb70 */
/* Hand-written reconstruction from the disassembly (same-file static log stub FUN_0028fb44 defined in front and placed at its own retail address by helpers=); exact match verified; names and types await human review. */
struct Pump {
  char pad[0xd4];
  void *buf[2];
  unsigned size;
  unsigned cur;
};
extern "C" {
extern unsigned char DAT_0034a80c;
extern int DAT_0034a810;
extern int DAT_0034a814;
extern char DAT_002ff830[];
extern char DAT_002ff85c[];
extern char DAT_002ff890[];
extern char DAT_002ff8bc[];
extern void FUN_000114c0(void *, int, unsigned);
extern int FUN_000615f4(int, int, int, void *);
extern int FUN_0006171c(int);
static void FUN_0028fb44(...)
{
}
extern void FUN_002b7958(int);
extern void FUN_002b79b0(int);
extern int FUN_002b7a78();
extern int FUN_002b7ed0(Pump *, void *);
extern int FUN_002b7ee0(Pump *);

static inline bool Flag() { return DAT_0034a80c != 0; }
static inline bool Idle() { return Flag() ^ true; }

int FUN_00290da4(Pump *self)
{
  int r;
  for (;;) {
    if (!Idle()) break;
    r = FUN_002b7ee0(self);
    if (r < 0) {
      FUN_0028fb44(DAT_002ff85c, r);
      break;
    }
    if (r == 2) break;
    r = FUN_002b7ed0(self, self->buf[self->cur]);
    if (r != -0x7f9e9ff4) {
      if (r < 0) {
        FUN_0028fb44(DAT_002ff890, r);
        break;
      }
      if (r == 0) {
        r = FUN_000615f4(0, DAT_0034a810, DAT_0034a814, self->buf[self->cur]);
        if (r < 0) {
          FUN_0028fb44(DAT_002ff8bc, r);
        }
        self->cur = (self->cur + 1) & 1;
      }
    }
    r = FUN_002b7a78();
    if (r < 0) {
      FUN_0028fb44(DAT_002ff830, r);
      break;
    }
  }
  FUN_000114c0(self->buf[self->cur], 0, self->size);
  r = FUN_000615f4(0, 0, 0, self->buf[self->cur]);
  if (r < 0) {
    FUN_0028fb44(DAT_002ff8bc, r);
  }
  self->cur = (self->cur + 1) & 1;
  while (FUN_0006171c(0) != 0) {
    FUN_002b7958(1000);
  }
  FUN_002b79b0(0);
  return 0;
}
}
