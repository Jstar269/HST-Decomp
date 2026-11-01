/* HSTMATCH module=EBOOT.BIN start=0x001f02fc end=0x001f0554 symbol=FUN_001f02fc lang=c99 */
extern int DAT_002ee83c;
extern int DAT_002ee840;
extern char DAT_002cfc8c[];

extern char FUN_00001040[];
extern void FUN_001f9390();
extern void FUN_001e09dc(int, int);
extern void FUN_001e2990(int);
extern void FUN_001e71c4(int, int, int, int);
extern int FUN_00065740(int, char *, int);
extern void FUN_001e744c(int);
extern void FUN_001e28e0(int);
extern void FUN_001b1c44(int, int);
extern void FUN_001f94ac(int);
extern void FUN_001f3770(int);
extern int FUN_001b2af0(int, int);
extern void FUN_001f46b4(int, int);
extern void FUN_001e722c(int, int, unsigned char, int);
extern void FUN_001e73d8(int);
extern void FUN_001f93dc(int);
extern void FUN_001f461c(int, int);
extern void FUN_001f4ee0(int, int);
extern void FUN_001f94a4(int);
extern void FUN_001f4bfc(int);
extern void FUN_001f47a4(int);

void FUN_001f02fc(int param_1)
{
  int bVar1;
  int uVar2;
  int iVar3;
  int iVar4;

  if (*(int *)(param_1 + 0x44) == 0) {
    *(int *)(param_1 + 0x1038) = 0;
    *(int *)(param_1 + 0x104c) = 0;
    FUN_001f9390();
    FUN_001e09dc(*(int *)(param_1 + 0x510), 2);
    FUN_001e2990(*(int *)(param_1 + 0x510));
    FUN_001e71c4(*(int *)(param_1 + 0x510), *(int *)(param_1 + 0x1044), 0, 0);
  }
  if (*(int *)(param_1 + 0x1038) == 2) {
    iVar4 = *(int *)(param_1 + 0x104c) + 1;
    *(int *)(param_1 + 0x104c) = iVar4;
    iVar3 = DAT_002ee83c;
    if (iVar4 < DAT_002ee83c) {
      *(int *)(param_1 + 0x104c) = DAT_002ee83c;
    }
    else {
      iVar3 = DAT_002ee840;
      if (iVar4 <= DAT_002ee840) {
        iVar3 = iVar4;
      }
      *(int *)(param_1 + 0x104c) = iVar3;
    }
    iVar4 = FUN_00065740(*(int *)(*(int *)(param_1 + 0x584) + 0x90), (char *)0x2ee830, 1);
    if (iVar3 == iVar4 + 1) {
      FUN_001e744c(*(int *)(param_1 + 0x510));
      iVar3 = *(int *)(param_1 + 0x104c);
    }
    else {
      iVar3 = *(int *)(param_1 + 0x104c);
    }
    if (iVar3 == 10) {
      FUN_001e28e0(*(int *)(param_1 + 0x510));
      iVar3 = *(int *)(param_1 + 0x104c);
    }
    else {
      iVar3 = *(int *)(param_1 + 0x104c);
    }
    if (10 < iVar3) {
      if (*(char *)(param_1 + 0x58c) == '\0') {
        bVar1 = ((int)*(char *)(DAT_002cfc8c + 0x8b) << 0x1c) >> 0x1e == 0;
      }
      else {
        bVar1 = 0;
      }
      if (bVar1) {
        FUN_001b1c44(0x34179c, 3);
        FUN_001f9390(param_1);
        FUN_001f94ac(param_1);
        FUN_001f3770(param_1);
      }
    }
  }
  else if ((*(int *)(param_1 + 0x1038) == 0) &&
          (iVar3 = *(int *)(param_1 + 0x104c) + 1, *(int *)(param_1 + 0x104c) = iVar3, 0x28 < iVar3)) {
    *(int *)(param_1 + 0x1038) = 2;
    *(int *)(param_1 + 0x104c) = 0;
    *(int *)(param_1 + 0x1044) = *(int *)(FUN_00001040 + param_1);
    if (*(char *)(param_1 + 0x1048) == '\0') {
      uVar2 = FUN_001b2af0(0x34179c, 0xe);
      *(int *)(param_1 + 0x5b8) = uVar2;
      FUN_001f46b4(param_1, 0);
      FUN_001e722c(*(int *)(param_1 + 0x510), *(int *)(param_1 + 0x1044),
                   *(unsigned char *)(param_1 + 0x1048), 1);
      FUN_001e73d8(*(int *)(param_1 + 0x510));
      FUN_001f93dc(param_1);
    }
    else {
      uVar2 = FUN_001b2af0(0x34179c, 0xf);
      *(int *)(param_1 + 0x5b8) = uVar2;
      FUN_001f461c(param_1, 0);
      FUN_001f4ee0(param_1, *(int *)(param_1 + 0x3c4));
      FUN_001e722c(*(int *)(param_1 + 0x510), *(int *)(param_1 + 0x1044),
                   *(unsigned char *)(param_1 + 0x1048), 1);
      FUN_001e73d8(*(int *)(param_1 + 0x510));
      FUN_001f94a4(param_1);
    }
  }
  FUN_001f4bfc(param_1);
  FUN_001f47a4(param_1);
  return;
}

/* NEAR 151/150 words, 130 differ, first +0x3c. Probe 1 = pseudocode transcription. Word count is exact,
   so this is ALLOCATION-class, not a body gap; but the 130 differs are a whole-function cascade and one
   spelling cannot move 130 words, so the second probe was deliberately not spent (catalogue-first rule).
   First divergence: FUN_001e71c4 argument loads -- retail `lw a1,0x1044(s1)` before `lw a0,0x510(s1)`,
   ours the reverse; and the state test is `bne v0,2` in ours vs retail `beql v0,v1,0x1f0438` (branch-likely
   into the state==2 body, i.e. retail treats the ==2 arm as the likely path).
   Function shape: if (p[0x44]==0) init (clear 0x1038/0x104c, FUN_001f9390, FUN_001e09dc(p[0x510],2),
   FUN_001e2990, FUN_001e71c4(p[0x510],p[0x1044],0,0)); if (p[0x1038]==2) clamp ++p[0x104c] to
   [DAT_002ee83c, DAT_002ee840], read the FlashTime timer, call FUN_001e744c when it equals clamp+1,
   FUN_001e28e0 at 10, and when >10 with p[0x58c]==0 and bitfield test ((DAT_002cfc8c[0x8b]<<28)>>30)==0
   call FUN_001b1c44(0x34179c,3)+FUN_001f9390(p)+FUN_001f94ac(p)+FUN_001f3770(p); else if (p[0x1038]==0 &&
   ++p[0x104c] > 0x28) { p[0x1038]=2; p[0x104c]=0; p[0x1044]=*(int*)(FUN_00001040+p); then the
   p[0x1048]==0 arm (0xe message, FUN_001f46b4, FUN_001e722c(p[0x510],p[0x1044],byte p[0x1048],1),
   FUN_001e73d8, FUN_001f93dc) or the else arm (0xf message, FUN_001f461c, FUN_001f4ee0(p,p[0x3c4]), same
   two calls, FUN_001f94a4) } then FUN_001f4bfc(p); FUN_001f47a4(p).
   NEXT: name the FUN_001e71c4 arguments in retail's order as two locals read a1-first, and write the
   state test as an inverted `if (state != 2) { ... }` with the ==2 body as fall-through. */
// near miss from lane dsn8 (unreviewed)
