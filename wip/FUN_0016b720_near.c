/* HSTMATCH module=EBOOT.BIN start=0x0016b720 end=0x0016b7d0 symbol=FUN_0016b720 lang=c99 */
/* VARIANT v10: the merge value and the underflow arm both read a NON-VOLATILE alias taken once
   ahead of the clamp (`lo`), so the arm assigns from a live register and re-reads nothing. (v8: underflow arm written FIRST (c = LO on `v < LO`, else the upper branch), so the
   compare is consumed by a zero-test branch-likely as retail's `slt at` + `beqzl at` is. (v7: v6 plus volatile bounds, so the compare re-reads the global that the merge
   value was born from - retail's second load into v1. (v6: retail's clamp with the merge value as `int` (no mask at birth) and the mask at
   the store, so the lower bound is read once into the merge and once for the compare. */
extern volatile int DAT_002e0478;
extern int DAT_002e0478_ro;
extern int DAT_002e047c_ro;
extern volatile int DAT_002e047c;
extern void *FUN_0016bb88(int a, int b);
void FUN_0016b720(int idx, int value)
{
  void *base = FUN_0016bb88(idx, value);
  unsigned short v = (unsigned short)value;
  char *p;
  int lo = DAT_002e0478_ro;
  int c = lo;
  if (v < DAT_002e0478) {
    c = lo;
  } else {
    c = DAT_002e047c_ro;
    if (v <= DAT_002e047c) c = (unsigned short)v;
  }
  if (idx < 0) p = 0;
  else if (idx >= 15) p = 0;
  else p = (char *)base + idx * 56 + 4;
  *(short *)(p + 2) = (short)c;
}
// improved by lane dsn3 (unreviewed): 34 -> 21 differing words
