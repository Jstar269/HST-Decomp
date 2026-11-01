/* HSTMATCH module=EBOOT.BIN start=0x0005548c end=0x000555b4 symbol=FUN_0005548c lang=c99 */
extern int FUN_0002fe80(int, int);
extern void FUN_00055240(float *, int);
extern float DAT_002bc410;
extern float DAT_002bc414;
extern float DAT_002bc418;
extern float DAT_002bc41c;
extern float DAT_002bc420;
extern float DAT_002bc424;
extern float DAT_002bc428;
extern float DAT_002bc42c;
extern float DAT_002bc430;
extern float DAT_002bc434;
extern float DAT_002bc438;
extern float DAT_002bc43c;
extern float DAT_002bc440;
extern float DAT_002bc444;
extern float DAT_002bc448;
extern float DAT_002bc44c;

void FUN_0005548c(float *param_1, int param_2, int param_3)
{
  int iVar1;
  int iVar2;

  if (*(int *)(param_2 + 0x8c) != 0) {
    iVar1 = *(int *)(*(int *)(param_2 + 0x8c) + 8);
    if (iVar1 == 0) {
      iVar2 = -1;
    }
    else {
      iVar2 = FUN_0002fe80(iVar1, param_3);
    }
    if (-1 < iVar2) {
      FUN_00055240(param_1, param_2);
      return;
    }
  }
  param_1[0] = DAT_002bc410;
  param_1[1] = DAT_002bc414;
  param_1[2] = DAT_002bc418;
  param_1[3] = DAT_002bc41c;
  param_1[4] = DAT_002bc420;
  param_1[5] = DAT_002bc424;
  param_1[6] = DAT_002bc428;
  param_1[7] = DAT_002bc42c;
  param_1[8] = DAT_002bc430;
  param_1[9] = DAT_002bc434;
  param_1[10] = DAT_002bc438;
  param_1[11] = DAT_002bc43c;
  param_1[12] = DAT_002bc440;
  param_1[13] = DAT_002bc444;
  param_1[14] = DAT_002bc448;
  param_1[15] = DAT_002bc44c;
  return;
}

/* NEAR 77/74 words, 69 differ, first +0x18. Probe 1: 16-float struct assignment 40/74 @65 -- MWCC collapsed
   the copy to a block move (34 words short), so that differ counts a structurally wrong body. Probe 2:
   16 distinct `extern float` globals and `extern float DAT_002bc410[16]` + constant indices, byte-identical
   77/74 @69 (promoted A: faithful copy shape, 3 words over). Retail: s0=param_1, s1=param_2;
   lw v1,0x8c(param_2); if zero -> copy; a0=*(v1+8); beqzl a0 with `li a2,-1` in delay; else
   FUN_0002fe80(a0, param_3); bltz a2 -> copy; FUN_00055240(s0,s1) then return; copy is 16 x (lui + lwc1
   + swc1) from 0x2bc410..0x2bc44c -- one `lui 0x2c` per element, no shared base register.
   NEXT: 3 extra words + 69 differ are in the early block and the address materialisation; try (a) the
   `iVar1 == 0` arm inverted (`if (iVar1 != 0) iVar2 = FUN(...); else iVar2 = -1;`) to get retail's
   beqzl polarity, and (b) copy through a local `float *src = &DAT_002bc410;` so MWCC must emit one base
   plus 16-bit offsets instead of 16 independent `lui 0x2c` (retail's per-element lui is the odd part). */
// near miss from lane dsn8 (unreviewed)
