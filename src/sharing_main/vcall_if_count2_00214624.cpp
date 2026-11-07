/* HSTMATCH module=hstp_trans_main.prx start=0x00214624 end=0x0021465c symbol=FUN_00214624 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); };
struct Obj { Target *target; char pad[0x48]; int count; };
extern "C" void FUN_00214624(Obj *self) { if (self->count >= 2) self->target->v2(); }
