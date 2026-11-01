/* HSTMATCH module=EBOOT.BIN start=0x001c3280 end=0x001c352c symbol=FUN_001c3280 lang=c99 */

extern int FUN_00190724(int);
extern void FUN_00103628(int, int);
extern int FUN_0018c9f4(int);
extern void FUN_00114168(int, int, char *, int, int);
extern void FUN_0010b8cc(int, int, int);
extern int FUN_0022a930(void);
extern void FUN_001b3ea4(int, int);
extern void FUN_001b4848(int, int);
extern int FUN_001155c0(int);
extern int FUN_001156d4(int);
extern void FUN_001ca490(int);
extern void FUN_001ca480(int);
extern void FUN_002879e0(int, int, int, int, int, int);
extern void FUN_001b46bc(int);
extern void FUN_001cd3d0(int);
extern void FUN_001cd88c(int, int, int);

extern int DAT_003415d4;
extern int DAT_003415ac;
extern int DAT_0033b574;
extern int DAT_002c0e20;
extern int DAT_002c0e1c;
extern int DAT_002c0e18;

void FUN_001c3280(int param_1)
{
  unsigned char cVar1;
  int iVar2;
  int uVar3;
  char *puVar4;

  if (*(int *)(param_1 + 8) == 0) {
    *(int *)(param_1 + 0xec) = 0;
    *(char *)(param_1 + 0xf1) = 0;
  }
  if (*(int *)(param_1 + 0xec) >= 1) {
    iVar2 = *(int *)(param_1 + 0xec) - 1;
    *(int *)(param_1 + 0xec) = iVar2;
    if (iVar2 < 1) {
      if (*(int *)(param_1 + 0xe8) == 1) {
        FUN_001ca490(*(int *)(param_1 + 0x18));
        uVar3 = *(int *)(param_1 + 0x18);
      }
      else {
        if (*(int *)(param_1 + 0xe8) != 3) {
          if (*(char *)(param_1 + 0xf0) != 0) {
            FUN_002879e0(0xbf800000, DAT_0033b574 + 0x1320, 0, 0xf, 0, 0);
            FUN_001b46bc(0x34179c);
          }
        }
        else {
          FUN_001ca480(*(int *)(param_1 + 0x18));
        }
        uVar3 = *(int *)(param_1 + 0x18);
      }
      FUN_001cd3d0(uVar3);
      FUN_001cd88c(param_1, 0, 1);
    }
  }
  else {
    if (*(int *)(param_1 + 0xe8) == 2) {
      if ((*(char *)(param_1 + 0xf1) == 0) && (iVar2 = FUN_00190724(DAT_003415d4), iVar2 != 0)) {
        FUN_00103628(DAT_003415ac, 1);
        iVar2 = FUN_0018c9f4(0x11);
        if (*(int *)(iVar2 + 4) == 0) {
          puVar4 = (char *)&DAT_002c0e20;
        }
        else if (*(int *)(*(int *)(iVar2 + 8) + 8) < 9) {
          puVar4 = (char *)&DAT_002c0e1c;
        }
        else if (*(int *)(*(int *)(iVar2 + 0xc) + 100) < 3) {
          puVar4 = (char *)&DAT_002c0e18;
        }
        else {
          puVar4 = *(char **)(*(int *)(*(int *)(iVar2 + 0xc) + 0x60) + 8);
        }
        FUN_00114168(DAT_003415ac, 1, puVar4, 0, 0);
        FUN_0010b8cc(*(int *)(DAT_003415ac + 0x58), 1, 0);
        *(char *)(param_1 + 0xf1) = 1;
      }
      cVar1 = *(char *)(param_1 + 0xf1);
    }
    else {
      cVar1 = *(char *)(param_1 + 0xf1);
    }
    if (cVar1 == 0) {
      iVar2 = FUN_00190724(DAT_003415d4);
      if (iVar2 != 0) {
        FUN_00103628(DAT_003415ac, 1);
        *(int *)(param_1 + 0xec) = 6;
        if (*(int *)(param_1 + 0xe8) == 0) {
          iVar2 = FUN_0022a930();
          *(char *)(iVar2 + 0xab4c) = 0;
        }
        else {
          FUN_001b3ea4(0x3dcccccd, 0x34179c);
          FUN_001b4848(0x3dcccccd, 0x34179c);
        }
      }
    }
    else {
      iVar2 = FUN_001155c0(DAT_003415ac);
      if (iVar2 != 0) {
        FUN_00103628(DAT_003415ac, 1);
        iVar2 = FUN_001156d4(DAT_003415ac);
        if (iVar2 == 0) {
          *(int *)(param_1 + 0xe8) = 3;
        }
        else {
          *(int *)(param_1 + 0xe8) = 0;
        }
        *(char *)(param_1 + 0xf1) = 0;
      }
    }
  }
  return;
}
// Ruling: PROGRESS.md VERDICTS (0x001c3280); experiment: try/hsttry/state_step_001c3280-14776
// near miss from lane dsn8 (unreviewed)
