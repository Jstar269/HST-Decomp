/* HSTMATCH module=hstp_trans_main.prx start=0x0020b8e0 end=0x0020b924 symbol=FUN_0020b8e0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { virtual ~Obj(); };
extern "C" Obj *DAT_003f3474;
extern "C" void FUN_0020b8e0(void) { if (DAT_003f3474) { delete DAT_003f3474; DAT_003f3474 = 0; } }
