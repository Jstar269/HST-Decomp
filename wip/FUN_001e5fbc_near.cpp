/* HSTMATCH module=EBOOT.BIN start=0x001e5fbc end=0x001e611c symbol=FUN_001e5fbc lang=c++ */
#pragma opt_strength_reduction off
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef unsigned char uchar;
typedef unsigned long long undefined8;
typedef int code(...);
#define ABS(x) ((x) < 0 ? -(x) : (x))
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

extern "C" {
extern void FUN_000ffb3c(...);
extern void FUN_001e611c(...);
extern void FUN_001e617c(...);
}
struct VC_0 { virtual void d0(); virtual void m(int); };

extern "C" void FUN_001e5fbc(int param_1,undefined4 param_2,int param_3,undefined4 param_4,undefined4 param_5, undefined4 param_6,int param_7,int param_8,int iStackX_0,int iStackX_4, undefined1 uStackX_8)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  ((VC_0 *)(*(int **)(param_1 + 0x1968)))->m((int)(0));
  FUN_000ffb3c(*(undefined4 *)(param_1 + 0x1a70),0);
  *(int *)(*(int *)(param_1 + 0x1a70) + 0x24) =
       *(int *)(*(int *)(param_1 + 0x1a70) + 0x28) + iStackX_0;
  FUN_000ffb3c(*(undefined4 *)(param_1 + 0x1a48),0);
  *(int *)(*(int *)(param_1 + 0x1a48) + 0x24) =
       *(int *)(*(int *)(param_1 + 0x1a48) + 0x28) + iStackX_4;
  iVar1 = param_3 * 0xc + param_1;
  int *pList = (int *)(iVar1 + 0x1abc);
  iVar3 = *pList;
  for (iVar4 = 0; iVar4 < *(int *)(iVar3 + 0x14); iVar4++) {
    FUN_000ffb3c(*(undefined4 *)(*(int *)(iVar3 + 0x10) + iVar4 * 4),iVar4 != param_7);
  }
  iVar1 = *(int *)(*(int *)(*pList + 0x10) + param_7 * 4);
  *(int *)(iVar1 + 0x24) = *(int *)(iVar1 + 0x28) + param_8;
  FUN_001e617c(param_1,param_5,param_4,param_6);
  FUN_001e611c(param_1,uStackX_8);
  return;
}
// near miss from lane dsn3 (unreviewed)
