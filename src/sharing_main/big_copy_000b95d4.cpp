/* HSTMATCH module=hstp_trans_main.prx start=0x000b95d4 end=0x000b9610 symbol=FUN_000b95d4 lang=c++ */
/* Clone of src/eboot/big_copy_000b62e4.cpp (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Big {
  int a[16];
  int f40;
  Big() {}
};

extern "C" Big *FUN_000b95d4(Big *param_1, const Big *param_2)
{
  *param_1 = *param_2;
  return param_1;
}
