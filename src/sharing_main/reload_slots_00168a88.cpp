/* HSTMATCH module=hstp_trans_main.prx start=0x00168a88 end=0x00168dfc symbol=FUN_00168a88 lang=c++ */
/* Hand-written reconstruction: clone of the source of reload_slots_0015c4e0.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef unsigned int undefined4;
extern "C" {
extern char DAT_00385a3c[];
extern char DAT_00385a64[];
extern char DAT_00385a80[];
extern char DAT_00385a98[];
extern char DAT_00385ab4[];
extern int DAT_003b7ebc;
extern unsigned char DAT_003eaad8;
extern int FUN_00000bcc(...);
extern int FUN_00014934(...);
extern void FUN_000143b0(...);
extern void FUN_000149a8(...);
extern void FUN_0001c684(int *, int, const char *, ...);
extern void FUN_0001c73c(...);
extern undefined4 FUN_000518d4(...);
extern void FUN_00051ad8(...);
extern void FUN_00051d38(float, ...);
extern void FUN_00166c04(...);
extern void FUN_001c16b4(...);
}
struct VC { virtual ~VC(); };

static inline void reload(char *param_1, int slot, int sub, int fields)
{
  undefined4 uVar2;

  if (*(int *)(param_1 + slot) != 0) {
    delete (VC *)*(int **)(param_1 + slot);
    *(int *)(param_1 + slot) = 0;
  }
  uVar2 = FUN_00000bcc(0x48);
  if (uVar2 != 0) {
    uVar2 = FUN_000518d4(uVar2);
  }
  *(undefined4 *)(param_1 + slot) = uVar2;
  FUN_00051ad8(uVar2, param_1 + 0x370, 0);
  FUN_00051d38(3000.0f, *(undefined4 *)(param_1 + slot));
  FUN_000149a8(param_1 + sub, param_1 + 0x370);
  *(int *)(param_1 + fields) = 0;
  *(int *)(param_1 + fields + 4) = 0;
  *(int *)(param_1 + fields + 8) = 0;
  *(int *)(param_1 + fields + 0xc) = 0x3f800000;
}

extern "C" void FUN_00168a88(char *param_1, int param_2)
{
  FUN_000143b0(param_1 + 0x370, DAT_00385a3c, param_2);
  FUN_0001c684(&DAT_003b7ebc, 1, param_1 + 0x370);
  if (param_2 == 0) {
    FUN_000143b0(param_1 + 0x370, DAT_00385a64);
    if (*(int *)(param_1 + 0x34) == 0 || FUN_00014934(param_1 + 0x70, param_1 + 0x370) != 0) {
      reload(param_1, 0x34, 0x70, 0x40);
    }
    FUN_000143b0(param_1 + 0x370, DAT_00385a80);
    if (*(int *)(param_1 + 0x38) == 0 || FUN_00014934(param_1 + 0x170, param_1 + 0x370) != 0) {
      reload(param_1, 0x38, 0x170, 0x50);
    }
  }
  else {
    FUN_000143b0(param_1 + 0x370, DAT_00385a98);
    if (*(int *)(param_1 + 0x34) == 0 || FUN_00014934(param_1 + 0x70, param_1 + 0x370) != 0) {
      reload(param_1, 0x34, 0x70, 0x40);
    }
    FUN_000143b0(param_1 + 0x370, DAT_00385ab4);
    if (*(int *)(param_1 + 0x38) == 0 || FUN_00014934(param_1 + 0x170, param_1 + 0x370) != 0) {
      reload(param_1, 0x38, 0x170, 0x50);
    }
    FUN_00166c04(param_1);
    FUN_001c16b4(&DAT_003eaad8, 0);
  }
  FUN_0001c73c(&DAT_003b7ebc);
}
