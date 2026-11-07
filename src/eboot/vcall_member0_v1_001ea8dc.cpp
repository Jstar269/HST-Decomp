/* HSTMATCH module=EBOOT.BIN start=0x001ea8dc end=0x001ea8f0 symbol=FUN_001ea8dc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); };
struct Obj { Target *target; };
extern "C" void FUN_001ea8dc(Obj *self) { self->target->v1(); }
