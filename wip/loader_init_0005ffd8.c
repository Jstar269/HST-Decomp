/* HSTMATCH module=EBOOT.BIN start=0x0005ffd8 end=0x00060138 symbol=FUN_0005ffd8 lang=c99 */
extern int DAT_00307070;
extern unsigned char DAT_0031136c;
extern int FUN_0004f52c(char *, int, int, int);
extern char DAT_00311170[];
extern void FUN_00054510(int *, int *, char);
extern int FUN_0004ff14(char *, int, int);
extern int FUN_0004ff6c(char *, int, int);
extern int FUN_00000a90(int);

unsigned int *FUN_0005ffd8(unsigned int *param_1, int *param_2, int param_3, int param_4, int param_5, int param_6, int param_7)
{
  int iVar1;
  int uVar2;
  int uVar3;
  int local_4;

  *param_1 = (unsigned int)&DAT_00307070;
  param_1[10] = 0;
  param_1[0xb] = 0;
  param_1[0xc] = 0;
  *(unsigned char *)(param_1 + 1) = 0;
  param_1[2] = param_5;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = param_3;
  param_1[6] = param_4;
  param_1[7] = 0x1e0;
  param_1[8] = 0x110;
  param_1[9] = param_6;
  param_1[0xe] = param_7;
  iVar1 = *param_2;
  while (iVar1 != 0) {
    DAT_0031136c = 0;
    local_4 = FUN_0004f52c(DAT_00311170, *param_2, 0, 0);
    DAT_0031136c = 1;
    if (param_1[0xb] < param_1[0xc]) {
      iVar1 = param_1[0xb] + 1;
      param_1[0xb] = iVar1;
      *(int *)(param_1[10] + iVar1 * 4 - 4) = local_4;
    }
    else {
      FUN_00054510((int *)(param_1 + 10), &local_4, (char)0);
    }
    param_2 = param_2 + 1;
    iVar1 = *param_2;
  }
  uVar3 = *(int *)param_1[10];
  uVar2 = FUN_0004ff14(DAT_00311170, uVar3, 0);
  param_1[7] = uVar2;
  iVar1 = FUN_0004ff6c(DAT_00311170, uVar3, 0);
  param_1[8] = iVar1;
  uVar2 = FUN_00000a90(param_1[9] * param_1[7] * iVar1);
  param_1[0xd] = uVar2;
  return param_1;
}

/* NEAR 87/88 words, 41 differ, first +0x5c. Probe 1 = pseudocode with `0x311170` as an integer literal
   87/88 @42; probe 2: `extern char DAT_00311170[]` + pointer-typed callees 87/88 @41 (promoted; same as
   the while(*param_2) form, byte-identical).
   Retail: 32-byte frame, s1=param_1, s0=param_2 list; seven params a0,a1,a2,a3,t0,t1,t2 (param_7 is in
   t2 -> store at 0x38); field stores in order 0,40,44,48,byte 4,t0@8,12,16,a2@20,a3@24,0x1e0@28,
   0x110@32,t1@36,t2@56; loop: sb 0,0x31136c; local_4=FUN_0004f52c(0x311170,*param_2,0,0); sb 1;
   sltu p[0xb],p[0xc] -> p[0xb]++; *(p[10]+i*4-4)=local_4 else FUN_00054510(p+10,&local_4,(char)0)
   with `seb a2`; then uVar3=p[10][0]; p[7]=FUN_0004ff14(...); p[8]=FUN_0004ff6c(...);
   p[0xd]=FUN_00000a90(p[9]*(p[7]*p[8])) via two mults; return param_1.
   GAP: allocation (1 word short). NEXT: the remaining diff is the loop entry -- retail puts
   `beqz v0,exit` before `move s1,a0` (copy in the delay slot), ours emits the copy first; try declaring
   the return pointer as a local assigned at entry (`unsigned int *r = param_1;`) used only at `return`,
   which gives the allocator a second home to fill the delay with. */
// near miss from lane dsn8 (unreviewed)
