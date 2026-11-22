/* HSTMATCH module=EBOOT.BIN start=0x001faa10 end=0x001fabb4 symbol=FUN_001faa10 lang=c99 */
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

extern undefined4 DAT_0034a678;
extern void FUN_0015d1e8(uint param_1);
extern undefined4 FUN_0015d3fc(void);
extern int FUN_0016bb88(void);
extern int FUN_0022c484(void);
extern int FUN_0024e064(void);
extern int FUN_0024e230(int param_1,int param_2,int param_3,int param_4);
extern int FUN_002acd28(int param_1);
extern int FUN_002ada80(int param_1);

void FUN_001faa10(int param_1)
{
  int iVar2;
  bool bVar1;
  int iVar3;
  byte *pbVar4;
  undefined4 uVar5;
  int iVar6;
  
  iVar2 = FUN_0016bb88();
  if (((DAT_0034a678 == 0x2a) && (*(uchar *)(param_1 + 0x521) == '\0')) &&
     (iVar6 = FUN_0022c484(), iVar6 >= 3)) {
    FUN_0015d1e8((uint)(0));
  }
  bVar1 = true;
  iVar6 = 0;
  do {
    iVar3 = FUN_002acd28((int)(iVar6));
    if (iVar3 != 0) {
      if (iVar6 >= 0 && iVar6 < 0x28) pbVar4 = (byte *)(iVar2 + 0x1058); else pbVar4 = (byte *)0;
      if ((uint)*pbVar4 - 4 > 1) {
        bVar1 = false;
      }
    }
    iVar6 = iVar6 + 1;
    iVar2 = iVar2 + 0x14;
  } while (iVar6 < 0x28);
  if (bVar1) {
    FUN_0015d1e8((uint)(3));
  }
  bVar1 = true;
  iVar2 = 0;
  do {
    iVar6 = 0;
    do {
      uVar5 = FUN_0024e064();
      uVar5 = FUN_0024e230((int)(uVar5),(int)(iVar2 != 0),(int)(iVar6),(int)(3));
      iVar3 = FUN_002ada80((int)(uVar5));
      if (((iVar3 != 3) && (iVar3 != 4)) && (iVar3 != 5)) {
        bVar1 = false;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < 0xf);
    iVar2 = iVar2 + 1;
  } while (iVar2 < 2);
  if (bVar1) {
    FUN_0015d1e8((uint)(4));
  }
  iVar2 = FUN_0015d3fc();
  if (iVar2 != 0) {
    *(undefined1 *)(param_1 + 0x5a2) = 1;
    *(undefined1 *)(param_1 + 0x59f) = 0;
    *(undefined1 *)(param_1 + 0x5a0) = 0;
    *(undefined1 *)(param_1 + 0x5a1) = 0;
  }
  return;
}
// near miss api-hstm-20261121: 105/105 words, 6 differ (was 25). Fixes found: iVar2 declared before bVar1; the
// pbVar4 chain as `if (i >= 0 && i < 40) p = base+0x1058; else p = 0;` (one shared zero arm, no beqzl dup); `(uint)*p - 4 > 1`
// gives sltiu; `iVar6 >= 3` gives slti v0; field +0x521 is uchar.
// Left: the hoisted invariant `iVar2 != 0` (sltu) is s4 and the second-phase flag s1; retail has t=s1, flag=s4.
// Ruled out: separate flag locals (13 diffs), named `nz` local of any type (copy-propagated, same 6), `> 0`/`?:` forms (no hoist),
// for/do loop spellings, all 720 declaration orders of the six locals.
// improved by lane api-hstm-20261121 (unreviewed): 25 -> 6 differing words
