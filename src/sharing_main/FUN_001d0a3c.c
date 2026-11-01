/* HSTMATCH module=hstp_trans_main.prx start=0x001d0a3c end=0x001d0b9c symbol=FUN_001d0a3c lang=c99 */
/* Clone of src/eboot/FUN_001c2714.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned int undefined4;

extern undefined DAT_002d10cc;
extern undefined DAT_002d10d0;
extern undefined DAT_002d10d4;
extern undefined DAT_00391ff0;
extern void FUN_0019709c(undefined4, const void *);
extern undefined1 * FUN_001991d4(int);
extern void FUN_001dbf68(int, int, int);
extern undefined4 FUN_00238cf0(int, int, int);

void FUN_001d0a3c(int param_1,int param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5, undefined4 *param_6)
{
  int iVar1;
  undefined *puVar2;
  
  iVar1 = (int)(FUN_001991d4(8));
  *(undefined1 *)(param_1 + 99) = 0;
  *(undefined1 *)(param_1 + 100) = 0;
  if (*(int *)(iVar1 + 4) == 0) {
    puVar2 = &DAT_002d10d4;
  }
  else if (*(int *)(*(int *)(iVar1 + 8) + 8) < 1) {
    puVar2 = &DAT_002d10d0;
  }
  else if ((param_2 < 0) || (param_2 >= (*(int **)(iVar1 + 0xc))[1])) {
    puVar2 = &DAT_002d10cc;
  }
  else {
    puVar2 = *(undefined **)(**(int **)(iVar1 + 0xc) + param_2 * 4);
  }
  *(undefined **)(param_1 + 0x68) = puVar2;
  *(undefined4 *)(param_1 + 0x6c) = param_5;
  *(undefined4 *)(param_1 + 0x70) = 0;
  FUN_0019709c(*(undefined4 *)(param_1 + 0x20),&DAT_00391ff0);
  FUN_001dbf68(param_1,2,1);
  *(undefined4 *)(param_1 + 0x74) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x78) = 0xffffffff;
  FUN_00238cf0(param_1 + 0x34,0,param_2);
  *(undefined4 *)(param_1 + 0x74) = param_3;
  *(undefined4 *)(param_1 + 0x78) = param_4;
  {
    undefined4 tmp[4];
    undefined4 *s;

    if (param_6 != (undefined4 *)0x0) {
      s = param_6;
    } else {
      tmp[0] = 0;
      tmp[1] = 0;
      tmp[2] = 0;
      tmp[3] = 0;
      s = tmp;
    }
    *(float *)(param_1 + 0x80) = ((float *)s)[0];
    *(float *)(param_1 + 0x84) = ((float *)s)[1];
    *(float *)(param_1 + 0x88) = ((float *)s)[2];
    *(float *)(param_1 + 0x8c) = ((float *)s)[3];
  }
  return;
}
