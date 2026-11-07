/* HSTMATCH module=EBOOT.BIN start=0x0018726c end=0x00187280 symbol=FUN_0018726c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Obj { char pad[0x285c]; Target *target; };
extern "C" void FUN_0018726c(Obj *self) { self->target->v1(); }
