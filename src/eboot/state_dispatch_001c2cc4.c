/* HSTMATCH module=EBOOT.BIN start=0x001c2cc4 end=0x001c2e3c symbol=FUN_001c2cc4 lang=c99 */
/* Ghidra pseudocode (hstgen.py tag a7/v21) corrected by hand: guard written as the mirrored relational operator so the 0x94 load leads, FUN_001c38e0 declared int with typed pointer parameters (NOTES 0xd9d84: typed pointer args fix the argument-setup order and leave addiu a3 in the jal delay), and the iVar2 3/2/1 chain rewritten as a switch whose cases are listed in reverse; exact match verified; names and types await human review. */
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

extern undefined4 DAT_00341d84;
extern undefined4 DAT_0034a2e0;
extern void FUN_001c38c0();
extern int FUN_001c38e0(char *, char *, char *, char *);
extern void FUN_001c9750();
extern void FUN_001cd88c();
extern void FUN_001fcfc0();
extern void FUN_001fd0e4();
extern undefined4 FUN_001fd154();
extern void FUN_00225d54();
extern void FUN_00225d98();

void FUN_001c2cc4(int param_1)
{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (*(int *)(param_1 + 8) == 0) {
    FUN_00225d54(DAT_0034a2e0,0,0);
    FUN_00225d98(DAT_0034a2e0);
  }
  if (*(int *)(param_1 + 8) >= *(int *)(param_1 + 0x94)) {
    if ((*(int *)(param_1 + 0x94) == *(int *)(param_1 + 8)) &&
       (iVar2 = FUN_001c38e0((char *)param_1,(char *)param_1 + 0x98,(char *)param_1 + 0xa8,(char *)param_1 + 0xac), iVar2 != 0)) {
      if (*(int *)(param_1 + 0xa8) != 0) {
        *(undefined1 *)(*(int *)(param_1 + 0xa8) + 0x134) = 0;
      }
      iVar2 = *(int *)(param_1 + 0xac);
      if (iVar2 != 0) {
        *(undefined1 *)(iVar2 + 0x134) = 0;
      }
      uVar3 = *(undefined4 *)(param_1 + 0x18);
      FUN_001fcfc0(uVar3,param_1 + 0x98,*(undefined4 *)(param_1 + 0xa8),
                   *(undefined4 *)(param_1 + 0xac));
    }
    bVar1 = false;
    iVar2 = FUN_001fd154();
    switch (iVar2) {
    case 1:
      *(undefined1 *)(param_1 + 0x90) = 1;
      *(undefined1 *)(param_1 + 0x91) = 1;
      bVar1 = true;
      break;
    case 2:
      *(undefined1 *)(param_1 + 0x90) = 1;
      *(undefined1 *)(param_1 + 0x91) = 0;
      bVar1 = true;
      break;
    case 3:
      *(undefined1 *)(param_1 + 0x90) = 0;
      bVar1 = true;
      break;
    }
    if (bVar1) {
      FUN_001fd0e4();
      FUN_001cd88c(param_1,0,1);
      if (*(int *)(param_1 + 0xa8) != 0) {
        *(undefined1 *)(*(int *)(param_1 + 0xa8) + 0x134) = 1;
      }
      iVar2 = *(int *)(param_1 + 0xac);
      if (iVar2 != 0) {
        *(undefined1 *)(iVar2 + 0x134) = 1;
      }
      FUN_001c38c0(param_1,0);
      FUN_001c9750(*(undefined4 *)(param_1 + 0x18));
      DAT_00341d84 = 0;
    }
  }
  return;
}
