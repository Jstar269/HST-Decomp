/* HSTMATCH module=hstp_trans_main.prx start=0x000bdad0 end=0x000bdcb0 symbol=FUN_000bdad0 lang=c++ data=0x0037b730 */
/* Hand-written reconstruction: clone of the source of net_shutdown_000ba78c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   Matching idiom: each slot release keeps a redundant second null test (`if (p != 0) { if (p != 0) ... }`) -
   retail carries three tests per slot (outer guard, redundant guard, and delete's own test), and MWCC folds
   away a single guard. The net teardown is written as sequential early-return guards, which is what puts the
   continuation out of line and leaves `beqzl v0,<success>`. Data: its string literals (6 strings, 0x002d4e68-0x002d4f1a) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */

#include "hst/adhoc_net.h"

extern "C" void FUN_000009e4(void *);
extern "C" void FUN_000bd6f0(char *, ...);
extern "C" void FUN_000be1f4(void *);
extern "C" void FUN_000bde38(void *);
extern "C" int FUN_002c7b00(void);
extern "C" int FUN_002c7bc8(void);
extern "C" int FUN_002c7b18(void);

struct AdhocObj {
  virtual ~AdhocObj();
};

/* Releases the working buffers and slot objects, stops matching, then shuts the net stack down; returns 1 on success. */
extern "C" int FUN_000bdad0(AdhocNet *self)
{
  int i;
  int result;
  AdhocObj *obj;

  if (self->bufC != 0) {
    if (self->bufC != 0) {
      FUN_000009e4(self->bufC);
      self->bufC = 0;
    }
  }
  if (self->bufA != 0) {
    if (self->bufA != 0) {
      FUN_000009e4(self->bufA);
      self->bufA = 0;
    }
  }
  if (self->bufB != 0) {
    if (self->bufB != 0) {
      FUN_000009e4(self->bufB);
      self->bufB = 0;
    }
  }

  for (i = 0; i < ADHOC_SLOT_COUNT; i++) {
    obj = self->slotObj[i];
    if (obj != 0) {
      if (obj != 0) {
        delete obj;
        self->slotObj[i] = 0;
      }
    }
    obj = self->slotAux[i];
    if (obj != 0) {
      if (obj != 0) {
        delete obj;
        self->slotAux[i] = 0;
      }
    }
  }

  if (self->unk26c != 0) {
    if (self->unk26c != 0) {
      delete self->unk26c;
      self->unk26c = 0;
    }
  }
  FUN_000be1f4(self);
  FUN_000bde38(self);

  result = FUN_002c7b00();
  if (result != 0) {
    FUN_000bd6f0("sceNetAdhocctlTerm() failed(0x%X)\n",result);
    return 0;
  }
  FUN_000bd6f0("sceNetAdhocctlTerm() success\n");
  result = FUN_002c7bc8();
  if (result != 0) {
    FUN_000bd6f0("sceNetAdhocTerm() failed(0x%X)\n",result);
    return 0;
  }
  FUN_000bd6f0("sceNetAdhocTerm() success\n");
  result = FUN_002c7b18();
  if (result != 0) {
    FUN_000bd6f0("sceNetTerm() failed(0x%X)\n",result);
    return 0;
  }
  FUN_000bd6f0("sceNetTerm() success\n");
  return 1;
}
