/* HSTMATCH module=EBOOT.BIN start=0x0014d1cc end=0x0014d25c symbol=FUN_0014d1cc lang=c99 helpers=FUN_0014c610@0x0014c610-0x0014c63c */
/* Hand-written reconstruction from the disassembly (wip near miss FUN_0014d1cc_near as hint); exact match verified; the empty variadic log stub FUN_0014c610 is a same-file static placed by helpers=; matching idiom: the debug flag is tested twice (nested), as retail's two beqz do; names and types await human review. */
extern unsigned char DAT_003402f0;
extern char DAT_002dc240[];
extern char DAT_002dc24c[];
extern char DAT_002dc250[];
extern char DAT_002dc254[];

static void FUN_0014c610(...) { }

int FUN_0014d1cc(int param_1, int *param_2)
{
  int i;
  if (DAT_003402f0 != 0) {
    if (DAT_003402f0 != 0) {
      FUN_0014c610(DAT_002dc240);
      for (i = 0; i < param_2[6] + 1; i++) {
        FUN_0014c610(DAT_002dc24c);
      }
    }
    FUN_0014c610(DAT_002dc250);
  }
  FUN_0014c610(*(int *)(param_1 + 8));
  FUN_0014c610(DAT_002dc254);
  return 0;
}
