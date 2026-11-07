/* HSTMATCH module=EBOOT.BIN start=0x00127ac0 end=0x00127d5c symbol=FUN_00127ac0 lang=c++ */
/* Hand-written reconstruction: clone of the source of FUN_00132d30.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Stores the byte flag at +0x338 when it changes. Cleared: a slot index at +0x334 inside [DAT_0033b564,
   DAT_0033b570] is released (-1) and 0x13671c(self, 1, 1, -1.0f) runs when the target at +0x58 sits on the
   current slot, its first index is in range with the second negative, or its second index is in range. Set: an unset slot index is
   re-seeded from DAT_0033b56c/DAT_0033b564 + the base at +0x354 and 0x13671c runs when the target sits on it.
   Matching idioms: the range test and both predicates are static inline functions returning their && / ||
   value (retail materialises them: slt; xori / li 1 ... move zero); the two outer flags are explicit
   `bool ok = true; if (!A && !B) ok = false;` locals declared before a and b (saved-register order);
   AtSlot spells DAT_0033b4b0 + pos twice so the sum is formed after its `true` (CSE'd once). */
struct Target {
  char pad[0xf0];
  int f0f0;
  char pad2[0x110 - 0xf4];
  int f110;
};

struct Obj {
  char pad00[0x58];
  Target *f58;
  char pad5c[0x334 - 0x5c];
  int f334;
  unsigned char f338;
  char pad339[0x354 - 0x339];
  int f354;
  char pad358[0x35c - 0x358];
  void *f35c;
};

extern "C" {
extern int DAT_0033b4b0;
extern int DAT_0033b564;
extern int DAT_0033b56c;
extern int DAT_0033b570;
extern int FUN_0012b72c(Obj *, void *, int);
extern void FUN_0012b2b0(Obj *, int, int, float);
}

static inline bool InRange(int v) { return DAT_0033b564 <= v && v <= DAT_0033b570; }
static inline bool InRangeNeg(int a, int b) { return InRange(a) && b < 0; }
static inline bool AtSlot(Obj *self)
{
  int pos = self->f354 + FUN_0012b72c(self, self->f35c, 0) * 2;
  Target *t = self->f58;
  int c;
  if (t != 0) c = t->f0f0; else c = -1;
  int d = t->f110;
  return (c == DAT_0033b4b0 + pos && d < 0) || d == DAT_0033b4b0 + pos;
}

extern "C" void FUN_00127ac0(Obj *self, unsigned char on)
{
  if (on != self->f338) {
    self->f338 = on;
    if (self->f338 == 0) {
      if (InRange(self->f334)) {
        bool ok;
        bool near;
        self->f334 = -1;
        Target *t = self->f58;
        int a = t ? t->f0f0 : -1;
        int b = t->f110;
        ok = true;
        near = true;
        if (!AtSlot(self) && !InRangeNeg(a, b)) near = false;
        if (!near && !InRange(b)) ok = false;
        if (ok) FUN_0012b2b0(self, 1, 1, -1.0f);
      }
    } else if (self->f334 < 0) {
      int idx = FUN_0012b72c(self, self->f35c, 0);
      self->f334 = idx == 3 ? DAT_0033b56c + self->f354 : DAT_0033b564 + self->f354;
      if (AtSlot(self)) FUN_0012b2b0(self, 1, 1, -1.0f);
    }
  }
}
