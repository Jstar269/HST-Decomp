/* HSTMATCH module=hstp_trans_main.prx start=0x002bf77c end=0x002bf7c0 symbol=FUN_002bf77c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { virtual ~Obj(); };
extern "C" Obj *DAT_003f4918;
extern "C" void FUN_002bf77c(void) { if (DAT_003f4918) { delete DAT_003f4918; DAT_003f4918 = 0; } }
