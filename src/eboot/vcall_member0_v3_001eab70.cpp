/* HSTMATCH module=EBOOT.BIN start=0x001eab70 end=0x001eab84 symbol=FUN_001eab70 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { Target *target; };
extern "C" void FUN_001eab70(Obj *self) { self->target->v3(); }
