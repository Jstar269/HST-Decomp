/* HSTMATCH module=EBOOT.BIN start=0x0029e160 end=0x0029e3cc symbol=FUN_0029e160 lang=c99 */
extern int DAT_00309e80;
extern int DAT_00338ac4;
extern int DAT_0034176c;
extern int DAT_003415ac;
extern char DAT_002fe878[];
extern void FUN_0001f06c(void);

extern int FUN_00000bcc(int);
extern int FUN_000ff4fc(int, int);
extern void FUN_002956e0(int);
extern int FUN_000ba2a8(int);
extern void FUN_000ba48c(int);
extern void FUN_000ba9e0(int, void *);
extern void FUN_000ba57c(int, void *);
extern int FUN_000c02d8(int);
extern unsigned int FUN_00010184(int, unsigned int);
extern int FUN_00111044(int, void *, int);
extern int FUN_0018c9f4(int);
extern int FUN_0018f1ac(int, int, int);
extern int FUN_00198278(int, int, int, int *);
extern void FUN_00115088(int, int, int, int, int);
extern void FUN_000fe43c(int);
extern int FUN_00296b5c(char *, int);
extern void FUN_0029e3cc(char *, int);
extern void FUN_0001cb88(int);
extern void FUN_00047620(int);

unsigned int *FUN_0029e160(unsigned int *param_1)
{
  unsigned int *puVar1;
  int iVar2;
  int uVar3;
  unsigned char auStack_20[16];
  int local_10;
  int local_c;
  char *local_8;
  char *local_4;

  FUN_0001f06c();
  *param_1 = (unsigned int)&DAT_00309e80;
  param_1[0xf] = 0;
  ((void (*)(int, int, char *))0x001cad0)(0x30ebf4, 1, (char *)0x2fff84);
  puVar1 = (unsigned int *)FUN_00000bcc(0x205c4);
  if (puVar1 != (unsigned int *)0x0) {
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[3] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0x7fffffff;
    puVar1[6] = 0x7fffffff;
    puVar1[7] = 0;
    puVar1[8] = 0;
    *(unsigned char *)(puVar1 + 9) = 0;
    *(unsigned char *)((int)puVar1 + 0x25) = 0;
    puVar1[10] = 0xffffffff;
    puVar1[0x6b] = 0;
    puVar1[0x6c] = 0;
    puVar1[0x6d] = 0;
    puVar1[0x406e] = 0;
    puVar1[0x806f] = 0;
    puVar1[0x8070] = 0;
    iVar2 = FUN_00000bcc(0x14);
    if (iVar2 == 0) {
      puVar1[0x6d] = 0;
      uVar3 = 0;
    }
    else {
      uVar3 = FUN_000ff4fc(iVar2, 0x40000);
      puVar1[0x6d] = uVar3;
    }
    FUN_002956e0(uVar3);
    iVar2 = FUN_00000bcc(0x288);
    uVar3 = 0;
    if (iVar2 != 0) {
      uVar3 = FUN_000ba2a8(iVar2);
    }
    DAT_00338ac4 = uVar3;
    FUN_000ba48c(uVar3);
    FUN_000ba9e0(DAT_00338ac4, auStack_20);
    local_10 = 0x12000;
    local_c = 0x4000;
    local_8 = DAT_002fe878;
    local_4 = (char *)0x2fe888;
    FUN_000ba57c(DAT_00338ac4, auStack_20);
    iVar2 = FUN_00000bcc(0x1c);
    if (iVar2 == 0) {
      puVar1[1] = 0;
    }
    else {
      uVar3 = FUN_000c02d8(iVar2);
      puVar1[1] = uVar3;
    }
    uVar3 = FUN_00010184(0x40, 0x218000);
    puVar1[2] = uVar3;
    iVar2 = FUN_00000bcc(0x2c);
    if (iVar2 == 0) {
      puVar1[0x6c] = 0;
    }
    else {
      uVar3 = FUN_00111044(iVar2, (void *)0x2fff7c, 2);
      puVar1[0x6c] = uVar3;
    }
    uVar3 = FUN_0018c9f4(0x11);
    uVar3 = FUN_0018f1ac(uVar3, 0xe, 1);
    uVar3 = FUN_00198278(DAT_0034176c, uVar3, 0x400, (int *)(puVar1 + 0x8071));
    FUN_00115088(DAT_003415ac, 3, uVar3, 0, 0);
    FUN_000fe43c(1);
    iVar2 = FUN_00296b5c((char *)0x2cba18, 0);
    if (iVar2 < 0) {
      FUN_0029e3cc((char *)0x2cb9ec, iVar2);
    }
  }
  param_1[0xf] = (unsigned int)puVar1;
  FUN_0001cb88(0x30ebf4);
  FUN_00047620(0);
  return param_1;
}

/* NEAR 151/155 words, 147 differ, first +0x0. Probe 1 = pseudocode transcription, BODY gap (4 words
   short, frame/saved-register set wrong from the first word), parked after one probe by the pass rule.
   Shape: FUN_0001f06c(); *param_1=&DAT_00309e80; param_1[0xf]=0; FUN_001cad0(0x30ebf4,1,str 0x2fff84)
   -- FUN_001cad0 is NOT in the symbol map, so it had to be called through a raw function pointer at
   0x001cad0; puVar1=FUN_00000bcc(0x205c4); if non-null: 18 fixed stores (0,0,0,0,0,0x7fffffff,
   0x7fffffff,0,0,byte 0 at +0x24 and +0x25,0xffffffff at [10], zeros at [0x6b],[0x6c],[0x6d],[0x406e],
   [0x806f],[0x8070]); FUN_00000bcc(0x14) -> FUN_000ff4fc(p,0x40000) stored at [0x6d]; FUN_002956e0;
   FUN_00000bcc(0x288) -> FUN_000ba2a8 -> DAT_00338ac4, FUN_000ba48c, FUN_000ba9e0(DAT,sp-block),
   sp-block = {0x12000, 0x4000, 0x2fe878, 0x2fe888}, FUN_000ba57c; 0x1c alloc -> FUN_000c02d8 -> [1];
   FUN_00010184(0x40,0x218000) -> [2]; 0x2c alloc -> FUN_00111044(p,0x2fff7c,2) -> [0x6c];
   FUN_0018c9f4(0x11) -> FUN_0018f1ac(.,0xe,1) -> FUN_00198278(DAT_0034176c,.,0x400,puVar1+0x8071);
   FUN_00115088(DAT_003415ac,3,.,0,0); FUN_000fe43c(1); FUN_00296b5c(0x2cba18,0) < 0 ->
   FUN_0029e3cc(0x2cb9ec,rc); then param_1[0xf]=puVar1; FUN_0001cb88(0x30ebf4); FUN_00047620(0).
   NEXT: read retail's prologue to get the exact frame/saved set (first diff is +0x0), then align the
   sp-block declaration order (local_10/local_c/local_8/local_4 at sp+..) with retail's offsets before
   any statement-level change. */
// near miss from lane dsn8 (unreviewed)
