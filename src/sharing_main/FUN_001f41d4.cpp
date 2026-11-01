/* HSTMATCH module=hstp_trans_main.prx start=0x001f41d4 end=0x001f4334 symbol=FUN_001f41d4 lang=c++ */
/* Clone of src/eboot/FUN_001e5fbc.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

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
extern void FUN_00105470(...);
extern void FUN_001f4334(...);
extern void FUN_001f4394(...);
}
struct VC_0 { virtual void d0(); virtual void m(int); };

extern "C" void FUN_001f41d4(int param_1,undefined4 param_2,int param_3,undefined4 param_4,undefined4 param_5, undefined4 param_6,int param_7,int param_8,int iStackX_0,int iStackX_4, undefined1 uStackX_8)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  ((VC_0 *)(*(int **)(param_1 + 0x1968)))->m((int)(0));
  FUN_00105470(*(undefined4 *)(param_1 + 0x1a70),0);
  *(int *)(*(int *)(param_1 + 0x1a70) + 0x24) =
       *(int *)(*(int *)(param_1 + 0x1a70) + 0x28) + iStackX_0;
  FUN_00105470(*(undefined4 *)(param_1 + 0x1a48),0);
  *(int *)(*(int *)(param_1 + 0x1a48) + 0x24) =
       *(int *)(*(int *)(param_1 + 0x1a48) + 0x28) + iStackX_4;
  int off = param_3 * 12;
  iVar1 = off + param_1;
  int *pList = (int *)(iVar1 + 0x1abc);
  iVar3 = *pList;
  for (iVar4 = 0; iVar4 < *(int *)(iVar3 + 0x14); iVar4++) {
    FUN_00105470(*(undefined4 *)(*(int *)(iVar3 + 0x10) + iVar4 * 4),iVar4 != param_7);
  }
  iVar1 = *(int *)(*(int *)(*pList + 0x10) + param_7 * 4);
  *(int *)(iVar1 + 0x24) = *(int *)(iVar1 + 0x28) + param_8;
  FUN_001f4394(param_1,param_5,param_4,param_6);
  FUN_001f4334(param_1,uStackX_8);
  return;
}
