/* HSTMATCH module=hstp_trans_main.prx start=0x001d1050 end=0x001d11c8 symbol=FUN_001d1050 lang=c99 */
/* Clone of src/eboot/state_dispatch_001c2cc4.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag a7/v21) corrected by hand: guard written as the mirrored relational operator so the 0x94 load leads, FUN_001d1c6c declared int with typed pointer parameters (NOTES 0xd9d84: typed pointer args fix the argument-setup order and leave addiu a3 in the jal delay), and the iVar2 3/2/1 chain rewritten as a switch whose cases are listed in reverse; exact match verified; names and types await human review. */
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

extern undefined4 DAT_003eb0bc;
extern undefined4 DAT_003f3610;
extern void FUN_001d1c4c();
extern int FUN_001d1c6c(char *, char *, char *, char *);
extern void FUN_001d7be4();
extern void FUN_001dbf68();
extern void FUN_0020b738();
extern void FUN_0020b8b4();
extern undefined4 FUN_0020b924();
extern void FUN_002352c8();
extern void FUN_0023530c();

void FUN_001d1050(int param_1)
{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (*(int *)(param_1 + 8) == 0) {
    FUN_002352c8(DAT_003f3610,0,0);
    FUN_0023530c(DAT_003f3610);
  }
  if (*(int *)(param_1 + 8) >= *(int *)(param_1 + 0x94)) {
    if ((*(int *)(param_1 + 0x94) == *(int *)(param_1 + 8)) &&
       (iVar2 = FUN_001d1c6c((char *)param_1,(char *)param_1 + 0x98,(char *)param_1 + 0xa8,(char *)param_1 + 0xac), iVar2 != 0)) {
      if (*(int *)(param_1 + 0xa8) != 0) {
        *(undefined1 *)(*(int *)(param_1 + 0xa8) + 0x134) = 0;
      }
      iVar2 = *(int *)(param_1 + 0xac);
      if (iVar2 != 0) {
        *(undefined1 *)(iVar2 + 0x134) = 0;
      }
      uVar3 = *(undefined4 *)(param_1 + 0x18);
      FUN_0020b738(uVar3,param_1 + 0x98,*(undefined4 *)(param_1 + 0xa8),
                   *(undefined4 *)(param_1 + 0xac));
    }
    bVar1 = false;
    iVar2 = FUN_0020b924();
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
      FUN_0020b8b4();
      FUN_001dbf68(param_1,0,1);
      if (*(int *)(param_1 + 0xa8) != 0) {
        *(undefined1 *)(*(int *)(param_1 + 0xa8) + 0x134) = 1;
      }
      iVar2 = *(int *)(param_1 + 0xac);
      if (iVar2 != 0) {
        *(undefined1 *)(iVar2 + 0x134) = 1;
      }
      FUN_001d1c4c(param_1,0);
      FUN_001d7be4(*(undefined4 *)(param_1 + 0x18));
      DAT_003eb0bc = 0;
    }
  }
  return;
}
