/* HSTMATCH module=EBOOT.BIN start=0x0022bc14 end=0x0022bd08 symbol=FUN_0022bc14 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" {
int FUN_0022b6a0(int, int);
void FUN_0022b02c(int, int, int);
void FUN_0022bd08(int);
}

extern "C" int FUN_0022bc14(int param_1, int param_2, int param_3)
{
  int neq3;
  int neq2 = (param_2 != FUN_0022b6a0(param_1, 0x16));
  neq3 = (param_3 != FUN_0022b6a0(param_1, 1));
  if (neq2) {
    FUN_0022b02c(param_1, 0, 0);
    FUN_0022b02c(param_1, 2, 0);
    FUN_0022b02c(param_1, 0xd, 0);
    FUN_0022bd08(param_1);
    FUN_0022b02c(param_1, 0x16, param_2);
    FUN_0022b02c(param_1, 1, param_3);
  } else if (neq3) {
    FUN_0022bd08(param_1);
    FUN_0022b02c(param_1, 1, param_3);
  }
  return neq3;
}
