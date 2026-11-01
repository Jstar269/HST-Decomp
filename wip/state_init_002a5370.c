/* HSTMATCH module=EBOOT.BIN start=0x002a5370 end=0x002a55f8 symbol=FUN_002a5370 lang=c99 */

extern int FUN_00102e38();          /* called both with no argument and with one */
extern void FUN_00104118(void *, void *, int);
extern void FUN_00103540(int, void *);
extern int FUN_00000bcc(int);
extern int *FUN_001039d8(void *, void *, int);
extern int FUN_001026b8(void *, void *);
extern void FUN_000ffb3c(int, int);
extern int FUN_0008d994(void);
extern void FUN_00103628(int, int);
extern int FUN_001fea10(int, int, int);
extern void FUN_00110a78(int, int, int);
extern void FUN_002a57ec(void *, int, int);

extern int DAT_00309f78;
extern char DAT_00300a94[];
extern char DAT_00300abc[];
extern int DAT_00300ae4;
extern int DAT_00300ae8;
extern int DAT_0034a12c;
extern int DAT_0034b354;
extern int DAT_003386c8;
extern int *PTR_00300864[];
extern int DAT_00300960[];

int *FUN_002a5370(int *param_1)
{
  int *piVar3;
  int iVar1;
  int *uVar2;
  int iVar4;
  int iVar5;
  int **ppuVar6;
  int iVar7;
  int *obj;

  FUN_00102e38();
  *param_1 = (int)&DAT_00309f78;
  FUN_00104118(param_1, DAT_00300a94, 0);
  FUN_00103540(0x447a0000, param_1);
  iVar1 = FUN_00000bcc(0x54);
  uVar2 = 0;
  if (iVar1 != 0) {
    uVar2 = (int *)FUN_00102e38(iVar1);
  }
  param_1[0x15] = (int)uVar2;
  FUN_00104118(uVar2, DAT_00300abc, 0);
  FUN_00103540(0x447a0000, uVar2);
  piVar3 = FUN_001039d8(uVar2, &DAT_00300ae4, 0);
  (*(void (**)(int *, int))(*piVar3 + 0x1c))(piVar3, 0xffffe800);
  uVar2 = FUN_001039d8(uVar2, &DAT_00300ae4, 0);
  iVar1 = FUN_001026b8(uVar2, &DAT_00300ae8);
  iVar5 = *(int *)(iVar1 + 0x20);
  param_1[0x37] = iVar5;
  iVar1 = DAT_0034a12c;
  iVar7 = 0;
  if (0 < *(int *)(iVar5 + 0x14)) {
    iVar4 = *(int *)(iVar5 + 0x10);
    while (1) {
      FUN_000ffb3c(*(int *)(iVar4 + iVar7 * 4), iVar7 != iVar1);
      iVar7 = iVar7 + 1;
      if (*(int *)(iVar5 + 0x14) <= iVar7) {
        break;
      }
      iVar4 = *(int *)(iVar5 + 0x10);
    }
  }
  iVar5 = 0;
  ppuVar6 = PTR_00300864;
  iVar1 = 0;
  do {
    piVar3 = FUN_001039d8(param_1, *ppuVar6, 0);
    *(int **)((int)param_1 + iVar1 + 0x58) = piVar3;
    (*(void (**)(int *, int))(*piVar3 + 0x1c))(piVar3, 0xfffff800);
    iVar5 = iVar5 + 1;
    ppuVar6 = ppuVar6 + 1;
    iVar1 = iVar1 + 4;
  } while (iVar5 < 6);
  iVar5 = 0;
  piVar3 = DAT_00300960;
  iVar1 = 0;
  do {
    iVar7 = FUN_001026b8((void *)param_1[*piVar3 + 0x16], (void *)piVar3[1]);
    *(int *)((int)param_1 + iVar1 + 0x70) = iVar7;
    iVar5 = iVar5 + 1;
    iVar1 = iVar1 + 4;
    piVar3[2] = *(int *)(iVar7 + 0x24);
    piVar3 = piVar3 + 3;
  } while (iVar5 < 0x19);
  iVar1 = *(int *)(param_1[0x1d] + 0x20);
  param_1[0x35] = iVar1;
  *(int *)(iVar1 + 0xd8) = 0;
  *(unsigned char *)(iVar1 + 0x114) = 1;
  piVar3 = (int *)param_1[0x35];
  (*(void (**)(int *, int))(*piVar3 + 0x1c))(piVar3, 0xfffffff0);
  *(unsigned char *)((int)param_1 + 0xe0) = 0;
  param_1[0x36] = 0;
  DAT_0034b354 = 0;
  if (DAT_003386c8 != 0) {
    iVar1 = FUN_0008d994();
    param_1[0x36] = iVar1;
    if (iVar1 != 0) {
      DAT_0034b354 = iVar1;
      FUN_00103628(iVar1, 0);
      iVar1 = FUN_001fea10(0x12, 0x31, 1);
      FUN_00110a78(DAT_0034b354, iVar1, 0);
    }
  }
  FUN_002a57ec(param_1, 0, 0);
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x002a5370); experiment: try/hsttry/state_init_002a5370-9100
// near miss from lane dsn8 (unreviewed)
