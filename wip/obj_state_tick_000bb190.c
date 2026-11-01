/* HSTMATCH module=EBOOT.BIN start=0x000bb190 end=0x000bb468 symbol=FUN_000bb190 lang=c99 */
/* Hand-written reconstruction from the disassembly; near miss, not installed. Structural notes in the trailing comments. */
#include "hst/types.h"

extern u8 DAT_00338ac4;
extern int DAT_002d512c; /* "sceNetAdhocMatchingStop() failed" */
extern int DAT_002d515c; /* "sceNetAdhocMatchingDelete() failed" */
extern int DAT_002d5190; /* "MatchingEnd()::Success" */
extern int DAT_002d5030; /* "sceNetAdhocctlDisconnect() failed" */
extern int DAT_002d505c; /* "sceNetAdhocctlDisconnect()::success" */
extern void FUN_000bd014();
extern void FUN_000bda40();
extern void FUN_000bbe00();
extern void FUN_000bdac4();
extern void FUN_000bc800();
extern void FUN_000bdb1c();
extern int FUN_000ba3ac();
extern void FUN_000be8a8();
extern void FUN_00000a00();
extern int FUN_002b7db0();
extern int FUN_002b7df8();
extern int FUN_002b7d68();

/* list header: word 0 = pointer to the first element slot, word 1 = element count */
typedef struct Ref Ref;
struct Ref { char pad[0x10]; int count; };

typedef struct ObjList ObjList;
struct ObjList { Ref **begin; int count; };

void FUN_000bb190(u8 *self)
{
  ObjList *list;
  Ref **p;
  Ref **end;
  Ref *r;
  int iVar1;

  if (*(u8 *)(self + 0x278) == 0)
    return;
  FUN_000bd014();
  list = *(ObjList **)(self + 0x50);
  if (list == 0) {
    iVar1 = *(int *)(self + 0x28);
  } else {
    p = list->begin;
    end = list->begin + list->count;
    if (p == end) {
      iVar1 = list->count;
    } else {
      for (; p != end; p++) {
        r = *p;
        if (r->count > 0) {
          r->count = r->count - 1;
          if (r->count > 0)
            continue;
          break;
        }
      }
      iVar1 = (*(ObjList **)(self + 0x50))->count;
    }
    if (iVar1 == 0) {
      if (list == 0) {
        *(void **)(self + 0x50) = 0;
      } else {
        FUN_000be8a8(list, 0);
        FUN_00000a00(list);
        *(void **)(self + 0x50) = 0;
      }
    }
    iVar1 = *(int *)(self + 0x28);
  }
  if (iVar1 == 0xb) {
    iVar1 = *(int *)(self + 0x17c);
  } else if (iVar1 == 9) {
    if (*(u8 *)(self + 0x178) != 0 && *(int *)(self + 400) == 1) {
      *(int *)(self + 400) = 0;
      if (-1 < *(int *)(self + 0x16c)) {
        iVar1 = FUN_002b7db0();
        if (iVar1 < 0) {
          FUN_000ba3ac(&DAT_002d512c, iVar1);
        } else {
          iVar1 = FUN_002b7df8(*(int *)(self + 0x16c));
          if (iVar1 < 0) {
            FUN_000ba3ac(&DAT_002d515c, iVar1);
          } else {
            FUN_000ba3ac(&DAT_002d5190);
            *(int *)(self + 0x16c) = -1;
          }
        }
      }
      FUN_000bdac4(self);
      *(int *)(self + 0x17c) = -1;
      *(u8 *)(self + 0x168) = 1;
      iVar1 = FUN_002b7d68();
      if (iVar1 < 0) {
        FUN_000ba3ac(&DAT_002d5030, iVar1);
      } else {
        FUN_000ba3ac(&DAT_002d505c);
        *(int *)(self + 0x28) = 3;
      }
    }
    iVar1 = *(int *)(self + 0x17c);
  } else {
    if (iVar1 == 4 && *(u8 *)(self + 0x178) != 0)
      FUN_000bbe00(self);
    iVar1 = *(int *)(self + 0x17c);
  }
  if (-1 < iVar1 && *(int *)(self + 0x28) < 9 && *(u8 *)(self + 0x178) != 0 &&
      (*(u32 *)(self + 0x274) % 0x5a) == 0)
    FUN_000bc800(self);
  FUN_000bdb1c(self);
  *(int *)(self + 0x274) = *(int *)(self + 0x274) + 1;
}

// NEAR (draft, first pass). Retail structure read from tools/hstdis.py:
//   0xbb1ac  if (*(u8*)(this+0x278) == 0) return;          (beqz, whole-body guard)
//   0xbb1b4  FUN_000bd014();
//   0xbb1b0  list = *(ObjList**)(this+0x50);  beqzl list -> n = this->0x28 path
//   loop:    p = list->begin; end = list->begin + list->count*4; if (p == end) n = list->count;
//            body reloads `*p` after the store to elem->count (store aliases this->0x50, so MWCC
//            reloads list each iteration; the second blezl skips the p++ when count hits 0).
//   release: if (n == 0) { FUN_000be8a8(list, 0); FUN_00000a00(list); this->0x50 = 0; }
//   dispatch: switch on this->0x28 with cases 0xb, 9, 4 -> retail lowers it as beq chain 11, 9, 4.
//   case 9 body ends with this->0x28 = 3 and the compiler then tests the KNOWN 3 against 0xa, 8, 4
//            (dead compares, jumps to 0xbb3cc when equal) - reproduce by re-switching on the local
//            that was just assigned 3, with cases 10 and 8/4 sharing the 0xbb3cc block:
//            0xbb3cc: this->0x17c = FUN_000bda40(&DAT_00338ac4 + 56, *(u16*)(this+0x10), 1444, 0) & 0xff;
//   tail:    0xbb3f0 reloads this->0x17c, then the 4-condition gate (>=0, state<9, flag, count%90==0)
//            -> FUN_000bc800(this); then FUN_000bdb1c(this); this->0x274++;
// NEXT LEVER: model the case-9 tail as `iVar1 = 3; this->state = iVar1; switch (iVar1) { case 10: break;
//   case 8: case 4: <0xbb3cc block>; }` and give FUN_000bda40 its 5 arguments (it takes
//   (&DAT_00338ac4+56, u16 at +0x10, 1444, 0)); that is the only way to get the dead compares.
// near miss from lane dsn8 (unreviewed)
