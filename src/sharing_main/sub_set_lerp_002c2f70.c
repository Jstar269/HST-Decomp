/* HSTMATCH module=hstp_trans_main.prx start=0x002c2f70 end=0x002c2f8c symbol=FUN_002c2f70 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x41c]; char sub[4]; };
extern void FUN_002268b4(void *sub, int steps, float a, float b);
void FUN_002c2f70(struct Obj *self) { FUN_002268b4(self->sub, 5, 2.0f, 1.0f); }
