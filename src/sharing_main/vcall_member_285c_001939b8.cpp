/* HSTMATCH module=hstp_trans_main.prx start=0x001939b8 end=0x001939cc symbol=FUN_001939b8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Obj { char pad[0x285c]; Target *target; };
extern "C" void FUN_001939b8(Obj *self) { self->target->v1(); }
