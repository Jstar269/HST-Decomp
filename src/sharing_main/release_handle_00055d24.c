/* HSTMATCH module=hstp_trans_main.prx start=0x00055d24 end=0x00055d60 symbol=FUN_00055d24 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
struct Obj { char pad[0x8c]; void *handle; u8 active; };
extern void FUN_000541d8(void *handle, int flag);
void FUN_00055d24(struct Obj *self) { if (self->active != 0) { FUN_000541d8(self->handle, 1); self->active = 0; } self->handle = 0; }
