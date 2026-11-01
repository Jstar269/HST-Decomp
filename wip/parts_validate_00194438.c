/* HSTMATCH module=EBOOT.BIN start=0x00194438 end=0x0019485c symbol=FUN_00194438 lang=c99 */
/* Hand-written from the pseudocode; six-part validator with a 128-byte message buffer and a 24-byte part table. */

extern void FUN_000114c0(void *dst, int value, int size);
extern void FUN_0001cb88(char *buf, ...);
extern void FUN_0001ca58(char *buf, char *fmt, ...);
extern int  FUN_002041e8(int index);
extern int  FUN_00204220(int index);
extern int  FUN_00092720(int a, int b, int c, int d, int e);
extern int  FUN_00092790(int a, int b, int c, int d, int e);
extern void FUN_00092c38(int a, int b);
extern void FUN_00092d10(int a, int b);
extern void FUN_0010a6f0(char *buf, int flag);
extern void FUN_000143b0(char *buf, char *fmt, ...);
extern void FUN_000931ec(int a, int b, char *buf, int flag);
extern char DAT_002e1540[];
extern char DAT_002e155c[];
extern char DAT_002e1560[];
extern char DAT_002e1588[];

void FUN_00194438(char *param_1, int param_2, short *param_3, int param_4, char param_5)
{
  char buf[128];
  char flags[8];
  char tail[4];
  int nums[6];
  int *pi;
  char *pc;
  int i;
  int kind;
  int val;
  int idx;
  int res;
  int bVar16;
  char cVar5;
  int uVar10;
  unsigned char uVar1;
  unsigned uVar19;

  for (i = 0; i < 6; i++) {
    nums[i] = 0;
  }
  nums[0] = (int)param_3[0];
  nums[1] = (int)param_3[1];
  nums[2] = (int)param_3[2];
  nums[3] = (int)param_3[3];
  nums[4] = (int)param_3[4];
  nums[5] = (int)param_3[5];
  for (i = 0; i < 6; i++) {
    flags[i] = 0;
  }
  uVar1 = *(unsigned char *)((char *)param_3 + 0xd);
  uVar19 = *(unsigned char *)(param_3 + 6);
  FUN_000114c0((void *)0x341750, 0, 0x18);
  i = 0;
  pi = nums;
  do {
    kind = FUN_002041e8(i);
    val = *pi;
    uVar10 = FUN_00204220(kind);
    if (val != -1) {
      if (-1 < val) {
        if (kind == 5 && param_4 == 0) goto LAB_001946c8;
        idx = val;
        if (kind == 5) {
          idx = val / 10;
        }
        if ((unsigned)(kind - 3) < 2 && idx == 0) {
          FUN_00092c38(param_2, kind);
        }
        res = FUN_00092720(param_2, kind, idx, uVar1, 0);
        if (res == 0) {
          if (param_4 != 0) {
            FUN_0001ca58((char *)0x30ebf4, DAT_002e1540, uVar10, uVar10, idx, DAT_002e155c);
          }
          cVar5 = FUN_00092790(param_2, kind, idx, uVar1, 0);
          flags[kind] = cVar5 != 1;
          if (kind == 2) {
            *param_1 = cVar5;
          }
          if (cVar5 != 0) {
            *(int *)(kind * 4 + 0x341750) = val;
          }
          if (param_4 != 0) {
            FUN_0001cb88((char *)0x30ebf4);
          }
        }
      }
      if (kind == 2 && uVar19 != *(unsigned *)(param_2 + 0x68)) {
        if (param_4 != 0) {
          FUN_0001ca58((char *)0x30ebf4, DAT_002e1560, uVar19);
        }
        FUN_00092d10(param_2, uVar19);
        if (param_4 != 0) {
          FUN_0001cb88((char *)0x30ebf4);
        }
      }
    }
LAB_001946c8:
    i = i + 1;
    pi = pi + 1;
  } while (i < 6);
  bVar16 = 0;
  i = 0;
  pc = flags;
  while (*pc == 0) {
    i = i + 1;
    pc = pc + 1;
    if (5 < i) goto LAB_00194728;
  }
  i = 0;
  do {
    FUN_00092c38(param_2, i);
    i = i + 1;
  } while (i < 6);
  bVar16 = 1;
LAB_00194728:
  if (bVar16) {
    i = 0;
    pi = nums;
    do {
      kind = FUN_002041e8(i);
      uVar10 = FUN_00204220(kind);
      if (flags[kind] != 0) {
        FUN_000143b0(buf, DAT_002e1588, (int)param_3 + 0xf, uVar10, *pi);
        FUN_0010a6f0(buf, 0);
      }
      if (param_4 != 0) {
        FUN_0001ca58((char *)0x30ebf4, DAT_002e1540, uVar10, uVar10, 0, DAT_002e155c);
      }
      FUN_00092790(param_2, kind, 0, uVar1, 0);
      *(int *)(kind * 4 + 0x341750) = 0;
      if (param_4 != 0) {
        FUN_0001cb88((char *)0x30ebf4);
      }
      i = i + 1;
      pi = pi + 1;
    } while (i < 6);
  }
  for (i = 0; i < 2; i++) {
    tail[i] = 0;
  }
  bVar16 = 0;
  if (param_5 != 0) {
    bVar16 = (char)param_3[0x3e] == 2;
  }
  FUN_000931ec(param_2, 1, tail, bVar16);
}

