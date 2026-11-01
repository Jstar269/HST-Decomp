/* HSTMATCH module=EBOOT.BIN start=0x001b4b64 end=0x001b4c84 symbol=FUN_001b4b64 lang=c99 */
extern void FUN_00062418(int, int);
extern void FUN_001b0ef8(int *, int, int, int, int);

void FUN_001b4b64(int *param_1, int param_2, int param_3, int param_4)
{
  int *base;
  int *slot;
  int *p;
  int i;
  int count;
  int v;
  int mode;

  base = (int *)*param_1;
  slot = base + param_2 + 0x1de;
  if (*slot != 0) {
    count = 0;
    p = base + 0x1de;
    if (p != base + 0x1e2) {
      v = *p;
      while (1) {
        if (v == *slot) {
          count = count + 1;
        }
        p = p + 1;
        if (p == base + 0x1e2) break;
        v = *p;
      }
    }
    if (count < 2) {
      i = 0;
      p = base;
      do {
        if (*p == *slot) {
          v = base[i];
          if (v != 0) {
            FUN_00062418(v, 1);
            base[i] = 0;
          }
          break;
        }
        i = i + 1;
        p = p + 1;
      } while (i < 0x1da);
      *slot = 0;
    }
    else {
      *slot = 0;
    }
  }
  if (param_4 == 0) {
    mode = 2;
  }
  else {
    mode = 1;
  }
  FUN_001b0ef8(base, param_3 + 0x15b, mode, 0, 0);
  *slot = base[param_3 + 0x15b];
}
// Ruling: PROGRESS.md VERDICTS (0x001b4b64); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
