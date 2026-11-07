/* HSTMATCH module=EBOOT.BIN start=0x001d99f8 end=0x001d9a0c symbol=FUN_001d99f8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_001d99f8(Obj *self) { self->target->v3(); }
