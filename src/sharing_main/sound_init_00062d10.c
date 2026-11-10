/* HSTMATCH module=hstp_trans_main.prx start=0x00062d10 end=0x00062fa8 symbol=FUN_00062d10 lang=c99 */
/* Hand-written reconstruction from the disassembly (follows the EBOOT-twin near miss wip/init_audio_slots_00061e8c.c); exact match verified; names and types await human review. Loads the CODEC/SAS/ATRAC3plus modules, installs the SGX allocator callbacks, initialises SGX and resets the 32 channel slots. */
typedef struct { float a, b, c, d, e, f; } Slot;

extern unsigned char DAT_003dabb8;
extern int DAT_003dabbc;
extern Slot DAT_003dabc0[32];
extern int DAT_00376f08, DAT_00376f0c, DAT_00376f10, DAT_00376f14, DAT_00376f18;
extern int DAT_00376f1c, DAT_00376f20, DAT_00376f24, DAT_00376f28;
extern char DAT_00376f2c[], DAT_00376f54[], DAT_00376f78[], DAT_00376fa4[], DAT_00377004[];

extern void FUN_00062fa8(const char *fmt, ...);
extern void FUN_0000ef40(int code);
extern int FUN_002c79b8(int id);
extern void *FUN_000104b0(unsigned n);
extern void FUN_000104e0(void *p);
extern int FUN_00062cf8(void);
extern void FUN_00071b50(void *(*f)(unsigned));
extern void FUN_00071b60(void (*f)(void *));
extern void FUN_00071b70(int (*f)(void));
extern int FUN_000714c4(const char *cfg);
extern void FUN_0007a3b4(int a, int b);
extern void FUN_0007a890(int ch, int a, int b);
extern void FUN_000880ac(int h, int a);
extern void FUN_00088180(int h, int a, int b);
extern void FUN_000881e0(int h, int i, int a);

void FUN_00062d10(void)
{
  int r;
  int i;
  if (DAT_003dabb8 != 0)
    return;
  r = FUN_002c79b8(0x300);
  if (r < 0) {
    FUN_00062fa8(DAT_00376f2c, r);
    FUN_0000ef40(1);
  }
  r = FUN_002c79b8(0x301);
  if (r < 0) {
    FUN_00062fa8(DAT_00376f54, r);
    FUN_0000ef40(1);
  }
  r = FUN_002c79b8(0x302);
  if (r < 0) {
    FUN_00062fa8(DAT_00376f78, r);
    FUN_0000ef40(1);
  }
  FUN_00071b50(FUN_000104b0);
  FUN_00071b60(FUN_000104e0);
  FUN_00071b70(FUN_00062cf8);
  if (FUN_000714c4(DAT_00376fa4) == 0) {
    FUN_00062fa8(DAT_00377004);
    FUN_0000ef40(1);
  }
  DAT_003dabb8 = 1;
  FUN_0007a3b4(DAT_00376f08, DAT_00376f08);
  FUN_0007a890(3, DAT_00376f0c, DAT_00376f0c);
  FUN_0007a890(2, DAT_00376f10, DAT_00376f10);
  FUN_0007a890(4, DAT_00376f14, DAT_00376f14);
  FUN_0007a890(9, DAT_00376f18, DAT_00376f18);
  for (i = 0; i <= 31; i++) {
    DAT_003dabc0[i].a = 1.0f;
    DAT_003dabc0[i].b = 1.0f;
    DAT_003dabc0[i].c = 1.0f;
    DAT_003dabc0[i].d = 1.0f;
    DAT_003dabc0[i].e = 0.0f;
    DAT_003dabc0[i].f = 0.0f;
  }
  FUN_000880ac(DAT_003dabbc, DAT_00376f1c);
  FUN_00088180(DAT_003dabbc, DAT_00376f20, DAT_00376f20);
  FUN_000881e0(DAT_003dabbc, 0, DAT_00376f24);
  FUN_000881e0(DAT_003dabbc, 1, DAT_00376f28);
}
