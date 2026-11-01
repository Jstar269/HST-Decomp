/* HSTMATCH module=EBOOT.BIN start=0x00153b98 end=0x00153e0c symbol=FUN_00153b98 lang=c99 */
extern char DAT_003402f0;
extern char DAT_002dc440[];
extern char DAT_002dc624[];
extern int *DAT_0034a118;
extern int DAT_0034a114;
extern char DAT_003083cc[];
extern char DAT_0030899c[];
extern void FUN_0014d044(int);
extern unsigned long long FUN_00020f5c(int);
extern void FUN_0014fd94(char *, int, int, char *, int, int, void *, char *);
extern int FUN_001c6f70(int, int, int);
extern int FUN_001c70c0(int);
extern void FUN_00287ff0(int *, int, int, int, int *, char);
extern int FUN_00000bcc(int);

int *FUN_00153b98(int param_1, int param_2, int param_3)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar8;
  char *puVar6;
  int *piVar7;
  unsigned long long uVar9;

  if (DAT_003402f0 != '\0') {
    FUN_0014d044(param_3);
    if (*(char *)(param_2 + 0x1a) == '\0') {
      pcVar8 = DAT_002dc440;
    }
    else {
      pcVar8 = (char *)0x2dc804;
    }
    if (*(char *)(param_2 + 0x19) == '\0') {
      puVar6 = DAT_002dc440;
    }
    else {
      puVar6 = DAT_002dc624;
    }
    uVar9 = FUN_00020f5c(*(int *)(param_2 + 0x14));
    if (*(char *)(param_2 + 0x18) == '\0') {
      pcVar5 = DAT_002dc440;
    }
    else {
      pcVar5 = (char *)0x2dc654;
    }
    FUN_0014fd94((char *)0x2dc810, *(int *)(param_2 + 8), *(int *)(param_2 + 0xc), pcVar5,
                 (int)uVar9, (int)(uVar9 >> 32), puVar6, pcVar8);
  }
  iVar2 = *(int *)(param_2 + 8);
  piVar7 = *(int **)(param_3 + 0xc);
  if (iVar2 == -1) {
    piVar1 = DAT_0034a118;
    if (DAT_0034a118 != (int *)0x0) {
      piVar1 = DAT_0034a118 + 0x38;
    }
  }
  else {
    piVar1 = piVar7;
    if (-1 < iVar2) {
      if (iVar2 < 100) {
        piVar1 = *(int **)(*(int *)(DAT_0034a114 + 0x4c) + iVar2 * 4);
      }
      else if (iVar2 < 1000) {
        piVar1 = (int *)FUN_001c6f70(DAT_0034a114, iVar2, 0);
      }
      else {
        piVar1 = (int *)FUN_001c70c0(DAT_0034a114);
      }
    }
  }
  if (piVar1 == (int *)0x0) {
    piVar7 = (int *)0x0;
  }
  else {
    FUN_00287ff0(piVar1, *(int *)(param_2 + 0xc), *(int *)(param_2 + 0x10), 0, piVar7,
                 *(unsigned char *)(param_2 + 0x1a));
    piVar1[0x10] = -1;
    (*(int (**)(int, int *))(*piVar1 + 0x40))(*(int *)(param_2 + 0x14), piVar1);
    if (*(char *)(param_2 + 0x18) == '\0') {
      piVar7 = (int *)0x0;
    }
    else {
      piVar7 = (int *)FUN_00000bcc(0x10);
      if (piVar7 != (int *)0x0) {
        iVar4 = *(int *)(param_2 + 0x10);
        iVar3 = *(int *)(param_2 + 0xc);
        iVar2 = *(int *)(param_2 + 8);
        *piVar7 = (int)&DAT_003083cc;
        *piVar7 = (int)&DAT_0030899c;
        piVar7[1] = iVar2;
        piVar7[2] = iVar3;
        piVar7[3] = iVar4;
      }
      iVar2 = (*(int (**)(int, int *, int))(*piVar7 + 0xc))(param_1, piVar7, param_3);
      if (iVar2 != 0) {
        if (piVar7 == (int *)0x0) {
          piVar7 = (int *)0x0;
        }
        else {
          if (piVar7 != (int *)0x0) {
            (*(int (**)(int *, int))(*piVar7 + 8))(piVar7, 1);
          }
          piVar7 = (int *)0x0;
        }
      }
    }
  }
  return piVar7;
}

/* NEAR 151/157 words, 153 differ, first +0x0. Probe 1 = pseudocode transcription; BODY gap (6 words
   short, prologue/frame wrong at the first word), parked after one probe.
   Decoded shape: if (DAT_003402f0): FUN_0014d044(param_3); three string selects on param_2 bytes
   +0x1a (0x2dc804 vs 0x2dc440), +0x19 (0x2dc624 vs DAT_002dc440), +0x18 (0x2dc654 vs DAT_002dc440);
   FUN_00020f5c(param_2[0x14]) returns a 64-bit value passed as TWO args (lo, hi) to
   FUN_0014fd94(0x2dc810, param_2[8], param_2[0xc], pcVar5, lo, hi, puVar6, pcVar8) -- 8 arguments.
   Then target select: n=param_2[8]; piVar7=param_3[0xc]; n==-1 -> DAT_0034a118 (+0x38 when non-null);
   else 0<=n<100 -> *(*(DAT_0034a114+0x4c)+n*4); 100<=n<1000 -> FUN_001c6f70(DAT_0034a114,n,0);
   >=1000 -> FUN_001c70c0(DAT_0034a114). If target null return 0; else FUN_00287ff0(target,
   param_2[0xc], param_2[0x10], 0, piVar7, byte param_2[0x1a]); target[0x10]=-1;
   (*(*target+0x40))(param_2[0x14], target); then byte param_2[0x18]==0 -> return 0, else allocate
   0x10, write vtable 0x30899c (and 0x3083cc first), fields {param_2[8], [0xc], [0x10]}, and call
   (*(*obj+0xc))(param_1,obj,param_3); on non-zero result call (*(*obj+8))(obj,1) and return 0.
   NEXT: frame at +0x0 -- the double vtable store in the 0x10 alloc and the unused first `*piVar7 =
   &DAT_003083cc` are likely Ghidra artifacts of a constructor; read retail's prologue and the alloc
   block before rewriting anything else. */
// near miss from lane dsn8 (unreviewed)
