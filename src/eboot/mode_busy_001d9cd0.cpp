/* HSTMATCH module=EBOOT.BIN start=0x001d9cd0 end=0x001d9cfc symbol=FUN_001d9cd0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x54]; int mode; bool Busy() const { return mode == 1 || mode == 2; } };
struct Obj { char pad[0x3c]; Sub *sub; };
extern "C" bool FUN_001d9cd0(Obj *self) { return self->sub->Busy(); }
