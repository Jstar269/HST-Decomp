/* HSTMATCH module=hstp_trans_main.prx start=0x00297314 end=0x00297450 symbol=FUN_00297314 lang=c99 */
/* Hand-written reconstruction: clone of the source of FUN_00285d04.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly and pseudocode; exact match verified; names and types await human review. */
typedef unsigned char uchar;

extern int DAT_003e1e04;
extern int FUN_000bf018(int, int);

int FUN_00297314(int param_1)
{
  int iVar2;
  char *p;
  char *iVar5;
  int *t3;
  int i;
  int j;
  int iVar7;
  int *piVar6;
  int bVar1;

  if (DAT_003e1e04 == 0) {
    return 0;
  }

  iVar2 = FUN_000bf018(DAT_003e1e04, DAT_003e1e04 + 0x3e);
  p = (char *)DAT_003e1e04;
  if (*(uchar *)(p + iVar2 * 7 + 0x5e) == '\0' ||
      *(uchar *)((iVar5 = (char *)(iVar2 * 0x458) + param_1) + 0xa6d) == '\0') {
    return 0;
  }

  t3 = (int *)(iVar5 + 0x958);
  for (i = 0; i < 4; i++, p += 7, param_1 += 0x458) {
    if (*(uchar *)(p + 0x5e) == '\0') {
      continue;
    }
    if (*(uchar *)(param_1 + 0xa6d) == '\0') {
      continue;
    }
    if (i == iVar2) {
      continue;
    }
    if (*(uchar *)(param_1 + 0xa48) != '\0') {
      continue;
    }

    j = 0;
    iVar7 = param_1;
    piVar6 = t3;
    do {
      if (*piVar6 != *(int *)(iVar7 + 0x958)) {
        bVar1 = 0;
        goto LAB_00285e08;
      }
      j++;
      iVar7 += 4;
      piVar6++;
    } while (j < 7);

    if (t3[7] == *(int *)(param_1 + 0x974)) {
      if (t3[8] != *(int *)(param_1 + 0x978)) {
        bVar1 = 0;
      } else {
        bVar1 = 1;
      }
    } else {
      bVar1 = 0;
    }

LAB_00285e08:
    if (!bVar1) {
      return 0;
    }
  }

  return 1;
}
