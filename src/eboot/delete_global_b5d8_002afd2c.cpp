/* HSTMATCH module=EBOOT.BIN start=0x002afd2c end=0x002afd70 symbol=FUN_002afd2c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { virtual ~Obj(); };
extern "C" Obj *DAT_0034b5d8;
extern "C" void FUN_002afd2c(void) { if (DAT_0034b5d8) { delete DAT_0034b5d8; DAT_0034b5d8 = 0; } }
