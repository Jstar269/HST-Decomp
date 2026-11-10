/* HSTMATCH module=EBOOT.BIN start=0x00269798 end=0x00269ab0 symbol=FUN_00269798 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode corrected by hand (lane wip, then the final -1 fill loop as a block with its own counter declared before the pointer); exact match verified; names and types await human review. */
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef unsigned char uchar;
typedef unsigned long long undefined8;
typedef int code();
#define true 1
#define false 0
#define ABS(x) ((x) < 0 ? -(x) : (x))
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

extern "C" {
extern undefined DAT_00309380;
extern undefined DAT_00309ba8;
extern undefined * DAT_003415a8;
extern int DAT_003417a4;
extern char DAT_002fe05c[];
extern char DAT_002fe07c[];
extern char DAT_002fe084[];
extern char DAT_002fe090[];
extern char DAT_002fe09c[];
extern char DAT_002fe0a4[];
extern char DAT_002fe0b0[];
extern char DAT_002fe0b8[];
extern char DAT_002fe0c4[];
extern char DAT_002fe0cc[];
extern void FUN_000143b0(void *, const char *, ...);
extern undefined8 FUN_00020f5c(float);
extern int FUN_001026b8(int, void *);
extern undefined4 * FUN_00102e38();
extern int FUN_001039d8(void *, const char *, int);
extern void FUN_00104118(void *, const char *, int);
extern void FUN_0010d4a8();
extern int FUN_001bc630(float param_1);
extern void FUN_00269ab0(void *, int);
}

struct ControlView {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void Set(int);
};

struct MainView {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void Finish();
};

extern "C" char * FUN_00269798(char *param_1)
{
  uint uVar8;
  int iVar7;
  int iVar3;
  short sVar1;
  int halfValue;
  undefined4 uVar4;
  float fVar;
  int iVar5;
  int record;
  undefined8 uVar9;
  undefined1 auStack_40 [64];
  
  FUN_00102e38();
  *(int *)param_1 = (int)&DAT_00309380;
  *(undefined1 *)(param_1 + 0x54) = 0;
  *(undefined1 *)(param_1 + 0x55) = 0;
  *(int *)(param_1 + 0x58) = 0;
  *(int *)param_1 = (int)&DAT_00309ba8;
  FUN_00104118(param_1,DAT_002fe05c,0);
  iVar3 = FUN_001039d8(param_1,DAT_002fe07c,0);
  *(int *)(param_1 + 0x90) = iVar3;
  iVar3 = FUN_001039d8(param_1,DAT_002fe084,0);
  *(int *)(param_1 + 0x94) = iVar3;
  iVar3 = FUN_001039d8(param_1,DAT_002fe090,0);
  *(int *)(param_1 + 0x98) = iVar3;
  iVar3 = FUN_001026b8(*(int *)(param_1 + 0x94),DAT_002fe09c);
  uVar8 = 0;
  iVar7 = 0;
  *(int *)(param_1 + 0x58) = *(int *)(iVar3 + 0x20);
  iVar3 = 0;
  do {
    if ((int)uVar8 < 3) {
      FUN_000143b0(auStack_40,DAT_002fe0a4,uVar8);
      uVar4 = FUN_001026b8(*(int *)(param_1 + 0x94),auStack_40);
      *(undefined4 *)(param_1 + iVar7 + 0x9c) = uVar4;
    }
    else {
      FUN_000143b0(auStack_40,DAT_002fe0a4,uVar8 - 3);
      uVar4 = FUN_001026b8(*(int *)(param_1 + 0x98),auStack_40);
      *(undefined4 *)(param_1 + iVar7 + 0x9c) = uVar4;
    }
    record = *(int *)(param_1 + iVar7 + 0x9c);
    iVar5 = (int)*(short *)(record + 4);
    iVar5 += (int)*(short *)(record + 8);
    halfValue = iVar5 / 2;
    *(short *)(param_1 + iVar3 + 0xb4) = (short)halfValue;
    record = *(int *)(param_1 + iVar7 + 0x9c);
    iVar5 = (int)*(short *)(record + 6);
    iVar5 += (int)*(short *)(record + 10);
    halfValue = iVar5 / 2;
    *(short *)(param_1 + iVar3 + 0xc0) = (short)halfValue;
    *(undefined4 *)(param_1 + iVar7 + 0xcc) = 0x3f800000;
    uVar8 = uVar8 + 1;
    iVar7 = iVar7 + 4;
    iVar3 = iVar3 + 2;
  } while (uVar8 < 6);
  if (DAT_003415a8 != 0) {
    fVar = *(float *)((char *)DAT_003415a8 + 0x1414);
  }
  else {
    fVar = 0.01f;
  }
  iVar3 = FUN_001bc630(fVar);
  iVar7 = FUN_001026b8(*(int *)(param_1 + 0x94),DAT_002fe0b0);
  *(int *)(param_1 + 0xf4) = iVar7;
  int row = *(int *)(iVar7 + 0x24);
  row = row + iVar3;
  iVar3 = row;
  *(int *)(param_1 + 0xfc) = *(int *)(iVar7 + 0x24);
  iVar7 = *(int *)(param_1 + 0xf4);
  *(int *)(iVar7 + 0x24) = iVar3;
  *(int *)(iVar7 + 0x28) = iVar3;
  iVar3 = FUN_001026b8(*(int *)(param_1 + 0x94),DAT_002fe0b8);
  *(int *)(param_1 + 0x100) = *(int *)(iVar3 + 0x20);
  uVar9 = FUN_00020f5c(fVar);
  FUN_000143b0(param_1 + 0xe4,DAT_002fe0c4,(int)uVar9,(int)((ulonglong)uVar9 >> 0x20));
  iVar3 = *(int *)(param_1 + 0x100);
  *(int **)(iVar3 + 0xf8) = (int *)(param_1 + 0xe4);
  *(undefined4 *)(iVar3 + 0x138) = 1;
  FUN_0010d4a8();
  ((ControlView *)*(int *)(param_1 + 0x100))->Set(0x80000000);
  if (DAT_003417a4 < 9) {
    iVar3 = *(int *)(param_1 + 0x100);
    sVar1 = -(short)*(short *)(iVar3 + 0xfc);
    *(undefined2 *)(iVar3 + 0xfc) = 0;
    *(short *)(iVar3 + 0x100) = *(short *)(iVar3 + 0x100) + sVar1;
    sVar1 = *(short *)(iVar3 + 0xfe);
    *(undefined2 *)(iVar3 + 0xfe) = 4;
    sVar1 = (short)(4 - sVar1);
    *(short *)(iVar3 + 0x102) = *(short *)(iVar3 + 0x102) + sVar1;
    iVar3 = *(int *)(param_1 + 0x94);
  }
  else {
    iVar3 = *(int *)(param_1 + 0x94);
  }
  iVar3 = FUN_001026b8(iVar3,DAT_002fe0cc);
  *(int *)(param_1 + 0xf8) = iVar3;
  *(undefined1 *)(param_1 + 0x55) = 0;
  {
    unsigned i = 0;
    int *p = (int *)param_1;
    do { i++; p[24] = -1; p++; } while (i < 11);
  }
  FUN_00269ab0(param_1,4);
  ((MainView *)param_1)->Finish();
  return param_1;
}
