/* HSTMATCH module=hstp_trans_main.prx start=0x002a9e18 end=0x002a9f70 symbol=FUN_002a9e18 lang=c++ */
/* Hand-written reconstruction: clone of the source of net_match_teardown_0029ab94.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/net_adhoc.h"

struct RefObj {
  virtual void Release(int);
  virtual void Slot0();
};

extern "C" void FUN_002c7700(int, int);
extern "C" void FUN_000c3d90(RefObj *);
extern "C" int FUN_002c7b48(void);
extern "C" void FUN_002c7518(int);
extern "C" void FUN_002a9808(const char *fmt, ...);
extern "C" void FUN_000bdad0(RefObj *);
extern "C" void FUN_00000a00(MatchState *);
extern "C" char DAT_002db86c[];
extern "C" char DAT_002db880[];
extern "C" char DAT_002db8a4[];
extern "C" RefObj *DAT_003e1e04;

/* Match state deleting destructor: leaves the ad-hoc session, releases its objects, frees when flag > 0. */
extern "C" MatchState *FUN_002a9e18(MatchState *self,short flag)
{
  RefObj *p;
  int r;

  if (self != 0) {
    FUN_002c7700(0,0x26);
    if (self->ref != 0) {
      FUN_000c3d90(self->ref);
    }
    p = self->ref;
    if (p != 0) {
      if (p != 0) {
        p->Release(1);
      }
      self->ref = 0;
    }
    r = self->handle;
    if (r >= 0) {
      FUN_002c7518(r);
      self->handle = -1;
    }
    if (self->active != 0) {
      self->active = 0;
      r = FUN_002c7b48();
      if (r < 0) {
        FUN_002a9808(DAT_002db8a4,r);
      } else {
        FUN_002a9808(DAT_002db880);
      }
      FUN_000bdad0(DAT_003e1e04);
    }
    if (DAT_003e1e04 != 0) {
      if (DAT_003e1e04 != 0) {
        DAT_003e1e04->Release(1);
      }
      DAT_003e1e04 = 0;
    }
    FUN_002a9808(DAT_002db86c);
    p = self->obj1c;
    if (p != 0) {
      if (p != 0) {
        p->Release(1);
      }
      self->obj1c = 0;
    }
    if (0 < flag) {
      FUN_00000a00(self);
    }
  }
  return self;
}