/* PARKED 2026-11-01 (batch 1): NEAR 259/265 words, 252 differ, first at +0x28.
 * FRAME LANDS: 240 (`27bdff10` identical) and the saves match word for word through
 *   ra@44, s8@40, s7@36, s6@32, s5@28, s4@24 - the first difference is the s0 save, so the body
 *   needs one more (or one fewer) callee-saved register than this draft.
 * The two count loops are written from the STORE counts (6 ints = 24 bytes, 6 chars, 2 chars), not
 *   from Ghidra's code-pointer typing; that produced a matching prologue, so the reading was right.
 * NEXT LEVER: the six-iteration body keeps `kind`, `val`, `idx` and the FUN_00204220 result live at
 *   once. Retail's first divergence is at the s0 save, so try hoisting uVar10 (the FUN_00204220
 *   string) OUT of the loop into a local array read by index, or splitting the `val != -1` test into
 *   two separate ifs the way the pseudo nests them.
 */

/* BATCH 2 (2026-11-01): no-pi, no-pc and no-both all measure IDENTICAL - 259/265 words, 252 differ.
 * So the two pointer walks (`pi` over nums, `pc` over flags) are already strength-reduced by MWCC;
 * replacing them with indexed loads changes nothing. The extra callee-saved value is NOT the loop
 * pointers, so identifier/pointer hygiene is a dead end here (same lesson as 0x000c0fb8).
 * NEXT LEVER: the first divergence is the s0 save, i.e. count, not order. Retail keeps one value
 * live that our spelling drops somewhere in the six-iteration body - try inlining FUN_00204220's
 * result into each of its three uses (it is currently a loop-carried local), or splitting
 * `if (val != -1) { if (-1 < val) {` into two statements at the same level.
 */

/* BATCH 3 (2026-11-01, six variants, all measured): none beat the base.
 *   flatten (val != -1 && -1 < val)                    253 differ / 259 words
 *   flatten-nopc                                       253 / 259
 *   inline  (FUN_00204220 at every use site)           254 / 265
 *   inline-flatten, inline-flatten-nopc, flatten-inline 254 / 265
 * BASE STAYS BEST at 252 differ / 259 words. Inlining the string lookup costs 6 words and 2 differ -
 * so the value IS loop-carried in retail after all, and flattening the nested test is a small loss.
 * The s0 save divergence is neither the pointer walks nor the string local. Both batches are now
 * spent on this item.
 * NEXT LEVER (untried): the `do { } while (i < 6)` loop form. Retail's loop is entered from a guard
 * that also feeds the `if (kind == 5 && param_4 == 0) goto` tail; try a `for (i = 0; i < 6; i++)`
 * form, and try moving the `i == 6` bound test to the top (`while (i < 6) { ... }`) so the guard is
 * a pre-test rather than the pseudo's do/while.
 */
// near miss from lane dsn8 (unreviewed)
