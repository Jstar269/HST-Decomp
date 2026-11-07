/* HSTMATCH module=hstp_trans_main.prx start=0x001bc52c end=0x001bc544 symbol=FUN_001bc52c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Owner { char pad[0x3fc]; Target *target; };
struct Obj { int pad; Owner *owner; };
extern "C" void FUN_001bc52c(Obj *self) { self->owner->target->v1(); }
