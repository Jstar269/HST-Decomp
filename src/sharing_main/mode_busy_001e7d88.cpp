/* HSTMATCH module=hstp_trans_main.prx start=0x001e7d88 end=0x001e7db4 symbol=FUN_001e7d88 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x54]; int mode; bool Busy() const { return mode == 1 || mode == 2; } };
struct Obj { char pad[0x3c]; Sub *sub; };
extern "C" bool FUN_001e7d88(Obj *self) { return self->sub->Busy(); }
