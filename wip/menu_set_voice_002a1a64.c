/* HSTMATCH module=EBOOT.BIN start=0x002a1a64 end=0x002a1d7c symbol=FUN_002a1a64 lang=c99 rodata=0x002cbfe0 */
typedef unsigned char uchar;
typedef struct Menu {
  char pad0[0x1b0];
  int f1b0;
  char pad1[0x1e0 - 0x1b4];
  int f1e0;
  uchar f1e4;
  uchar f1e5;
  char pad2[2];
  int f1e8;
  char pad3[0x1f8 - 0x1ec];
  int f1f8;
} Menu;

extern char DAT_00300744[];
extern void FUN_002a2488(Menu *, int);
extern void FUN_002a165c();
extern void FUN_002a1398();
extern void FUN_00058ddc(Menu *, int, int);
extern void FUN_00058ec4(float, Menu *, int);
extern void FUN_00059084(Menu *, int);
extern void FUN_00057bc0(Menu *, int, int, float, float, float);
extern void FUN_00057b58(float, Menu *, int, int);
extern void FUN_002a0b30(char *, int);

void FUN_002a1a64(Menu *self, int v)
{
  if (self->f1b0 == 5) return;
  FUN_002a2488(self, 3);
  self->f1f8 = 5;
  self->f1e8 = v;
  switch (v) {
  case 0x18:
    self->f1e4 = 1;
    self->f1e5 = 0;
    FUN_002a165c(self, 5);
    self->f1e0 = 13;
    FUN_002a1398(self, 1, 13);
    FUN_00058ddc(self, 1, 1);
    FUN_00058ec4(1.0f, self, 1);
    FUN_00059084(self, 1);
    break;
  case 0x17:
  case 0x19:
  case 0x21:
    self->f1e4 = 1;
    self->f1e5 = 0;
    FUN_002a165c(self, 1);
    self->f1e0 = 0;
    FUN_002a1398(self, 1, 0);
    FUN_00058ddc(self, 1, 1);
    FUN_00058ec4(1.0f, self, 1);
    FUN_00059084(self, 1);
    break;
  case 0x1e:
    self->f1e4 = 1;
    self->f1e5 = 0;
    FUN_002a165c(self, 6);
    self->f1e0 = 14;
    FUN_00057bc0(self, 1, -1, 0.0f, 0.0f, 0.0f);
    FUN_00057b58(0.0f, self, 1, 14);
    FUN_00058ddc(self, 1, 1);
    FUN_00058ec4(1.0f, self, 1);
    FUN_00059084(self, 1);
    FUN_00058ddc(self, 0, 1);
    self->f1f8 = 0x28;
    break;
  case 0x20:
    self->f1e4 = 1;
    self->f1e5 = 0;
    FUN_002a165c(self, 6);
    self->f1e0 = 14;
    FUN_00057bc0(self, 1, -1, 0.0f, 0.0f, 0.0f);
    FUN_00057b58(0.0f, self, 1, 14);
    FUN_00058ddc(self, 1, 1);
    FUN_00058ec4(1.0f, self, 1);
    FUN_00059084(self, 1);
    FUN_00058ddc(self, 0, 1);
    self->f1f8 = 0x28;
    break;
  case 0x1a:
  case 0x1b:
  case 0x1c:
    self->f1e4 = 1;
    self->f1e5 = 0;
    FUN_002a165c(self, 6);
    self->f1e0 = 14;
    FUN_00057bc0(self, 1, -1, 0.0f, 0.0f, 0.0f);
    FUN_00057b58(0.0f, self, 1, 14);
    FUN_00058ddc(self, 1, 1);
    FUN_00058ec4(1.0f, self, 1);
    FUN_00059084(self, 1);
    FUN_00058ddc(self, 0, 1);
    self->f1f8 = 0x28;
    break;
  case 0x22:
  default:
    FUN_002a0b30(DAT_00300744, v);
    break;
  }
}
// NEAR 10 words (rodata=0x002cbfe0 jump table matches). Only the pre-switch temporaries differ: retail uses
// `li a1,5; addiu v1,s0,-23; sw a1,504(s1); sltiu at,v1,12` and `lui a0; sll v1; addiu a0; addu v1,v1,a0; lw v1; jr v1`
// (a1/v1/a0) where we emit `li v0,5; sw v0,504; addiu v0,s0,-23; sltiu at,v0,12` and v0/v1 for the dispatch.
// Ruled out (no change): nested `if (f1b0 != 5) {..}`, int/pointer return types on the callees, `int` function result,
// `unsigned v`, `switch (v - 0x17)` with rebased cases, `switch (self->f1e8)`, named local for the constant 5,
// `5 == f1b0`, statement order of the two stores (swapping moves the delay-slot store: 188 words off).
// Lessons applied: FUN_00057bc0 is (Menu *, int, int, float, float, float) (int args evaluated before f13/f14);
// FUN_002a165c(self, 1) in the 0x17/0x19/0x21 arm shares the constant 1 with the store (retail uses a1).
// near miss from lane sn3 (unreviewed)
// opus1 2026-11-09 (10 -> 8): case 0x18 calls FUN_002a165c(self, 5): retail's `li a1,5` before the f1f8 store is the
// constant shared with that call's dropped 2nd argument (a1 is never set in the 0x18 arm). Left: the dispatch temps
// (retail index v1 / table a0, ours v0 / v1); `FUN_002a165c(self, self->f1f8)` 184.
// improved by lane opus1 (unreviewed): 10 -> 8 differing words
