/* HSTMATCH module=EBOOT.BIN start=0x000ec204 end=0x000ec664 symbol=FUN_000ec204 lang=c99 */
/* Hand-written: six-slot footprint sweep + four-slot effect sweep, 512-byte format buffer, 19 calls. */

extern int  FUN_00000bcc(int size);
extern int  FUN_00050ff4(int obj, char *text, int mode);
extern void FUN_00051398(int a, int b);
extern void FUN_000513a0(int a, int b);
extern void FUN_000513fc(int a, int b);
extern int  FUN_000d8c24(void *table, int a, int b);
extern void FUN_000143b0(char *buf, char *fmt, ...);
extern int  FUN_0002fe80(int bone, char *name);
extern void FUN_000ed87c(int a, int *rec, int c);

extern char *PTR_002c0830[];
extern char DAT_002d6c08[];
extern char DAT_002d6c0c[];
extern char DAT_002d6c3c[];
extern char DAT_002d6c6c[];
extern char DAT_002d6c78[];
extern char DAT_003417b8[];
extern char DAT_003386b0[];
extern char DAT_00338b08[];

void FUN_000ec204(int param_1)
{
  char buf[512];
  int local_21c;
  int local_218;
  int local_214;
  int local_210;
  int local_20c;
  char local_208;
  char local_4[4];
  char *pcVar14;
  char **ppuVar10;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar11;
  int iVar12;
  int iVar15;
  unsigned int uVar7;
  int uVar9;
  int *puVar8;

  iVar15 = 0;
  pcVar14 = local_4;
  iVar12 = 0;
  ppuVar10 = PTR_002c0830;
  iVar11 = param_1;
  do {
    if (iVar15 < 0) {
      iVar6 = FUN_00000bcc(0x48);
      if (iVar6 == 0) {
        *(int *)(iVar11 + 0x40) = 0;
        uVar9 = 0;
      }
      else {
        uVar9 = FUN_00050ff4(iVar6, *ppuVar10, 0);
        *(int *)(iVar11 + 0x40) = uVar9;
      }
      FUN_00051398(0x453bb000, uVar9);
      FUN_000513a0(*(int *)(iVar11 + 0x40), 2);
      FUN_000513fc(*(int *)(iVar11 + 0x40), 1);
    }
    else if (3 < iVar15) {
      iVar6 = FUN_00000bcc(0x48);
      if (iVar6 == 0) {
        *(int *)(iVar11 + 0x40) = 0;
        uVar9 = 0;
      }
      else {
        uVar9 = FUN_00050ff4(iVar6, *ppuVar10, 0);
        *(int *)(iVar11 + 0x40) = uVar9;
      }
      FUN_00051398(0x453bb000, uVar9);
      FUN_000513a0(*(int *)(iVar11 + 0x40), 2);
      FUN_000513fc(*(int *)(iVar11 + 0x40), 1);
    }
    else {
      iVar6 = *(int *)(DAT_003386b0 + iVar12 + 0x70);
      *pcVar14 = '\0';
      if (iVar6 == 0) {
        *(int *)(iVar11 + 0x40) = 0;
      }
      else {
        int sVar1;
        int sVar2;
        int sVar3;
        iVar6 = *(int *)(iVar6 + 0x154);
        sVar1 = *(short *)(iVar6 + 4);
        sVar2 = *(short *)(iVar6 + 6);
        sVar3 = *(short *)(iVar6 + 8);
        iVar6 = FUN_000d8c24(DAT_00338b08, 2, sVar1);
        iVar4 = FUN_000d8c24(DAT_00338b08, 3, sVar2);
        iVar5 = FUN_000d8c24(DAT_00338b08, 3, sVar3);
        FUN_000143b0(buf, DAT_002d6c08, *ppuVar10);
        if (iVar6 != 0) {
          if (((*(char *)(iVar6 + 100) << 0x1c) >> 0x1c) == 1) {
            FUN_000143b0(buf, DAT_002d6c0c, sVar1);
            *pcVar14 = '\x01';
          }
        }
        if (iVar4 != 0) {
          if (((*(char *)(iVar4 + 100) << 0x1c) >> 0x1c) == 1) {
            FUN_000143b0(buf, DAT_002d6c3c, sVar2);
            *pcVar14 = '\x01';
          }
        }
        if (iVar5 != 0) {
          if (((*(char *)(iVar5 + 100) << 0x1c) >> 0x1c) == 1) {
            FUN_000143b0(buf, DAT_002d6c3c, sVar3);
            *pcVar14 = '\x01';
          }
        }
        iVar6 = FUN_00000bcc(0x48);
        if (iVar6 == 0) {
          *(int *)(iVar11 + 0x40) = 0;
          uVar9 = 0;
        }
        else {
          uVar9 = FUN_00050ff4(iVar6, buf, 0);
          *(int *)(iVar11 + 0x40) = uVar9;
        }
        FUN_00051398(0x453bb000, uVar9);
        FUN_000513a0(*(int *)(iVar11 + 0x40), 2);
        FUN_000513fc(*(int *)(iVar11 + 0x40), 1);
      }
    }
    iVar15 = iVar15 + 1;
    pcVar14 = pcVar14 + 1;
    iVar12 = iVar12 + 4;
    iVar11 = iVar11 + 4;
    ppuVar10 = ppuVar10 + 1;
  } while (iVar15 < 6);
  iVar12 = 0;
  iVar11 = 0;
  puVar8 = (int *)DAT_003417b8;
  iVar6 = 0;
  pcVar14 = local_4;
  do {
    iVar15 = *(int *)(DAT_003386b0 + iVar11 + 0x70);
    if (iVar15 != 0) {
      iVar15 = *(int *)(iVar15 + 0x5bc);
      if (iVar15 != 0) {
        iVar15 = *(int *)(iVar15 + 0x58);
        if (iVar15 != 0) {
          local_214 = 2;
          uVar9 = -1;
          local_210 = 0;
          local_20c = 0;
          local_21c = -1;
          local_218 = -1;
          if (*(char *)((char *)puVar8 + 0x7e) == '\x01') {
            iVar4 = *(int *)(*(int *)(iVar15 + 0x8c) + 8);
            if (iVar4 != 0) {
              uVar9 = FUN_0002fe80(iVar4, DAT_002d6c6c);
            }
            iVar15 = *(int *)(*(int *)(iVar15 + 0x8c) + 8);
            local_21c = uVar9;
            if (iVar15 == 0) {
              local_218 = -1;
            }
            else {
              local_218 = FUN_0002fe80(iVar15, DAT_002d6c78);
            }
          }
          else {
            iVar4 = *(int *)(*(int *)(iVar15 + 0x8c) + 8);
            if (iVar4 != 0) {
              uVar9 = FUN_0002fe80(iVar4, DAT_002d6c78);
            }
            iVar15 = *(int *)(*(int *)(iVar15 + 0x8c) + 8);
            local_21c = uVar9;
            if (iVar15 == 0) {
              local_218 = -1;
            }
            else {
              local_218 = FUN_0002fe80(iVar15, DAT_002d6c6c);
            }
          }
          local_210 = 0x41400000;
          local_20c = 0x40c00000;
          if (*pcVar14 == '\0') {
            uVar7 = *(unsigned int *)(param_1 + 0x6c);
          }
          else {
            local_208 = 1;
            uVar7 = *(unsigned int *)(param_1 + 0x6c);
          }
          if (uVar7 < *(unsigned int *)(param_1 + 0x70)) {
            puVar8 = (int *)(*(int *)(param_1 + 0x68) + uVar7 * 0x18);
            if (puVar8 == (int *)0) {
              iVar15 = *(int *)(param_1 + 0x6c);
            }
            else {
              *puVar8 = local_21c;
              puVar8[1] = local_218;
              puVar8[2] = local_214;
              puVar8[3] = 0x41400000;
              puVar8[4] = 0x40c00000;
              *(char *)(puVar8 + 5) = local_208;
              iVar15 = *(int *)(param_1 + 0x6c);
            }
            *(int *)(param_1 + 0x6c) = iVar15 + 1;
          }
          else {
            FUN_000ed87c(param_1 + 0x68, &local_21c, 0);
          }
        }
      }
    }
    iVar12 = iVar12 + 1;
    iVar11 = iVar11 + 4;
    puVar8 = (int *)((char *)puVar8 + 0x120);
    pcVar14 = pcVar14 + 1;
  } while (iVar12 < 4);
}
// near miss from lane dsn8 (unreviewed)
