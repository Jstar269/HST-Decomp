/* HSTMATCH module=EBOOT.BIN start=0x0001f0f0 end=0x0001f1fc symbol=FUN_0001f0f0 lang=c99 */
extern int DAT_00306f60;
extern int DAT_0030fe5c;
extern void FUN_0001f294(void);
extern void FUN_0001f6ec(int *, int, int *);
extern void FUN_0001f64c(int *, int *, int *, int *);
extern void FUN_00000a00(int *);

unsigned int *FUN_0001f0f0(unsigned int *param_1, short param_2)
{
  int iVar1;
  int iVar3;
  unsigned int *puVar2;
  unsigned char auStack_8[8];
  int local_c;
  unsigned char auStack_10[4];
  int local_14;
  unsigned int *local_18;

  if (param_1 != 0) {
    *param_1 = (unsigned int)&DAT_00306f60;
    DAT_0030fe5c = DAT_0030fe5c + 1;
    FUN_0001f294();
    iVar1 = param_1[7];
    if (iVar1 != 0) {
      iVar3 = *(int *)(iVar1 + 0x10);
      if (iVar3 != iVar1 + 0xc) {
        puVar2 = *(unsigned int **)(iVar3 + 8);
        while (1) {
          if (puVar2 == param_1) {
            local_c = iVar3;
            FUN_0001f6ec((int *)auStack_8, param_1[7] + 8, &local_c);
            goto LAB_0001f180;
          }
          iVar3 = *(int *)(iVar3 + 4);
          if (iVar3 == iVar1 + 0xc) goto LAB_0001f180;
          puVar2 = *(unsigned int **)(iVar3 + 8);
        }
      }
    }
LAB_0001f180:
    puVar2 = param_1 + 2;
    DAT_0030fe5c = DAT_0030fe5c + -1;
    param_1[1] = 0;
    if (((puVar2 != 0) && (puVar2 != 0)) && (puVar2 != 0)) {
      local_18 = param_1 + 3;
      local_14 = param_1[4];
      FUN_0001f64c((int *)auStack_10, (int *)puVar2, &local_14, (int *)&local_18);
    }
    if (0 < param_2) {
      FUN_00000a00((int *)param_1);
    }
  }
  return param_1;
}

/* NEAR 69/67 words, 42 differ, first +0x14. Probe 1: pseudocode loop (while puVar2 != param_1) 69/67 @48.
   Probe 2 batch: found-arm-first loop + reversed address-taken locals 69/67 @42 (promoted above);
   found-arm-first with pseudocode local order 69/67 @45.
   Retail decoded: 48-byte frame; s1=this, s0=(short)param_2; *this=&DAT_00306f60; ++DAT_0030fe5c;
   FUN_0001f294(); list walk from this[7]+0x10 against bound this[7]+0xc uses bnel so `iVar3=*(iVar3+4)`
   sits in the delay slot and is nullified on the found path; found -> local_c=iVar3 then
   FUN_0001f6ec(sp+40, this[7]+8, &local_c); then --counter, this[1]=0, the triple
   `(puVar2 != 0) && (puVar2 != 0) && (puVar2 != 0)` on this+8 (three beqz survive), then
   FUN_0001f64c(sp+32, this+8, &local_14, &local_18); `if (0 < param_2) FUN_00000a00(this)`;
   return this (seh before blezl).
   NEXT: 2 extra words at 69/67 and 42 differ spread over the second block; try (a) an explicit
   `int iVar2 = this[4];`/`this+3` pair stored as `param_1[4]` through a named local so the two
   stores land at sp+0x1c/0x18 as retail, and (b) `if (param_2 > 0)` written as `if (0 < (int)param_2)`
   with the seh hoisted -- retail sign-extends the short once into v0 before the blezl. */
// near miss from lane dsn8 (unreviewed)
