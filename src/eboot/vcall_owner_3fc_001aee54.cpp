/* HSTMATCH module=EBOOT.BIN start=0x001aee54 end=0x001aee6c symbol=FUN_001aee54 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(); };
struct Owner { char pad[0x3fc]; Target *target; };
struct Obj { int pad; Owner *owner; };
extern "C" void FUN_001aee54(Obj *self) { self->owner->target->v1(); }
