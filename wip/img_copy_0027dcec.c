/* HSTMATCH module=EBOOT.BIN start=0x0027dcec end=0x0027df68 symbol=FUN_0027dcec lang=c99 */
extern char DAT_003070bc[];
extern void FUN_0008b1c8(int *, int, int);
extern void FUN_000649c0(void *);
extern void FUN_0008c230(void *);
extern int FUN_000651b8(void *, int, int, int, void *);
extern int FUN_000650e0(void *);
extern int FUN_000650e8(void *, int);
extern void FUN_0027dfac(int *);
extern void FUN_000114c0(void *, int, int);
extern unsigned char FUN_00065104(void *, unsigned int, int);
extern void FUN_0027df68(int *, void *, int);
extern void FUN_00066338(void *, int);
extern void FUN_00064a08(void *, int);

void FUN_0027dcec(int *param_1, int param_2)
{
  unsigned char uVar1;
  int iVar2;
  int iVar3;
  int iVar8;
  int iVar9;
  int iVar10;
  unsigned int uVar7;
  unsigned char *puVar4;
  unsigned char *puVar5;
  unsigned char *puVar6;
  unsigned char local_98[100];
  unsigned char local_34[4];
  char *local_30[4];
  unsigned char auStack_20[16];
  unsigned char auStack_10[16];

  FUN_0008b1c8(param_1, param_1[1], 0);
  FUN_000649c0(local_30);
  local_30[0] = (char *)&DAT_003070bc;
  FUN_0008c230(auStack_20);
  FUN_0008c230(auStack_10);
  iVar2 = FUN_000651b8(local_30, param_2, 0, 0, (char *)0x2fe780);
  if (iVar2 != 0) {
    iVar2 = FUN_000650e0(local_30);
    iVar3 = FUN_000650e8(local_30, 0);
    if ((unsigned int)param_1[2] < (unsigned int)(iVar3 / 5)) {
      FUN_0027dfac(param_1);
    }
    iVar10 = 0;
    if (0 < iVar3) {
      do {
        FUN_000114c0(local_98, 0, 0x68);
        iVar8 = 0;
        puVar6 = local_98;
        do {
          iVar9 = iVar10;
          uVar7 = 0;
          do {
            uVar1 = FUN_00065104(local_30, uVar7, iVar9);
            puVar6[uVar7] = uVar1;
            uVar7 = uVar7 + 1;
          } while (uVar7 < 0x14);
          iVar8 = iVar8 + 1;
          iVar10 = iVar9 + 1;
          puVar6 = puVar6 + 0x14;
        } while (iVar8 < 5);
        if (iVar2 == 0x18) {
          uVar7 = 0;
          puVar6 = local_98;
          do {
            uVar1 = FUN_00065104(local_30, uVar7 + 0x14, iVar9);
            puVar6[100] = uVar1;
            uVar7 = uVar7 + 1;
            puVar6 = puVar6 + 1;
          } while (uVar7 < 4);
          uVar7 = param_1[1];
        }
        else {
          uVar7 = param_1[1];
        }
        if (uVar7 < (unsigned int)param_1[2]) {
          puVar6 = (unsigned char *)(*param_1 + uVar7 * 0x68);
          if (puVar6 == (unsigned char *)0x0) {
            iVar8 = param_1[1];
          }
          else {
            puVar5 = local_98;
            iVar8 = 0x32;
            puVar4 = puVar6;
            do {
              uVar1 = puVar5[1];
              iVar8 = iVar8 - 1;
              *puVar4 = *puVar5;
              puVar4[1] = uVar1;
              puVar5 = puVar5 + 2;
              puVar4 = puVar4 + 2;
            } while (0 < iVar8);
            puVar6[100] = local_34[0];
            puVar6[0x65] = local_34[1];
            puVar6[0x66] = local_34[2];
            puVar6[0x67] = local_34[3];
            iVar8 = param_1[1];
          }
          param_1[1] = iVar8 + 1;
        }
        else {
          FUN_0027df68(param_1, local_98, 0);
        }
      } while (iVar10 < iVar3);
    }
  }
  local_30[0] = (char *)&DAT_003070bc;
  FUN_00066338(auStack_10, 0);
  FUN_00066338(auStack_20, 0);
  FUN_00064a08(local_30, 0);
  return;
}

/* NEAR 167/159 words, 166 differ, first +0x0. Probe 1 = pseudocode transcription; BODY gap (8 words
   over, prologue/frame wrong at the first word), parked after one probe.
   Family note: this is the FIRST CALLER of the already-parked 0x000651b8 (same 5-argument
   FUN_000651b8(local_30, param_2, 0, 0, &DAT_002fe780) call shape), and it is the image-slice copier
   built on the FUN_000650e0/FUN_000650e8/FUN_00065104 accessor trio -- group it with the 0x000651b8
   and 0x000653ac/0x000657e4 dispatch siblings.
   Decoded: FUN_0008b1c8(param_1, param_1[1], 0); vtable local_30[0]=&DAT_003070bc (FUN_000649c0
   first); FUN_0008c230 on two 16-byte blocks; FUN_000651b8(...) must be non-zero; then
   iVar2=FUN_000650e0(local_30), iVar3=FUN_000650e8(local_30,0); if (param_1[2] < iVar3/5)
   FUN_0027dfac(param_1); outer do/while over rows iVar10<iVar3: FUN_000114c0(local_98,0,0x68);
   5 inner rows x 20 columns of FUN_00065104(local_30, col, row) into local_98; if iVar2==0x18 a
   4-byte tail from FUN_00065104(local_30, col+0x14, iVar9) into local_98[100..103]; then either copy
   0x64 bytes two-at-a-time into *param_1 + param_1[1]*0x68 (re-reading param_1[1], with the dead
   null check on the computed pointer) and fill +100/+0x65/+0x66/+0x67 from local_34, incrementing
   param_1[1], or FUN_0027df68(param_1, local_98, 0) when full; teardown: local_30[0]=&DAT_003070bc,
   FUN_00066338 on both blocks, FUN_00064a08(local_30,0).
   NEXT: frame at +0x0 -- local_98[100] is followed by local_34[4] and the code writes puVar6[100],
   which only matches if MWCC lays local_34 immediately after local_98; read retail's frame (sp offsets)
   and declare the block as one 0x68 array with local_34 views before touching the loops. */
// near miss from lane dsn8 (unreviewed)
