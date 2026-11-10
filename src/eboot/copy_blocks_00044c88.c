/* HSTMATCH module=EBOOT.BIN start=0x00044c88 end=0x00044d40 symbol=FUN_00044c88 lang=c99 */
/* Clone of src/sharing_main/copy_blocks_00044e2c.c (hstclone.py tag twin11100306-EBOOT.BIN): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* steer=pragma:opt_propagation (off, scoped to FUN_00044c88 by #pragma push/pop; found by the 7-decomp pragma sweep; no natural source matched) */
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

extern undefined DAT_002bc048;
extern int FUN_00051464();

#pragma push
#pragma opt_propagation off
void FUN_00044c88(char *param_1,int param_2)
{
  undefined4 *puVar1;
  int iVar4;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar5;
  
  puVar1 = (undefined4 *)FUN_00051464(*(undefined4 *)(param_1 + 0x19074));
  iVar4 = 0;
  puVar3 = (undefined4 *)(&DAT_002bc048 + param_2 * 0x40);
  do {
    uVar5 = *puVar3;
    iVar4 = iVar4 + 8;
    puVar2 = puVar1 + 7;
    *puVar1 = uVar5;
    puVar1[1] = puVar3[1];
    puVar1[2] = puVar3[2];
    puVar1[3] = puVar3[3];
    puVar1[4] = puVar3[4];
    puVar1[5] = puVar3[5];
    puVar1[6] = puVar3[6];
    puVar1 = puVar1 + 8;
    *puVar2 = puVar3[7];
    puVar3 = puVar3 + 8;
  } while (iVar4 < 0x10);
  *(int *)(param_1 + 0x190b0) = param_2;
  return;
}
#pragma pop

// NEAR MISS: 2 words differ at +0x40 and +0x4c; retail loads the first copied word into v1, this source uses a0.
// near miss from lane u1r2-0 (unreviewed)
