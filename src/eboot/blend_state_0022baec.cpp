/* HSTMATCH module=EBOOT.BIN start=0x0022baec end=0x0022bbc8 symbol=FUN_0022baec lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" {
int FUN_0022b6a0(int, int);
void FUN_0022bbc8(int);
void FUN_0022bd08(int);
void FUN_0022b02c(int, int, int);
}

extern "C" int FUN_0022baec(int param_1, int param_2, int param_3)
{
  int neq3;
  int neq2 = (param_2 != FUN_0022b6a0(param_1, 0x16));
  neq3 = (param_3 != FUN_0022b6a0(param_1, 0));
  if (neq2) {
    FUN_0022bbc8(param_1);
    FUN_0022bd08(param_1);
    FUN_0022b02c(param_1, 0x16, param_2);
    FUN_0022b02c(param_1, 0, param_3);
    FUN_0022b02c(param_1, 1, param_3);
  } else if (neq3) {
    FUN_0022bbc8(param_1);
    FUN_0022b02c(param_1, 0, param_3);
  }
  return neq3;
}
