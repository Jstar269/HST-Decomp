/* HSTMATCH module=EBOOT.BIN start=0x001af0d8 end=0x001af218 symbol=FUN_001af0d8 lang=c99 */
/* Ghidra pseudocode (lbpgen.py tag a2/v0) corrected by hand: two-argument calls, p+0x10 hoisted before the clamp, loop bound re-read from the child. */
typedef unsigned char undefined;

extern int DAT_002e2138;
extern int DAT_002e213c;
extern void FUN_00103628(void *p, int n);
extern void FUN_000ffb3c(void *p, int n);
extern void FUN_001ac970(void *p);
extern int FUN_0019d474(void);
extern int FUN_001a49dc(int a, int b);

#pragma opt_strength_reduction off
void FUN_001af0d8(int p)
{
  int *child = *(int **)(p + 4);
  FUN_00103628(*(void **)child, 1);
  FUN_000ffb3c(*(void **)((char *)child + 32), 0);
  FUN_00103628(**(void ***)(p + 8), 1);
  FUN_001ac970(*(void **)(p + 0xc));
  int *q = *(int **)(p + 0x10);
  FUN_00103628(*(void **)q, 1);
  int a = FUN_0019d474();
  int b = FUN_0019d474();
  int n = FUN_001a49dc(b, *(int *)(a + 0x2c4)) - 1;
  int min = DAT_002e2138;
  if (n < DAT_002e2138) {
    min = DAT_002e2138;
  }
  else {
    min = DAT_002e213c;
    if (n < min) {
      min = n;
    }
  }
  int *r = q + 7;
  int *node = *(int **)(q + 8);
  int i = 0;
  while (i < node[5]) {
    FUN_000ffb3c(*(void **)(node[4] + i * 4), i != min);
    ++i;
  }
  **(int **)(*(int *)(r[1] + 0x10) + min * 4) = r[2];
}
// near miss from lane dsn3 (unreviewed)
