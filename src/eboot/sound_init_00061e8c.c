/* HSTMATCH module=EBOOT.BIN start=0x00061e8c end=0x00062124 symbol=FUN_00061e8c lang=c99 */
/* Hand-written reconstruction from the disassembly (lane sy2 near miss wip/init_audio_slots_00061e8c.c, finished by lane fs1 with the inclusive loop bound found on the Sharing twin 0x00062d10); exact match verified; names and types await human review. */
typedef unsigned int u32;
extern unsigned char DAT_00331878;
typedef struct { float r, g, b, a, u, v; } Ent;
extern Ent DAT_00331880[]; extern u32 DAT_0033187c;
extern int DAT_002d1058, DAT_002d105c, DAT_002d1060, DAT_002d1064, DAT_002d1068;
extern int DAT_002d106c, DAT_002d1070, DAT_002d1074, DAT_002d1078;
extern int FUN_002b7c18(int); extern void FUN_00062124(const char *, ...); extern int FUN_0000ef40();
extern int FUN_00070390(); extern int FUN_000703a0(); extern int FUN_000703b0(); extern void FUN_00061e74(void);
extern int FUN_0006fd04(); extern void FUN_00078bf4(); extern void FUN_000790d0();
extern int FUN_000868ec(); extern int FUN_000869c0(); extern int FUN_00086a20();
extern int FUN_000104b0(); extern int FUN_000104e0();
extern char DAT_002d107c[], DAT_002d10a4[], DAT_002d10c8[], DAT_002d10f4[], DAT_002d1154[];
void FUN_00061e8c(void)
{
  int i, r;
  if (DAT_00331878 != 0) return;
  r = FUN_002b7c18(0x300);
  if (r < 0) { FUN_00062124(DAT_002d107c, r); FUN_0000ef40(1); }
  r = FUN_002b7c18(0x301);
  if (r < 0) { FUN_00062124(DAT_002d10a4, r); FUN_0000ef40(1); }
  r = FUN_002b7c18(0x302);
  if (r < 0) { FUN_00062124(DAT_002d10c8, r); FUN_0000ef40(1); }
  FUN_00070390(FUN_000104b0);
  FUN_000703a0(FUN_000104e0);
  FUN_000703b0(FUN_00061e74);
  if (FUN_0006fd04(DAT_002d10f4) == 0) { FUN_00062124(DAT_002d1154); FUN_0000ef40(1); }
  DAT_00331878 = 1;
  FUN_00078bf4(DAT_002d1058, DAT_002d1058);
  FUN_000790d0(3, DAT_002d105c, DAT_002d105c);
  FUN_000790d0(2, DAT_002d1060, DAT_002d1060);
  FUN_000790d0(4, DAT_002d1064, DAT_002d1064);
  FUN_000790d0(9, DAT_002d1068, DAT_002d1068);
  for (i = 0; i <= 31; i++) {
    DAT_00331880[i].r = 1.0f;
    DAT_00331880[i].g = 1.0f;
    DAT_00331880[i].b = 1.0f;
    DAT_00331880[i].a = 1.0f;
    DAT_00331880[i].u = 0.0f;
    DAT_00331880[i].v = 0.0f;
  }
  FUN_000868ec(DAT_0033187c, DAT_002d106c);
  FUN_000869c0(DAT_0033187c, DAT_002d1070, DAT_002d1070);
  FUN_00086a20(DAT_0033187c, 0, DAT_002d1074);
  FUN_00086a20(DAT_0033187c, 1, DAT_002d1078);
}
