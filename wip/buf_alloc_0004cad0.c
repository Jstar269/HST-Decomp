/* HSTMATCH module=EBOOT.BIN start=0x0004cad0 end=0x0004cc18 symbol=FUN_0004cad0 lang=c99 */
extern char DAT_002d0950[];
extern int FUN_0004ccb8(int, int, int);
extern int FUN_0004d084(int);
extern unsigned int FUN_00010184(int, unsigned int);
extern int FUN_00014d64(int, char *);
extern void FUN_000104e0(unsigned int);
extern int FUN_0004d028(int, unsigned int, int);
extern void FUN_0004cfd0(int);

unsigned int FUN_0004cad0(int param_1, int *param_2, int param_3, int param_4)
{
  int iVar1;
  unsigned int uVar2;
  int iVar3;
  unsigned int uVar4;
  int iVar5;
  unsigned int uVar6;

  uVar6 = 0;
  iVar5 = 0;
  iVar1 = FUN_0004ccb8(param_1, 1, param_4);
  if (iVar1 < 0) {
    uVar6 = 0;
  }
  else {
    iVar5 = FUN_0004d084(iVar1);
    if (-1 < iVar5) {
      uVar4 = -param_3 & iVar5 + param_3 + -1;
      uVar2 = FUN_00010184(param_3, uVar4);
      iVar3 = FUN_00014d64(param_1, DAT_002d0950);
      uVar6 = uVar2;
      if ((iVar3 != 0) && ((uVar2 & 0xff000000) != (uVar2 + iVar5 & 0xff000000))) {
        uVar6 = FUN_00010184(param_3, uVar4);
        FUN_000104e0(uVar2);
      }
      if ((uVar6 != 0) && (iVar3 = FUN_0004d028(iVar1, uVar6, iVar5), iVar5 == iVar3))
        goto LAB_0004cbd0;
    }
  }
  FUN_000104e0(uVar6);
  uVar6 = 0;
LAB_0004cbd0:
  if (-1 < iVar1) {
    FUN_0004cfd0(iVar1);
  }
  if (param_2 != (int *)0x0) {
    *param_2 = iVar5;
  }
  return uVar6;
}

/* NEAR 85/82 words, 72 differ, first +0x28. Probe 1 = pseudocode transcription (promoted). Probe 2 batch:
   uVar6 made the single buffer home through the retry (old pointer saved to uVar2 only around the
   second alloc) 85/82 @73; same with an XOR page test 85/82 @73. No move.
   Retail: 32-byte frame, 7 saved (s0-s6): s6=param_1, s5=param_2, s4=param_3; s2=handle, s1=size,
   s3=result/buffer, s0=rounded size; `bltzl s2` jumps straight to the shared free with `move a0,s3` in
   the delay; page test `lui a0,0xff00` compare on (s3 & 0xff000000) vs ((s3+s1) & 0xff000000);
   second alloc saves the old pointer in s6 via the jal delay; `beqzl s5` returns s3 with `move v0,s3`.
   Ours: same frame and same 7 saved regs, but param_1 landed in s2 and the handle in s3 (retail s6/s2),
   and we carry 3 extra move words.
   GAP: allocation. NEXT: force the parameter homes by copying param_1 to a local that is used in BOTH
   the first lookup and the later FUN_00014d64 call (`int h = param_1;` used twice), which is the only
   value retail keeps in s6; if that still lands s2, the home order is allocator-internal and this item
   needs a different declaration count, not another spelling. */
// near miss from lane dsn8 (unreviewed)
