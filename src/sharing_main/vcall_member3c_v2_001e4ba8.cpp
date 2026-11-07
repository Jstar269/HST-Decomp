/* HSTMATCH module=hstp_trans_main.prx start=0x001e4ba8 end=0x001e4bbc symbol=FUN_001e4ba8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_001e4ba8(Obj *self) { self->target->v2(); }
