/* HSTMATCH module=hstp_trans_main.prx start=0x000b520c end=0x000b5230 symbol=FUN_000b520c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
struct Obj { char pad[0x418]; u8 f418; u8 f419; char pad2[0x41c - 0x41a]; int f41c; int f420; int f424; char pad3[0x454 - 0x428]; int f454; };
extern void FUN_000b52a4(struct Obj *self, int idx, int flag, float v);
void FUN_000b520c(struct Obj *self, int idx) { self->f418 = 0; self->f419 = 0; self->f41c = 0; self->f420 = 0; self->f424 = 0; self->f454 = 0; FUN_000b52a4(self, idx, 0, 0.0f); }
