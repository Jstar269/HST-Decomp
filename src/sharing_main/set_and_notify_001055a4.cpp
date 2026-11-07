/* HSTMATCH module=hstp_trans_main.prx start=0x001055a4 end=0x001055d8 symbol=FUN_001055a4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Target { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); };
struct Obj { int value; char pad[0x1c]; Target *listener; };
extern "C" void FUN_001055a4(Obj *self, int value) { self->value = value; if (self->listener) self->listener->v4(); }
