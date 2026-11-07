/* HSTMATCH module=hstp_trans_main.prx start=0x0021019c end=0x002101cc symbol=FUN_0021019c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { int pad; Target *target; };
extern "C" void FUN_0021019c(Obj *self) { if (self->target) self->target->v3(); }
