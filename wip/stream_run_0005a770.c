/* HSTMATCH module=EBOOT.BIN start=0x0005a770 end=0x0005a8ec symbol=FUN_0005a770 lang=c99 */
extern int DAT_003313c4;
extern int DAT_0032b180;
extern char DAT_00311170[];
extern void FUN_00050824(char *, int);
extern void FUN_0005185c(int);
extern void FUN_00051490(int, int, int, int, int, int, int);
extern void FUN_00052bf4(int, int *, int, int, int, int, int);

void FUN_0005a770(int *param_1, int param_2, int param_3, int *param_4)
{
  int iVar1;

  if (0 < param_1[0x17]) {
    if (param_3 == 0) {
      param_3 = DAT_003313c4;
    }
    if (DAT_0032b180 != param_3) {
      FUN_0005185c(param_2);
      FUN_00050824(DAT_00311170, param_3);
      FUN_00051490(param_2, 0, 0, 0x1e0, 0x110, 0, 0);
    }
    else {
      FUN_00050824(DAT_00311170, param_3);
    }
    if (param_4 == (int *)0x0) {
      param_4 = param_1 + 4;
    }
    iVar1 = (*(int (**)(int *))(*param_1 + 0xc))(param_1);
    if (iVar1 == 1) {
      FUN_00052bf4(param_2, param_4, 1, param_1[0x18], param_1[0x17], 0, param_1[param_1[0x15] + 0x19]);
    }
    else if (iVar1 == 3) {
      FUN_00052bf4(param_2, param_4, 3, param_1[0x18], param_1[0x17], 0, param_1[param_1[0x15] + 0x19]);
    }
    else if (iVar1 == 4) {
      FUN_00052bf4(param_2, param_4, 4, param_1[0x18], param_1[0x17], 0, param_1[param_1[0x15] + 0x19]);
    }
  }
  return;
}

/* NEAR 94/95 words, 53 differ, first +0x48. Probe 1 = pseudocode shape (if/else then-first) 95/95 @60;
   probe 2: else-first arms 94/95 @53 (promoted), same plus case bodies written 4/3/1 94/95 @54.
   Retail: 32-byte frame, s2=param_2, s1=param_3, s0=param_4, s3=param_1; blez p[0x17]; bnez param_3 else
   load DAT_003313c4; beql DAT_0032b180==param_3 with `lui a0,0x31` in the delay (single FUN_00050824 arm
   out of line at 0x5a800); else arm: FUN_0005185c(param_2), FUN_00050824, FUN_00051490(param_2,0,0,0x1e0,
   0x110,0,0) with t1=0 and t2=0 set explicitly; `bnezl s0` with `lw t9,0(s3)` in the delay then
   `addiu s0,s3,16` (param_4 = param_1+4); t9=*(t9+0xc); jalr with move a0,s3; chain li 1/3/4 with beql and
   `lw v0,84(s3)` delays; bodies laid out 4,3,1, each a3=p[0x18], t0=p[0x17], t2=p[p[0x15]+0x19], t1=0.
   GAP: allocation (1 word short). NEXT: the missing word is the vtable-load/param_4-test pairing -- retail
   fills `bnezl s0` with `lw t9,0(s3)`; try loading the vtable before the param_4 test
   (`iVar1 = *param_1; if (param_4 == 0) param_4 = param_1 + 4;`) so the load is a free delay filler. */
// near miss from lane dsn8 (unreviewed)
