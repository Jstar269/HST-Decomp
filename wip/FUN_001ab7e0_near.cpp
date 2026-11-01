/* HSTMATCH module=EBOOT.BIN start=0x001ab7e0 end=0x001abd8c symbol=FUN_001ab7e0 lang=c++ */

extern "C" {
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef int undefined;

struct Widget {
    virtual void func0();
    virtual void func1(int);
};

extern int DAT_002e20d4;
extern int DAT_0034a77c;

extern int FUN_00014d64(...);
extern int FUN_000d7d80(...);
extern int FUN_000d7e30(...);
extern int FUN_000ffb3c(...);
extern int FUN_0010d4a8(...);
extern int FUN_0016b21c(...);
extern int FUN_0016bb7c(...);
extern int FUN_00193474(...);
extern int FUN_0019d474(...);
extern int FUN_001fea10(...);
extern int FUN_002045e8(...);
extern int FUN_002181a4(...);
extern int FUN_002182a0(...);
extern uint FUN_0024dd40(float, int);


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_001ab7e0(int param_1,int param_2,int param_3)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  short *psVar8;
  int *piVar9;
  uint uVar10;
  int iVar11;
  int local_dc;
  int local_d8 [4];
  undefined1 auStack_c8 [200];
  
  int idx11 = param_3 * 11 + 60;
  int idx5 = param_3 * 5 + 30;
  iVar2 = FUN_0019d474();
  uVar10 = 0;
  iVar2 = *(int *)(iVar2 + 0x160) + param_2 * 0x30;
  psVar8 = *(short **)(iVar2 + 4);
  bVar1 = false;
  local_dc = (int)psVar8[5];
  iVar11 = (int)*psVar8;
  iVar3 = FUN_0019d474();
  iVar3 = *(int *)(iVar3 + 0x88);
  if (*(unsigned char *)(iVar2 + 10) == '\0') {
    iVar4 = FUN_0019d474();
    if ((*(unsigned char *)(iVar4 + 0x2a4) != '\0') && (*(unsigned char *)(param_2 + iVar3 + 0x14) == '\0')) {
      bVar1 = true;
    }
  }
  else {
    bVar1 = true;
  }
  if (bVar1) {
    piVar7 = *(int **)(param_3 * 4 + param_1 + 0x1c);
    ((Widget *)piVar7)->func1(1);
  }
  else {
    if ((param_2 == 3) || (param_2 == 2)) {
      iVar3 = FUN_0019d474();
      local_dc = (int)psVar8[0x3d];
      uVar10 = (uint)*(unsigned char *)((int)psVar8 + *(int *)(iVar3 + 100) * 3 + 0x7e);
    }
    else if (param_2 == 1) {
      iVar3 = FUN_0019d474();
      if (*(char *)(iVar3 + 0x2a4) == '\0') {
        iVar3 = FUN_0016b21c(iVar11);
        uVar10 = FUN_0024dd40(*(float *)(iVar3 + 4),DAT_0034a77c);
      }
      else {
        iVar3 = FUN_0019d474();
        if (*(char *)(iVar3 + 0x2a5) == '\0') {
          iVar3 = FUN_0016b21c(iVar11);
          uVar10 = FUN_0024dd40(*(float *)(iVar3 + 4),DAT_0034a77c);
        }
        else {
          uVar10 = (uint)*(unsigned char *)((int)psVar8 + 0x67);
        }
      }
    }
    else if (param_2 == 0) {
      iVar3 = FUN_0019d474();
      if ((*(char *)(iVar3 + 0x2a4) == '\0') ||
         (iVar3 = FUN_0019d474(), *(char *)(iVar3 + 0x2a5) != '\0')) {
        iVar3 = FUN_0016b21c(iVar11);
        uVar10 = FUN_0024dd40(*(float *)(iVar3 + 4),DAT_0034a77c);
      }
      else {
        uVar10 = (uint)*(unsigned char *)((int)psVar8 + 0x67);
      }
    }
    FUN_000d7d80(auStack_c8);
    FUN_002045e8(auStack_c8,iVar11,(int)psVar8[1],(int)psVar8[2],(int)psVar8[3],(int)psVar8[4],
                 local_dc,uVar10,*(undefined4 *)(iVar2 + 0xc));
    uVar10 = (uint)*psVar8;
    iVar3 = FUN_00014d64(psVar8 + 0x34,&DAT_002e20d4);
    bVar1 = iVar3 != 0;
    if (bVar1) {
      uVar10 = (uint)(unsigned short)psVar8[0x10];
    }
    else {
      psVar8 = (short *)FUN_00193474((int)*psVar8);
    }
    iVar3 = 0;
    do {
      iVar11 = param_1 + (idx11 + 1 + iVar3) * 0x14;
      FUN_000ffb3c(*(undefined4 *)(iVar11 + 0x2c),0);
      iVar4 = FUN_002181a4(uVar10,bVar1,psVar8 + 0x34,iVar3,auStack_c8);
      iVar11 = *(int *)(iVar11 + 0x2c);
      iVar3 = iVar3 + 1;
      *(int *)(iVar11 + 0x24) = *(int *)(iVar11 + 0x28) + iVar4;
    } while (iVar3 < 5);
    FUN_002182a0(local_d8,uVar10,bVar1,psVar8 + 0x34);
    iVar3 = 0;
    piVar7 = local_d8;
    do {
      iVar11 = param_1 + (idx11 + 7 + iVar3) * 0x14;
      piVar9 = (int *)(iVar11 + 0x2c);
      iVar6 = *piVar7;
      iVar4 = param_1 + (idx5 + 1 + iVar3) * 0xc;
      if ((iVar6 < 0) || (0xc < iVar6)) {
        if (iVar6 < 0xe) {
          iVar11 = *piVar9;
        }
        else {
          if (iVar6 < 0x1b) {
            FUN_000ffb3c(*piVar9,0);
            iVar6 = *piVar9;
            iVar11 = *(int *)(iVar11 + 0x30) + 1;
            *(int *)(iVar6 + 0x24) = iVar11;
            *(int *)(iVar6 + 0x28) = iVar11;
            (*(Widget **)(iVar4 + 0x850))->func1(0);
            uVar5 = FUN_001fea10(0xb,3,*piVar7 + -0xe);
            iVar11 = *(int *)(iVar4 + 0x850);
            *(undefined4 *)(iVar11 + 0xf8) = uVar5;
            *(undefined4 *)(iVar11 + 0x138) = 1;
            FUN_0010d4a8();
            goto LAB_001abc34;
          }
          iVar11 = *piVar9;
        }
        FUN_000ffb3c(iVar11,1);
        (*(Widget **)(iVar4 + 0x850))->func1(1);
      }
      else {
        FUN_000ffb3c(*piVar9,0);
        uVar5 = *(undefined4 *)(iVar11 + 0x30);
        iVar11 = *piVar9;
        *(undefined4 *)(iVar11 + 0x24) = uVar5;
        *(undefined4 *)(iVar11 + 0x28) = uVar5;
        (*(Widget **)(iVar4 + 0x850))->func1(0);
        uVar5 = FUN_001fea10(0xb,3,*piVar7);
        iVar11 = *(int *)(iVar4 + 0x850);
        *(undefined4 *)(iVar11 + 0xf8) = uVar5;
        *(undefined4 *)(iVar11 + 0x138) = 1;
        FUN_0010d4a8();
      }
LAB_001abc34:
      iVar3 = iVar3 + 1;
      piVar7 = piVar7 + 1;
    } while (iVar3 < 4);
    param_1 = param_1 + idx5 * 0xc;
    iVar3 = FUN_0019d474();
    param_2 = param_2 * 4;
    iVar3 = *(int *)(param_2 + iVar3 + 0x3a4);
    if (iVar3 == 3) {
      iVar3 = *(int *)(param_1 + 0x850);
      *(int *)(iVar3 + 0xf8) = iVar2 + 0x10;
      *(undefined4 *)(iVar3 + 0x138) = 1;
      FUN_0010d4a8();
    }
    else if (iVar3 == 2) {
      int idx11 = param_3 * 11 + 60;
  int idx5 = param_3 * 5 + 30;
  iVar2 = FUN_0019d474();
      uVar5 = FUN_001fea10(0xc,0,(int)**(short **)(param_2 + iVar2 + 0x3b4));
      iVar2 = *(int *)(param_1 + 0x850);
      *(undefined4 *)(iVar2 + 0xf8) = uVar5;
      *(undefined4 *)(iVar2 + 0x138) = 1;
      FUN_0010d4a8();
    }
    else if (iVar3 == 1) {
      int idx11 = param_3 * 11 + 60;
  int idx5 = param_3 * 5 + 30;
  iVar2 = FUN_0019d474();
      uVar5 = FUN_001fea10(7,0,(int)**(short **)(param_2 + iVar2 + 0x3b4));
      iVar2 = *(int *)(param_1 + 0x850);
      *(undefined4 *)(iVar2 + 0xf8) = uVar5;
      *(undefined4 *)(iVar2 + 0x138) = 1;
      FUN_0010d4a8();
    }
    else if (iVar3 == 0) {
      iVar2 = FUN_0016bb7c();
      iVar3 = *(int *)(param_1 + 0x850);
      *(int *)(iVar3 + 0xf8) = iVar2 + 0x20;
      *(undefined4 *)(iVar3 + 0x138) = 1;
      FUN_0010d4a8();
    }
    FUN_000d7e30(auStack_c8,0xffffffff);
  }
  return;
}


}

// Differing words: 336 / 363 words. First at +0x24.
// Ruled out:
// - Frame size 304 bytes matches retail.
// - Precomputed indices idx11 = param_3 * 11 + 60 and idx5 = param_3 * 5 + 30 match the prologue multiplication sequence.
// - Prototype FUN_0024dd40(float, int) restores lwc1  and lw a0.
// - Control flow includes small switch for param_2 (cases 0, 1, 2, 3), and loop structure over Widget objects with virtual calls at offset 12.
// near miss from lane dsn4 (unreviewed)
