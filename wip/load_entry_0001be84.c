/* HSTMATCH module=EBOOT.BIN start=0x0001be84 end=0x0001bf64 symbol=FUN_0001be84 lang=c99 */
extern int DAT_002cfae4;
extern int DAT_0030ebd0;
extern int DAT_0030ebcc;
extern int DAT_0030ebdc;
extern int DAT_0030ebe0;
extern int DAT_0030ebe4;
extern char DAT_002cfc04[];
extern int FUN_0001b788(int, int);
extern int FUN_00010184(int, unsigned int);
extern void FUN_0001bc6c(void);
extern void FUN_0001b918(char *);

int FUN_0001be84(int param_1, int param_2)
{
  int iVar1;
  unsigned int uVar2;
  int p;

  if (DAT_002cfae4 != 3) {
    FUN_0001b918(DAT_002cfc04);
    for (;;) {
    }
  }
  DAT_0030ebd0 = 0;
  iVar1 = FUN_0001b788(DAT_0030ebcc, param_1);
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = (*(int *)(iVar1 + 0xc) + 0xf) & 0xfffffff0;
  p = param_2;
  if (p == 0) {
    p = FUN_00010184(0x40, uVar2 << 0xb);
  }
  if (p == 0) {
    return 0;
  }
  DAT_0030ebdc = *(int *)(iVar1 + 4);
  DAT_0030ebe0 = uVar2;
  DAT_0030ebe4 = p;
  FUN_0001bc6c();
  return p;
}

/* NEAR 52/56 words, 54 differ, first +0x0. Two probes: (1) positive-test/fused-return draft 53/56 @55;
   (2) separate result var + early returns 52/56 @54 (promoted above; A==B byte-identical).
   Retail decoded: s3=p2, s1=lookup result, s0=(field_c+15)&~15, s2=working ptr = s3; bnez s3 skips
   the 0x10184(0x40,size<<11) call; both paths fall to `beqz s2 -> return 0`; stores ebdc/ebe0/ebe4 then
   FUN_0001bc6c() and return s2. Ours merges param_2 and p into one home (3 s-regs, 16-byte frame vs
   retail's 4 s-regs, 32-byte frame) and loses 4 words in the two return copies.
   NEXT: force the second home with the goto-result recipe that closed 0x0004eac4 -- `p = param_2;
   if (p == 0) { p = alloc(...); if (p == 0) goto fail; } ... fail: return p;` -- and keep the
   parameter live by testing `param_2 == 0` again after the call. */
// near miss from lane dsn8 (unreviewed)
