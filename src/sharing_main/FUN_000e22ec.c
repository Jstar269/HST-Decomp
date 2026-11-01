/* HSTMATCH module=hstp_trans_main.prx start=0x000e22ec end=0x000e23f4 symbol=FUN_000e22ec lang=c99 */
/* Clone of src/eboot/FUN_000de100.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly and pseudocode; exact match verified; names and types await human review. */
typedef unsigned char undefined1;
typedef unsigned int undefined4;

extern unsigned char DAT_003eaaf3;
extern int FUN_000e30f8();

void FUN_000e22ec(int param_1, int *param_2, undefined1 *param_3, undefined4 *param_4)
{
  int limit;
  int *piVar2;
  int iVar1;
  
  *param_3 = 0;
  *param_4 = 0xffffffff;
  *param_2 = *(int *)(param_1 + 0xd0);
  param_2[1] = *(int *)(param_1 + 0xd4);
  if (FUN_000e30f8() != 0) {
    return;
  }
  if (DAT_003eaaf3 == 1) {
    return;
  }
  limit = *(int *)(param_1 + 0xcc) - 1;
  if (*param_2 < limit) {
    return;
  }
  if (param_2[1] < limit) {
    return;
  }

  piVar2 = (*(volatile int *)(param_2 + 1) < *(volatile int *)param_2) ? (param_2 + 1) : param_2;
  iVar1 = *piVar2;
  *param_2 = *param_2 - iVar1;
  iVar1 = param_2[1] - iVar1;
  param_2[1] = iVar1;
  if (*param_2 == iVar1) {
    *param_3 = 1;
  }
  else if (iVar1 < *param_2) {
    *param_4 = 0;
  }
  else {
    *param_4 = 1;
  }
}
