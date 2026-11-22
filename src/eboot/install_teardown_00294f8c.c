/* HSTMATCH module=EBOOT.BIN start=0x00294f8c end=0x00295068 symbol=FUN_00294f8c lang=c99 helpers=FUN_00294684@0x00294684-0x002946b0 */
/* Hand-written reconstruction from the disassembly (same-file static log stub FUN_00294684 placed by helpers=); exact match verified; names and types await human review. */

struct Obj {
  char pad[0x24];
  int f24;
  char pad2[0x32c - 0x28];
  int f32c;
  unsigned char f330;
};

extern char DAT_002cae44[];
extern char DAT_002cae5c[];
extern char DAT_002cacc8[];
extern char DAT_002caca4[];
extern char DAT_002cac94[];
int FUN_002b79a8(int);
int FUN_002b7768(int, void *);
int FUN_002b7738(int);

static void FUN_00294684(...)
{
}

void FUN_00294f8c(struct Obj *self)
{
  int r;
  char buf[8];

  if (self->f24 >= 0) {
    r = FUN_002b79a8(self->f24);
    if (r != 0) {
      FUN_00294684(DAT_002cae5c, r);
    } else {
      FUN_00294684(DAT_002cae44, self->f24);
    }
    self->f24 = -1;
  }
  if (self->f32c >= 0) {
    if (self->f330 != 0) {
      r = FUN_002b7768(self->f32c, buf);
      if (r < 0) {
        FUN_00294684(DAT_002cacc8, r);
      }
      self->f330 = 0;
    }
    r = FUN_002b7738(self->f32c);
    if (r != 0) {
      FUN_00294684(DAT_002caca4, r);
    }
    self->f32c = -1;
  }
  FUN_00294684(DAT_002cac94);
}
