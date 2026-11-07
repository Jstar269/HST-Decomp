/* HSTMATCH module=EBOOT.BIN start=0x00263658 end=0x0026366c symbol=FUN_00263658 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_00263658(Obj *self) { self->target->v3(); }
