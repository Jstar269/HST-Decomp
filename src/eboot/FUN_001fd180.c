/* HSTMATCH module=EBOOT.BIN start=0x001fd180 end=0x001fd2e0 symbol=FUN_001fd180 lang=c99 */
/* Ghidra pseudocode (hstgen.py tag gen-a2/v0c) corrected by hand: the three 0xffff halfword stores are signed short -1 (retail materialises addiu v0,zero,-1, not ori 0xffff); each sub-object initialiser hoists its base into one reused pointer and stores the vtable through it; that store is volatile-qualified (matching idiom: volatile single-word store keeps it ahead of the following call so the address stays in v0 instead of being CSE'd into a callee-saved register); and the clear-loop pointer is declared before its counter so the pointer takes a0 (MWCC-NOTES 0xe3944); exact match verified; names and types await human review. */
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef unsigned char uchar;
typedef unsigned char bool;
typedef unsigned long long undefined8;
typedef int code();
#define true 1
#define false 0
#define ABS(x) ((x) < 0 ? -(x) : (x))
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

extern undefined DAT_003070bc;
extern undefined DAT_003095b0;
extern undefined4 DAT_0034a13c;
extern void FUN_000007f4();
extern uint * FUN_000114c0();
extern undefined4 * FUN_0001f06c();
extern void FUN_0001f21c();
extern int * FUN_000649c0();
extern undefined4 * FUN_0008c230();
extern int FUN_001ff69c();
extern undefined1 * FUN_002019fc();
extern void FUN_0005b670();
extern void FUN_0005ba00();

undefined4 * FUN_001fd180(undefined4 *param_1,int param_2)
{
  undefined4 *puVar2;
  int iVar1;
  undefined4 *p;
  
  FUN_0001f06c(param_1);
  *param_1 = (unsigned int)(&DAT_003095b0);
  FUN_000114c0(param_1 + 0x16,0,0x6c);
  *(short *)(param_1 + 0x30) = -1;
  *(short *)((int)param_1 + 0xc2) = -1;
  *(short *)(param_1 + 0x1f) = -1;
  FUN_001ff69c((int)(param_1 + 0x32));
  FUN_000007f4((int)(param_1 + 0x334),FUN_0005b670,FUN_0005ba00,0x2c0,2);
  FUN_002019fc((undefined1 *)(param_1 + 0x494));
  p = param_1 + 0x4a5;
  FUN_000649c0(p);
  *(volatile undefined4 *)p = (unsigned int)(&DAT_003070bc);
  FUN_0008c230(p + 4);
  FUN_0008c230(p + 8);
  p = param_1 + 0x4b1;
  FUN_000649c0(p);
  *(volatile undefined4 *)p = (unsigned int)(&DAT_003070bc);
  FUN_0008c230(p + 4);
  FUN_0008c230(p + 8);
  puVar2 = param_1 + 0x4bd;
  iVar1 = 0;
  do {
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2[2] = 0;
    puVar2[3] = 0;
    puVar2[4] = 0;
    puVar2[5] = 0;
    iVar1 = iVar1 + 7;
    puVar2[6] = 0;
    puVar2 = puVar2 + 7;
  } while (iVar1 < 0x15);
  DAT_0034a13c = (unsigned int)(param_1);
  param_1[0xf] = 0;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  *(undefined1 *)(param_1 + 0x31) = 0;
  param_1[0x13] = 1;
  param_1[0x14] = 0;
  param_1[0x15] = 0;
  FUN_0001f21c(param_2,(int)param_1);
  *(undefined1 *)(param_1 + 0x4a4) = 0;
  *(undefined1 *)((int)param_1 + 0x1291) = 0;
  *(undefined1 *)((int)param_1 + 0x1292) = 1;
  return param_1;
}
