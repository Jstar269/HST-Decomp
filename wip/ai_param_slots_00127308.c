/* HSTMATCH module=EBOOT.BIN start=0x00127308 end=0x00127770 symbol=FUN_00127308 lang=c99 */
/* Hand-written: two 0x30-byte motion objects, the 0x31-slot table fill, two motion lists, two vtable disposes. */

extern void FUN_00127788(void);
extern int  FUN_00000bcc(int size);
extern void FUN_000649c0(void *p);
extern void FUN_0008c230(void *p);
extern int  FUN_000651b8(void *p, char *name, int a, int b, void *c);
extern int  FUN_000650e0(void *p);
extern int  FUN_00065554(void *p, int i, char *key);
extern int  FUN_0006569c(void *p, int i, char *key);
extern void FUN_000143b0(char *buf, char *fmt, ...);
extern void FUN_00055eac(void *dst, char *name, int a);
extern int  FUN_00014934(int a, char *b);
extern int  FUN_00000a90(int size);
extern int  FUN_000008d8(int a, void *b, void *c, int d, int e);
extern void FUN_00055dd0(void);
extern void FUN_00055e48(void);

extern char DAT_003070bc[];
extern char DAT_002da850[];
extern char DAT_002da874[];
extern char DAT_002da878[];
extern char DAT_0033b4b0[];
extern char DAT_0033b4a8[];
extern char DAT_0033b4ac[];
extern char DAT_002da89c[];
extern char DAT_002da8a8[];
extern char DAT_002da8b4[];
extern char DAT_002da8d0[];
extern char DAT_002da8d8[];
extern char DAT_002da8e0[];
extern char DAT_002da8e8[];
extern char *PTR_002c13a8[];
extern char LAB_00127770[];

void FUN_00127308(int param_1, unsigned int param_2)
{
  char auStack_100[256];
  unsigned int uVar12;
  void *uVar6;
  int iVar5;
  int iVar7;
  int iVar10;
  int iVar11;
  int *piVar3;
  int *piVar4;
  int *puVar8;
  char **ppuVar9;
  int bVar1;
  char uVar2;

  FUN_00127788();
  uVar12 = 1 << (param_2 & 0x1f);
  piVar3 = (int *)FUN_00000bcc(0x30);
  if (piVar3 != (int *)0) {
    FUN_000649c0(piVar3);
    *piVar3 = (int)DAT_003070bc;
    FUN_0008c230(piVar3 + 4);
    FUN_0008c230(piVar3 + 8);
  }
  piVar4 = (int *)FUN_00000bcc(0x30);
  if (piVar4 != (int *)0) {
    FUN_000649c0(piVar4);
    *piVar4 = (int)DAT_003070bc;
    FUN_0008c230(piVar4 + 4);
    FUN_0008c230(piVar4 + 8);
  }
  iVar5 = FUN_000651b8(piVar3, DAT_002da850, 0, 1, DAT_002da874);
  if (iVar5 != 0) {
    iVar5 = FUN_000651b8(piVar4, DAT_002da878, 0, 1, DAT_002da874);
    if (iVar5 != 0) {
      iVar5 = 0;
      puVar8 = (int *)DAT_0033b4b0;
      do {
        *puVar8 = -1;
        puVar8[1] = -1;
        puVar8[2] = -1;
        puVar8[3] = -1;
        puVar8[4] = -1;
        puVar8[5] = -1;
        iVar5 = iVar5 + 7;
        puVar8[6] = -1;
        puVar8 = puVar8 + 7;
      } while (iVar5 < 0x31);
      uVar6 = (void *)FUN_00000a90(0x3210);
      *(int *)DAT_0033b4a8 = FUN_000008d8((int)uVar6, FUN_00055dd0, FUN_00055e48, 0x10, 800);
      uVar6 = (void *)FUN_00000a90(0x970);
      *(int *)DAT_0033b4ac = FUN_000008d8((int)uVar6, LAB_00127770, 0, 3, 800);
      iVar10 = 1;
      iVar5 = FUN_000650e0(piVar3);
      if (1 < iVar5) {
        iVar11 = 3;
        iVar5 = param_1 + 1;
        do {
          bVar1 = (uVar12 & (int)*(char *)(iVar5 - 1)) != 0;
          if (!bVar1) {
            iVar5 = FUN_00065554(piVar3, iVar10, DAT_002da89c);
            bVar1 = 0 < iVar5;
          }
          if (bVar1) {
            uVar6 = (void *)FUN_0006569c(piVar3, iVar10, DAT_002da8a8);
            FUN_000143b0(auStack_100, DAT_002da8b4, uVar6);
            FUN_00055eac(DAT_0033b4a8 + (iVar10 - 1) * 0x10, auStack_100, 0);
            uVar2 = FUN_00065554(piVar3, iVar10, DAT_002da8d0);
            *(char *)(DAT_0033b4ac + iVar11 - 3) = uVar2;
            uVar2 = FUN_00065554(piVar3, iVar10, DAT_002da8d8);
            *(char *)(DAT_0033b4ac + iVar11 - 2) = uVar2;
            uVar2 = FUN_00065554(piVar3, iVar10, DAT_002da8e0);
            iVar5 = 0;
            ppuVar9 = PTR_002c13a8;
            *(char *)(DAT_0033b4ac + iVar11 - 1) = uVar2;
            do {
              iVar7 = FUN_00014934((int)uVar6, *ppuVar9);
              if (iVar7 == 0) {
                *(int *)(DAT_0033b4b0 + iVar5 * 4) = iVar10 - 1;
                break;
              }
              iVar5 = iVar5 + 1;
              ppuVar9 = ppuVar9 + 1;
            } while (iVar5 < 0x31);
          }
          iVar11 = iVar11 + 3;
          iVar10 = iVar10 + 1;
          iVar5 = FUN_000650e0(piVar3);
          if (iVar5 <= iVar10) break;
          iVar5 = param_1 + iVar10;
        } while (1);
      }
      iVar10 = 1;
      iVar5 = FUN_000650e0(piVar4);
      if (1 < iVar5) {
        do {
          iVar5 = FUN_00065554(piVar4, iVar10, DAT_002da8e8);
          iVar5 = iVar5 + 200;
          if ((uVar12 & (int)*(char *)(param_1 + iVar5)) != 0) {
            uVar6 = (void *)FUN_0006569c(piVar4, iVar10, DAT_002da8a8);
            FUN_000143b0(auStack_100, DAT_002da8b4, uVar6);
            FUN_00055eac(DAT_0033b4a8 + iVar5 * 0x10, auStack_100, 0);
            uVar2 = FUN_00065554(piVar4, iVar10, DAT_002da8d0);
            *(char *)(DAT_0033b4ac + iVar5 * 3) = uVar2;
            uVar2 = FUN_00065554(piVar4, iVar10, DAT_002da8d8);
            *(char *)(DAT_0033b4ac + iVar5 * 3 + 1) = uVar2;
          }
          iVar10 = iVar10 + 1;
          iVar5 = FUN_000650e0(piVar4);
        } while (iVar10 < iVar5);
      }
      if (piVar4 != (int *)0) {
        if (piVar4 != (int *)0) {
          (*(void (**)(int *, int))(*(int *)piVar4 + 8))(piVar4, 1);
        }
      }
      if (piVar3 != (int *)0) {
        if (piVar3 != (int *)0) {
          (*(void (**)(int *, int))(*(int *)piVar3 + 8))(piVar3, 1);
        }
      }
    }
  }
}
// near miss from lane dsn8 (unreviewed)
