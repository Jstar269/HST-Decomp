/* HSTMATCH module=hstp_trans_main.prx start=0x002744d4 end=0x002744e8 symbol=FUN_002744d4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_002744d4(Obj *self) { self->target->v3(); }
