/* HSTMATCH module=EBOOT.BIN start=0x00151204 end=0x00151484 symbol=FUN_00151204 lang=c99 */
extern char DAT_003402f0;
extern char DAT_002dc440[];
extern char DAT_002dc4c0[];
extern char DAT_002dc500[];
extern int *DAT_0034a118;
extern int DAT_0034a114;
extern char DAT_003083cc[];
extern char DAT_00308da0[];
extern void FUN_0014d044(int);
extern void FUN_0014fd94(char *, int, int, int, int, void *, void *, char *);
extern int FUN_001c6f70(int, int, int);
extern int FUN_001c70c0(int);
extern void FUN_002874c4(int *, int, int, unsigned char, unsigned char, int);
extern int FUN_00000bcc(int);

int *FUN_00151204(int param_1, int param_2, int param_3)
{
  char cVar1;
  int bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *puVar6;
  char *puVar7;
  char *pcVar8;
  int *piVar9;

  if (DAT_003402f0 != '\0') {
    FUN_0014d044(param_3);
    if (*(char *)(param_2 + 0x1a) == '\0') {
      pcVar8 = (char *)0x2dc4b4;
    }
    else {
      pcVar8 = (char *)0x2dc4a8;
    }
    if (*(char *)(param_2 + 0x19) == '\0') {
      puVar7 = DAT_002dc440;
    }
    else {
      puVar7 = DAT_002dc4c0;
    }
    if (*(char *)(param_2 + 0x18) == '\0') {
      puVar6 = DAT_002dc440;
    }
    else {
      puVar6 = DAT_002dc500;
    }
    FUN_0014fd94((char *)0x2dc4c8, *(int *)(param_2 + 8), *(int *)(param_2 + 0xc),
                 *(int *)(param_2 + 0x10), *(int *)(param_2 + 0x14), puVar6, puVar7, pcVar8);
  }
  iVar5 = *(int *)(param_2 + 8);
  piVar9 = *(int **)(param_3 + 0xc);
  if (iVar5 == -1) {
    piVar9 = DAT_0034a118;
    if (DAT_0034a118 != (int *)0x0) {
      piVar9 = DAT_0034a118 + 0x38;
    }
  }
  else if (-1 < iVar5) {
    if (iVar5 < 100) {
      piVar9 = *(int **)(*(int *)(DAT_0034a114 + 0x4c) + iVar5 * 4);
    }
    else if (iVar5 < 1000) {
      piVar9 = (int *)FUN_001c6f70(DAT_0034a114, iVar5, 0);
    }
    else {
      piVar9 = (int *)FUN_001c70c0(DAT_0034a114);
    }
  }
  if (piVar9 == (int *)0x0) {
    cVar1 = *(char *)(param_2 + 0x19);
  }
  else {
    iVar5 = (*(int (**)(int *))(*piVar9 + 0x34))(piVar9);
    bVar2 = iVar5 == 2;
    if (!bVar2) {
      iVar5 = (*(int (**)(int *))(*piVar9 + 0x34))(piVar9);
      bVar2 = iVar5 == 3;
    }
    if (bVar2) {
      FUN_002874c4(piVar9, *(int *)(param_2 + 0xc), *(int *)(param_2 + 0x14),
                   *(unsigned char *)(param_2 + 0x18), *(unsigned char *)(param_2 + 0x1a),
                   *(int *)(param_2 + 0x10));
    }
    cVar1 = *(char *)(param_2 + 0x19);
  }
  if (cVar1 == '\0') {
    piVar9 = (int *)0x0;
  }
  else {
    piVar9 = (int *)FUN_00000bcc(0x10);
    if (piVar9 != (int *)0x0) {
      iVar5 = *(int *)(param_2 + 0x10);
      iVar4 = *(int *)(param_2 + 0xc);
      iVar3 = *(int *)(param_2 + 8);
      *piVar9 = (int)&DAT_003083cc;
      *piVar9 = (int)&DAT_00308da0;
      piVar9[1] = iVar3;
      piVar9[2] = iVar4;
      piVar9[3] = iVar5 + 1;
    }
    iVar5 = (*(int (**)(int, int *, int))(*piVar9 + 0xc))(param_1, piVar9, param_3);
    if (iVar5 != 0) {
      if (piVar9 == (int *)0x0) {
        piVar9 = (int *)0x0;
      }
      else {
        if (piVar9 != (int *)0x0) {
          (*(int (**)(int *, int))(*piVar9 + 8))(piVar9, 1);
        }
        piVar9 = (int *)0x0;
      }
    }
  }
  return piVar9;
}

/* NEAR 158/160 words, 152 differ, first +0x8. Probe 1 = pseudocode transcription; BODY gap (2 words
   short, prologue save sequence diverges at the second word), parked after one probe.
   Sibling of 0x00153b98: if (DAT_003402f0) { FUN_0014d044(param_3); string selects on p bytes
   +0x1a (0x2dc4a8/0x2dc4b4), +0x19 (0x2dc4c0 vs 0x2dc440), +0x18 (0x2dc500 vs 0x2dc440);
   FUN_0014fd94(0x2dc4c8, p[8], p[0xc], p[0x10], p[0x14], puVar6, puVar7, pcVar8) -- 8 args } then the
   same target select (p[8]==-1 -> DAT_0034a118 +0x38; <100 -> *(*(DAT_0034a114+0x4c)+n*4);
   <1000 -> FUN_001c6f70; else FUN_001c70c0); if target: call (*target+0x34)() twice, bVar2 when
   result==2 else ==3, and when set FUN_002874c4(target, p[0xc], p[0x14], byte p[0x18], byte p[0x1a],
   p[0x10]); cVar1 = byte p[0x19] in both arms; if cVar1==0 return 0 else alloc 0x10, vtable 0x308da0
   (after a dead 0x3083cc store), fields {p[8], p[0xc], p[0x10]+1}, call (*obj+0xc)(param_1,obj,param_3)
   and on non-zero call (*obj+8)(obj,1) and return 0.
   NEXT: prologue at +0x8 only -- ours is 2 words short in the save sequence; read retail's header and
   match the saved-register set before touching the body. The dead first vtable store and the double
   (*target+0x34)() call are the two body questions after that. */
// near miss from lane dsn8 (unreviewed)
