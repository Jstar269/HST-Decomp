/* HSTMATCH module=EBOOT.BIN start=0x0028f5bc end=0x0028f5d0 symbol=FUN_0028f5bc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_0028f5bc(Obj *self) { self->target->v2(); }
