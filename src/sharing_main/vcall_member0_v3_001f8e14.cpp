/* HSTMATCH module=hstp_trans_main.prx start=0x001f8e14 end=0x001f8e28 symbol=FUN_001f8e14 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { Target *target; };
extern "C" void FUN_001f8e14(Obj *self) { self->target->v3(); }
