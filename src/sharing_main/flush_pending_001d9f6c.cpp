/* HSTMATCH module=hstp_trans_main.prx start=0x001d9f6c end=0x001d9fa0 symbol=FUN_001d9f6c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0xfc]; unsigned char pending; char pad1[0x15c - 0xfd]; void *target; };
extern "C" void FUN_001d07f4(void *target, int a, int b, unsigned char c);
extern "C" void FUN_001d9f6c(Obj *self, int a) { FUN_001d07f4(self->target, a, true, self->pending); self->pending = false; }
