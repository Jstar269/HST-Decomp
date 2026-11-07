/* HSTMATCH module=EBOOT.BIN start=0x00265e0c end=0x00265e20 symbol=FUN_00265e0c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { char pad[0x3c]; Target *target; };
extern "C" void FUN_00265e0c(Obj *self) { self->target->v3(); }
