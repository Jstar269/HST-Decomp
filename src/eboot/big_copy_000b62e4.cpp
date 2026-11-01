/* HSTMATCH module=EBOOT.BIN start=0x000b62e4 end=0x000b6320 symbol=FUN_000b62e4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Big {
  int a[16];
  int f40;
  Big() {}
};

extern "C" Big *FUN_000b62e4(Big *param_1, const Big *param_2)
{
  *param_1 = *param_2;
  return param_1;
}
