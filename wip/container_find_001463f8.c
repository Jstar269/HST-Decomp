/* HSTMATCH module=EBOOT.BIN start=0x001463f8 end=0x0014648c symbol=FUN_001463f8 lang=c99 */

extern void FUN_001465ac(int *, int *, int *, int);

int FUN_001463f8(int param_1, int param_2, int *param_3)
{
  unsigned int i = 0;
  if (*(unsigned int *)(param_1 + 0x7c0) != 0) {
    do {
      int *e = (int *)(*(int *)(param_1 + 0x7bc) + i * 12);
      if (param_2 == *(int *)(*(int *)*e + 0x10)) {
        if (param_3 == e) {
          return 1;
        }
        FUN_001465ac(param_3, (int *)*e, (int *)(*e + e[1] * 4), 0);
        return 1;
      }
      i++;
    } while (i < *(unsigned int *)(param_1 + 0x7c0));
  }
  return 0;
}
// NEAR 30/37 words, 35 differ, first +0x0: retail's frame is 32 bytes over a 37-word
// body (the element address i*12 is recomputed into v1 before the element load), while
// ours folds the base into a0 and keeps a 16-byte frame. Ruled out: the natural
// element-pointer loop above (one compile); needs the retail frame/slot grid first.
// 2026-10-31 4th pass: rewrote the element access as a 12-byte struct pointer (`Entry *e =
// (Entry *)(*(int *)(p+0x7bc)) + i;`) so the i*12 addressing is `(i*2+i)<<2` like retail's
// sll/addu/sll -- still 30/37 with a 16-byte frame, so the 32-byte frame and the recomputed
// `addu v1,a3,v0` are NOT driven by the element type. Next idea: the frame comes from the
// 4-argument FUN_001465ac call (check whether the 4th arg is passed on the stack, i.e. a 5th
// parameter), and the element address is likely re-read from a local `Entry *base` kept in a3.
// near miss from lane dsn8 (unreviewed)
