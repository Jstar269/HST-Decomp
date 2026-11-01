/* HSTMATCH module=EBOOT.BIN start=0x0017d470 end=0x0017d590 symbol=FUN_0017d470 lang=c++ */
#include "hst/types.h"
#include "hst/slot_mgr.h"

extern "C" void FUN_000937f0(void *p, int a, float f);

extern "C" void FUN_0017d470(SlotMgr *self, int slot)
{
  bool found = false;
  int i = 0;
  int old = self->cur;
  SlotMgr *t0 = self;

  self->cur = slot;
  do {
    int *rec = (int *)((char *)t0 + 0x5c);
    if (rec[0] == slot) {
      if (rec[2] != VALUE_UNSET || rec[4] != VALUE_UNSET) {
        if (self->unk50 != 1) {
          self->unk54 = 1;
          if (self->unk54 != 0) { self->unk50 = 1; self->unk54 = 0; }
        } else {
          if (self->unk54 != 0) self->unk54 = 0;
        }
        found = true;
        break;
      }
    }
    i++;
    t0 = (SlotMgr *)((char *)t0 + 32);
  } while (i < 10);

  if (!found) {
    SlotEntry *entry = self->entries[slot];
    if (self->unk50 != 3) {
      self->unk54 = 3;
    } else {
      if (self->unk54 != 0) self->unk54 = 0;
    }
    if (slot != old) {
      SlotEntry *old_entry = self->entries[old];
      if (*(int *)old_entry == *(int *)entry) {
        int v0 = *(int *)((char *)old_entry + 104);
        float f = (v0 != 0) ? *(float *)(v0 + 0x108) : 0.0f;
        FUN_000937f0(entry->sub, -1, f);
      }
    }
  }
}

// Differing words: 1 (at +0xf0, 0x0017d560: ours bne a0,v1, retail bne v1,a0).
// What differs: bne operand register order comparing *(int *)entry (in v1) and *(int *)old_entry (in a0).
// What was ruled out: writing *(int *)entry == *(int *)old_entry directly alters the preceding load schedule (6 words differ).
// What was ruled out: comma expressions, inline helpers, and explicit local tag variables all alter preceding load order or spill to different registers.
// near miss from lane dsn4 (unreviewed)
