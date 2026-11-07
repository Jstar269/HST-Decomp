/* HSTMATCH module=hstp_trans_main.prx start=0x000938e0 end=0x00093954 symbol=FUN_000938e0 lang=c++ */
/* Hand-written reconstruction: clone of the source of clamp_set_00091c48.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/model_set_full.h"

extern "C" int DAT_00378b08; /* lowest accepted level */
extern "C" int DAT_00378b0c; /* highest accepted level */
extern "C" void FUN_00093954(void);

static inline int Clamp(const int &v, const int &lo, const int &hi)
{
  return v < lo ? lo : (hi < v ? hi : v);
}

/* Set the level (clamped to its limits); refill the entry table when it changed and is non-negative. */
extern "C" void FUN_000938e0(ModelSetFull *self,int level)
{
  int n = Clamp(level, DAT_00378b08, DAT_00378b0c);

  if (self->level != n) {
    self->level = n;
    if (n >= 0) {
      FUN_00093954();
    }
  }
}
