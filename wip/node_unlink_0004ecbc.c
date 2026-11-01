/* HSTMATCH module=EBOOT.BIN start=0x0004ecbc end=0x0004ee14 symbol=FUN_0004ecbc lang=c99 */
extern int DAT_0032b238;
extern int DAT_0032b23c;
extern int *DAT_0032b234;
extern void FUN_00000a00();
extern void FUN_000104e0(void);

void FUN_0004ecbc(int param_1)
{
  int iVar1;
  int *piVar3;
  int *piVar4;
  int *piVar5;

  if ((DAT_0032b238 <= param_1) && (param_1 < DAT_0032b23c)) {
    if (DAT_0032b234 != (int *)0x0) {
      iVar1 = DAT_0032b234[3];
      piVar3 = DAT_0032b234;
      while (iVar1 != param_1) {
        piVar3 = (int *)piVar3[1];
        if (piVar3 == (int *)0x0) {
          return;
        }
        iVar1 = piVar3[3];
      }
      *(unsigned char *)(piVar3 + 2) = 1;
      piVar5 = (int *)*piVar3;
      piVar4 = piVar3;
      if (piVar5 == (int *)0x0) {
        piVar3 = (int *)piVar3[1];
      }
      else {
        if (((unsigned char)piVar5[2] != 0) &&
           (piVar5[4] = piVar3[4], piVar4 = piVar5, piVar3 != (int *)0x0)) {
          if ((int *)piVar3[1] == (int *)0x0) {
            iVar1 = *piVar3;
          }
          else {
            *(int *)piVar3[1] = *piVar3;
            iVar1 = *piVar3;
          }
          if (iVar1 == 0) {
            DAT_0032b234 = (int *)piVar3[1];
          }
          else {
            *(int *)(iVar1 + 4) = piVar3[1];
          }
          FUN_00000a00(piVar3);
        }
        piVar3 = (int *)piVar4[1];
      }
      if (((piVar3 != (int *)0x0) && ((unsigned char)piVar3[2] != 0)) &&
         (piVar4[4] = piVar3[4], piVar3 != (int *)0x0)) {
        if ((int *)piVar3[1] == (int *)0x0) {
          iVar1 = *piVar3;
        }
        else {
          *(int *)piVar3[1] = *piVar3;
          iVar1 = *piVar3;
        }
        if (iVar1 == 0) {
          DAT_0032b234 = (int *)piVar3[1];
        }
        else {
          *(int *)(iVar1 + 4) = piVar3[1];
        }
        FUN_00000a00();
      }
    }
  }
  else {
    FUN_000104e0();
  }
  return;
}

/* NEAR 89/86 words, 77 differ, first +0x14. Probe 1 = pseudocode transcription with a `bVar2` flag 90/86 @82;
   probe 2: inline range test 89/86 @77 (promoted), reversed operands 89/86 @78.
   Retail: slt a0,b238 / bnez + slt a0,b23c / beqz with a1 as the 0/1 flag (no bool variable); list walk
   `bnel a1[3],param_1` with `a1=a1[1]` in the delay; found -> `sb 1,8(node)`, s0=node[0]; if s0==0 a0=node[1];
   else if s0[2]!=0 { s0[4]=node[4]; unlink node via beqzl/bnel chain writing node[1][0] and node[0][1];
   free(node) with `move a0,a1` in the jal delay; a1=s0 } a0=a1[1]; second block identical on a0 with a1 as
   parent, free(a0) with a `nop` jal delay; fail path calls FUN_000104e0() with a nop delay.
   GAP: allocation. NEXT: the two unlink blocks are written identically but our copies do not share homes --
   give the parent/child pair explicit locals (`int *p; int *c;`) reused in both blocks so MWCC allocates one
   pair of registers, and check the 3-word excess is the duplicate argument move for the first free. */
// near miss from lane dsn8 (unreviewed)
