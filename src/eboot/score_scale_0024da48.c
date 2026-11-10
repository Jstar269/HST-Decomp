/* HSTMATCH module=EBOOT.BIN start=0x0024da48 end=0x0024dd40 symbol=FUN_0024da48 lang=c99 rodata=0x002ca5d0 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_002fbe08[], DAT_002fbe14[], DAT_002fbe1c[], DAT_002fbe28[], DAT_002fbe30[], DAT_002fbe3c[];
extern char DAT_002fbe44[], DAT_002fbe50[], DAT_002fbe5c[], DAT_002fbe68[], DAT_002fbe70[], DAT_002fbe78[];
extern char DAT_002fbe80[], DAT_002fbe88[], DAT_002fbe90[], DAT_002fbe9c[], DAT_002fbea8[], DAT_002fbeb0[];
extern char DAT_002fbebc[], DAT_002fbec4[], DAT_002fbecc[];
extern float FUN_000657e4(void *, const char *, int);
extern void FUN_0024d8dc(...);

float FUN_0024da48(void *cfg, int sets, int game, int score)
{
  float setMin = 0.0f;
  float setDiv = 0.0f;
  float win = 0.0f;
  float g;
  float k;
  float b;
  float result;

  switch (sets) {
  case 1:
    setMin = FUN_000657e4(cfg, DAT_002fbe08, 1);
    setDiv = FUN_000657e4(cfg, DAT_002fbe14, 1);
    break;
  case 3:
    setMin = FUN_000657e4(cfg, DAT_002fbe1c, 1);
    setDiv = FUN_000657e4(cfg, DAT_002fbe28, 1);
    break;
  case 5:
    setMin = FUN_000657e4(cfg, DAT_002fbe30, 1);
    setDiv = FUN_000657e4(cfg, DAT_002fbe3c, 1);
    break;
  }
  g = (float)game;
  switch (score) {
  case 0:
    win = FUN_000657e4(cfg, DAT_002fbe44, 1);
    break;
  case 1:
    win = FUN_000657e4(cfg, DAT_002fbe50, 1);
    break;
  case 2:
    win = FUN_000657e4(cfg, DAT_002fbe5c, 1);
    break;
  case 3:
    win = FUN_000657e4(cfg, DAT_002fbe68, 1);
    break;
  case 4:
    win = FUN_000657e4(cfg, DAT_002fbe70, 1);
    break;
  case 5:
    win = FUN_000657e4(cfg, DAT_002fbe78, 1);
    break;
  }
  k = FUN_000657e4(cfg, DAT_002fbe80, 1);
  b = FUN_000657e4(cfg, DAT_002fbe88, 1);
  result = win * (b * (setDiv * (k + g * setMin)));
  FUN_0024d8dc(DAT_002fbe90, (double)g);
  FUN_0024d8dc(DAT_002fbe9c, (double)setMin);
  FUN_0024d8dc(DAT_002fbea8, (double)k);
  FUN_0024d8dc(DAT_002fbeb0, (double)setDiv);
  FUN_0024d8dc(DAT_002fbebc, (double)b);
  FUN_0024d8dc(DAT_002fbec4, (double)win);
  FUN_0024d8dc(DAT_002fbecc, (double)result);
  return result;
}
