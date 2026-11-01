/* HSTMATCH module=EBOOT.BIN start=0x00152484 end=0x001526e4 symbol=FUN_00152484 lang=c99 */
extern int *DAT_0034a114;
extern int *DAT_0034a118;
extern char DAT_003402f0;
extern char DAT_002dc65c[];
extern int FUN_001c6f70(int *, int, int);
extern int FUN_001c70c0(int *);
extern void FUN_0014d044(int);
extern void FUN_0014fd94(char *, int, int);
extern int FUN_00000a90(int);
extern int *FUN_000008d8(int, int, int, int, int);
extern void FUN_00022418(int *, int *, int);
extern void FUN_000009e4(void *);
extern int FUN_00057344();

int FUN_00152484(int param_1, int param_2)
{
  int *o;
  int n;
  int i;
  int j;
  int k;
  int *dst;
  int *src;
  int local_10, local_c, local_8, local_4;

  o = *(int **)(param_2 + 0xc);
  n = *(int *)(param_1 + 8);
  if (n == -1) {
    o = DAT_0034a118;
    if (DAT_0034a118 != 0) {
      o = DAT_0034a118 + 0x38;
    }
  }
  else if (-1 < n) {
    if (n < 100) {
      o = *(int **)(*(int *)(DAT_0034a114 + 0x4c) + n * 4);
    }
    else if (n < 1000) {
      o = (int *)FUN_001c6f70(DAT_0034a114, n, 0);
    }
    else {
      o = (int *)FUN_001c70c0(DAT_0034a114);
    }
  }
  if (DAT_003402f0 != 0) {
    FUN_0014d044(param_2);
    FUN_0014fd94(DAT_002dc65c, *(int *)(param_1 + 8), *(int *)(param_1 + 0xc));
  }
  if (o != 0) {
    i = (*(int (*)(int *))(*(int *)o + 0x34))(o);
    if (i != 2) {
      i = (*(int (*)(int *))(*(int *)o + 0x34))(o);
    }
    if (i == 2 || i == 3) {
      n = *(int *)(param_1 + 0xc) + 1;
      dst = (int *)FUN_00000a90(n * 0x10 + 0x10);
      dst = (int *)FUN_000008d8((int)dst, (int)FUN_00057344, 0, 0x10, n);
      src = (int *)(*(int (*)(int *))(*(int *)o + 0x20))(o);
      dst[0] = src[0];
      dst[1] = src[1];
      dst[2] = src[2];
      dst[3] = src[3];
      k = *(int *)(param_1 + 0xc);
      j = 0;
      if (0 < k) {
        i = 0;
        src = dst;
        do {
          FUN_00022418(&local_10, dst, *(int *)(param_1 + 0x10) + i);
          j = j + 1;
          i = i + 0x10;
          src[4] = local_10;
          src[5] = local_c;
          src[6] = local_8;
          src[7] = local_4;
          k = *(int *)(param_1 + 0xc);
          src = src + 4;
        } while (j < k);
      }
      if (o == 0) {
        o = 0;
        i = 0;
      }
      else {
        o = o + -0x38;
        i = *o;
      }
      (*(void (*)(int *, int *, int, int))(i + 0xa8))(o, dst, k + 1, 0);
      if (dst != 0) {
        FUN_000009e4(dst + -4);
      }
    }
  }
  return 0;
}
// Ruling: PROGRESS.md VERDICTS (0x00152484); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
