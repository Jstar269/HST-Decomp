/* HSTMATCH module=hstp_trans_main.prx start=0x001f5918 end=0x001f592c symbol=FUN_001f5918 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Obj { char pad[0x22c0]; Target *target; };
extern "C" void FUN_001f5918(Obj *self) { self->target->v1(); }
