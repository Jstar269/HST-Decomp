/* HSTMATCH module=EBOOT.BIN start=0x0012a254 end=0x0012a5cc symbol=FUN_0012a254 rodata=0x002c14a4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Matching idiom: the previous-mode copy reads the field through a one-int view (ModeHdr) so MWCC reloads it after the early test instead of reusing the compared value. */

struct Obj {
  char pad000[0x58];
  int f58;
  char pad05c[0x190 - 0x5c];
  int mode;
  int prev_mode;
  char pad198[0x250 - 0x198];
  int f250;
  char pad254[0x354 - 0x254];
  int f354;
  int f358;
  void *f35c;
};

extern int DAT_0033b4b8;
extern int DAT_0033b4bc;
extern int DAT_0033b4f0;
extern int DAT_0033b518;
extern int DAT_0033b528;
extern int DAT_0033b550;
extern int DAT_0033b554;
extern int DAT_0033b558;
extern int DAT_0033b55c;
extern int DAT_0033b560;

extern unsigned char FUN_00057b58(float f, int h, int a, int b);
extern void FUN_0012b2b0(struct Obj *self, int a, float f, int b);
extern int FUN_0012b72c(struct Obj *self, void *p, int n);
extern void FUN_0012ad54(struct Obj *self, int addr, int a, float f, int b, int p, int c);
extern void FUN_0012b360(struct Obj *self, int p);
extern void FUN_0012b3d4(struct Obj *self, int p);

struct ModeHdr { int mode; };

void FUN_0012a254(struct Obj *self, int mode, int sub, int force)
{
  if (self->mode != mode || force != 0) {
  self->prev_mode = ((struct ModeHdr *)((char *)self + 0x190))->mode;
  self->mode = mode;
  self->f250 = 0;
  switch (mode) {
  case 0:
    if (force != 0 && self->f58 != 0)
      FUN_00057b58(0.0f, self->f58, 0, -1);
    FUN_0012b2b0(self, 1, -1.0f, sub == 0);
    break;
  case 1:
    FUN_0012ad54(self, DAT_0033b4f0 + (self->f358 * 3 + FUN_0012b72c(self, self->f35c, mode)), 1, -1.0f, 0, 1, 1);
    break;
  case 2:
    FUN_0012ad54(self, DAT_0033b518 + FUN_0012b72c(self, self->f35c, mode), 1, -1.0f, 0, 1, 1);
    break;
  case 3:
    FUN_0012ad54(self, DAT_0033b528 + FUN_0012b72c(self, self->f35c, mode), 1, -1.0f, 0, 1, 1);
    break;
  case 4:
    FUN_0012b360(self, 1);
    break;
  case 5:
    FUN_0012b3d4(self, 1);
    break;
  case 6:
    FUN_0012ad54(self, DAT_0033b550 + sub, sub >= 6, -1.0f, 0, 1, 1);
    break;
  case 7:
    FUN_0012ad54(self, DAT_0033b554 + sub, sub == 4, -1.0f, 0, 1, 1);
    break;
  case 8:
    FUN_0012ad54(self, DAT_0033b558 + sub, sub != 1, -1.0f, 0, 1, 1);
    break;
  case 9:
    FUN_0012ad54(self, DAT_0033b55c + sub, sub >= 4, -1.0f, 0, 1, 1);
    break;
  case 10:
    FUN_0012ad54(self, DAT_0033b560 + sub, sub < 2, -1.0f, 0, 1, 1);
    break;
  case 11:
    FUN_0012b3d4(self, 1);
    break;
  case 12:
    FUN_0012b3d4(self, 1);
    break;
  case 13:
    if (FUN_0012b72c(self, self->f35c, 0) == 3 && self->f354 == 1)
      FUN_0012ad54(self, DAT_0033b4bc, 1, -1.0f, 0, 1, 1);
    else
      FUN_0012ad54(self, DAT_0033b4b8, 1, -1.0f, 0, 1, 1);
    break;
  }
  }
}
