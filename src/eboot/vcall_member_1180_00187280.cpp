/* HSTMATCH module=EBOOT.BIN start=0x00187280 end=0x00187294 symbol=FUN_00187280 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Obj { char pad[0x1180]; Target *target; };
extern "C" void FUN_00187280(Obj *self) { self->target->v1(); }
