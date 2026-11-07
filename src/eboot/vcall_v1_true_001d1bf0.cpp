/* HSTMATCH module=EBOOT.BIN start=0x001d1bf0 end=0x001d1c04 symbol=FUN_001d1bf0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Target { virtual void v0(); virtual void v1(int on); };
extern "C" void FUN_001d1bf0(Target *self) { self->v1(1); }
