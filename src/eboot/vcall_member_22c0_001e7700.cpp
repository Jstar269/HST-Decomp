/* HSTMATCH module=EBOOT.BIN start=0x001e7700 end=0x001e7714 symbol=FUN_001e7700 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Obj { char pad[0x22c0]; Target *target; };
extern "C" void FUN_001e7700(Obj *self) { self->target->v1(); }
