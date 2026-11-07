/* HSTMATCH module=EBOOT.BIN start=0x001fd110 end=0x001fd154 symbol=FUN_001fd110 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { virtual ~Obj(); };
extern "C" Obj *DAT_0034a13c;
extern "C" void FUN_001fd110(void) { if (DAT_0034a13c) { delete DAT_0034a13c; DAT_0034a13c = 0; } }
