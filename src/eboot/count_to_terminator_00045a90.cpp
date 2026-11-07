/* HSTMATCH module=EBOOT.BIN start=0x00045a90 end=0x00045ac8 symbol=FUN_00045a90 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" const unsigned short DAT_002d01c0;
extern "C" int FUN_00045a90(const unsigned short *s) { int n = 0; while (*s != DAT_002d01c0) { s++; n++; } return n; }
