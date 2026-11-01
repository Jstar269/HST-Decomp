/* HSTMATCH module=EBOOT.BIN start=0x0004fe04 end=0x0004ff14 symbol=FUN_0004fe04 lang=c99 */
extern short DAT_00019b84[];
extern int DAT_0001a004;
extern char DAT_002d0e60[];
extern void FUN_00050d8c(int, int);
extern void FUN_00049194(void);
extern int FUN_00033998(int);
extern void FUN_0004ea98(char *, int);
extern void FUN_000009e4(int);
extern void FUN_000491cc(void);

void FUN_0004fe04(int param_1, int param_2)
{
  int *p;
  short sVar1;
  int iVar2;

  if ((param_2 < 0) || (0x240 <= param_2)) {
    return;
  }
  if (1) {
    sVar1 = *(short *)((char *)DAT_00019b84 + param_1 + param_2 * 2);
    *(short *)((char *)DAT_00019b84 + param_1 + param_2 * 2) = sVar1 - 1;
    if ((short)(sVar1 - 1) == 0) {
      FUN_00050d8c(param_1, 1);
      p = (int *)(param_1 + param_2 * 0x10 + 0x204);
      FUN_00049194();
      iVar2 = FUN_00033998(param_1 + param_2 * 0xa0 + 0x2604);
      if (iVar2 != 0) {
        FUN_0004ea98(DAT_002d0e60, iVar2);
      }
      *(int *)((char *)&DAT_0001a004 + param_1) = *(int *)((char *)&DAT_0001a004 + param_1) + -1;
      FUN_000009e4(p[0]);
      p[0] = param_1 + param_2 * 6 + 0x18e04;
      p[1] = 0;
      p[2] = 0;
      p[3] = 0;
      FUN_000491cc();
    }
  }
  return;
}

/* NEAR 68/68 words, 43 differ, first +0x14. Probe 1 (pseudocode shape, index-style stores) 66/68 @59.
   Probe 2 batch: C merged guard + `int *p = (int*)(param_1 + param_2*0x10 + 0x204)` 68/68 @43 (promoted,
   verbatim: `if ((param_2 < 0) || (0x240 <= param_2)) return; if (1) { ... }` -- the `if (1)` folds away);
   A early `if (param_2 < 0) return;` + p base 67/68 @45; B upper guard first with `-1 < param_2` 67/68 @45.
   Retail decoded: bltz s1 exit; slti 576 / beqzl exit; two `lh/sh` against DAT_00019b84 materialised as
   `lui v1,0x2; addu v1,a2,v1; lh -25724(v1)` with a2 = param_1 + param_2*2; seh + bnez; then
   s0 = param_1 + param_2*0x10 + 0x204 in the FUN_00049194 jal delay slot; FUN_00033998(page+param_2*0xa0+0x2604);
   string 0x2d0e60 as a0, result a1; counter at param_1+0x1a004; FUN_000009e4(p[0]); p[0]=param_1+param_2*6+0x18e04;
   p[1..3]=0; FUN_000491cc() with p[3]=0 in its delay slot.
   NEXT: the remaining 43 differ start at the DAT_00019b84 access -- retail folds the -25724 into the lh/sh
   offset (base 0x20000 + index) where ours materialises the symbol first. Try declaring the table as
   `extern short DAT_00019b84[];` indexed by a **char-offset** local (`short *q = (short *)((char *)DAT_00019b84 + param_1); q[param_2]`),
   and `if ((short)(sVar1 - 1) == 0)` written as a `switch` on the decremented value. */
// near miss from lane dsn8 (unreviewed)
