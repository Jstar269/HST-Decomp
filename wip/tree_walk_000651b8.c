/* HSTMATCH module=EBOOT.BIN start=0x000651b8 end=0x000652e8 symbol=FUN_000651b8 lang=c99 */
extern int FUN_00064bf4(int, int, int, int);
extern void FUN_00066280(void *, int, void *);

typedef int (*VF)(void *);

int FUN_000651b8(int param_1, int param_2, int param_3, int param_4, int param_5)
{
  int iVar1;
  int iVar2;
  int *piVar4;
  int local_20;
  int local_1c;
  unsigned char auStack_18[8];
  int local_10;
  int local_c;
  unsigned char auStack_8[8];

  iVar1 = FUN_00064bf4(param_1, param_2, 0, param_5);
  if (iVar1 == 0) {
    iVar2 = 0;
  }
  else {
    if ((param_3 != 0) && (iVar1 = 0, 0 < *(int *)(param_1 + 8))) {
      while (1) {
        local_10 = (*(VF *)(**(int **)(*(int *)(param_1 + 4) + (iVar1 + iVar1 * 2) * 4) + 0x14))
                   (*(void **)(*(int *)(param_1 + 4) + (iVar1 + iVar1 * 2) * 4));
        local_c = iVar1;
        FUN_00066280(auStack_8, param_1 + 0x10, &local_10);
        iVar1 = iVar1 + 1;
        if (*(int *)(param_1 + 8) <= iVar1) break;
      }
    }
    if (param_4 == 0) {
      iVar2 = 1;
    }
    else {
      iVar1 = 0;
      if (0 < *(int *)(*(int *)(param_1 + 4) + 4)) {
        piVar4 = *(int **)(param_1 + 4);
        while (1) {
          local_20 = (*(VF *)(**(int **)(*piVar4 + iVar1 * 4) + 0x14))
                     (*(void **)(*piVar4 + iVar1 * 4));
          local_1c = iVar1;
          FUN_00066280(auStack_18, param_1 + 0x20, &local_20);
          iVar1 = iVar1 + 1;
          if (*(int *)(*(int *)(param_1 + 4) + 4) <= iVar1) break;
          piVar4 = *(int **)(param_1 + 4);
        }
      }
      iVar2 = 1;
    }
  }
  return iVar2;
}

/* NEAR 77/76 words, 72 differ, first +0x0. Probe 1 (ptr/obj temps + while(1)) 79/76 @74 with a 64-byte
   frame and 4 saved regs; probe 2 batch: inline double-deref 77/76 @72 (promoted), `iVar1*0xc` stride
   75/76 @76. Retail: 48-byte frame, s0=param_3, s1=param_4, s2=param_1; `move a3,t0` proves param_5 is
   the 5th integer arg in t0 and THIS compiler does the same (first diff is the frame, not the ABI).
   Loop1: i*12 stride over `*(param_1+4)`, double deref -> a0, `lw t9,0(a0); lw t9,0x14(t9); jalr t9`
   with a nop delay slot; local_10=result; local_c=i; FUN_00066280(sp+40, param_1+0x10, sp+32);
   count reloaded from param_1+8 each iteration and `bnezl` back with `sll v0,i,1` in the delay.
   Loop2: `pi = *(param_1+4); arr=*pi; count=pi[1]`; elements 4-byte; same virtual call shape; writes
   sp+16/sp+20 and calls FUN_00066280(sp+24, param_1+0x20, sp+16).
   NEXT: our 64-byte frame/4th saved reg is the whole first-diff; drop the `piVar4` local and index
   `*(int *)(param_1 + 4)` inline at every use (retail reloads `lw v0,4(s2)` at the loop bottom), and
   declare `iVar1` as the single loop counter so no second value stays live across the two calls. */
// near miss from lane dsn8 (unreviewed)
