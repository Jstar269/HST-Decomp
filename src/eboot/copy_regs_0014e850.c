/* HSTMATCH module=EBOOT.BIN start=0x0014e850 end=0x0014e958 symbol=FUN_0014e850 lang=c99 helpers=FUN_0014c610@0x0014c610-0x0014c63c */
/* Hand-written reconstruction from the disassembly (started from the lane-fm6 wip draft); the empty variadic log stub FUN_0014c610 is a static helper placed by helpers=; exact match verified; names and types await human review. Matching idiom: the trace-flag test is written twice (nested), as retail tests it twice. */
typedef struct {
  char pad0[8];
  signed char r[8];
} Cmd;

typedef struct {
  char pad0[0x18];
  int count;
} Ctx;

typedef struct {
  char pad0[0x20];
  int r[8];
} Regs;

extern unsigned char DAT_003402f0;
extern Regs DAT_003402f4;
extern char DAT_002dc240[];
extern char DAT_002dc24c[];
extern char DAT_002dc334[];

static void FUN_0014c610(...) { }

int FUN_0014e850(Cmd *cmd, Ctx *ctx)
{
  int regs[8];
  unsigned i;

  if (DAT_003402f0 != 0) {
    if (DAT_003402f0 != 0) {
      int j;
      FUN_0014c610(DAT_002dc240);
      for (j = 0; j < ctx->count + 1; j++)
        FUN_0014c610(DAT_002dc24c);
    }
    FUN_0014c610(DAT_002dc334, cmd->r[0], cmd->r[1], cmd->r[2], cmd->r[3], cmd->r[4], cmd->r[5], cmd->r[6], cmd->r[7]);
  }
  {
    unsigned k;
    int *s;
    int *d;
    for (i = 0; i < 8; i++) {
      int v = cmd->r[i];
      if (v < 0)
        v = i;
      regs[i] = DAT_003402f4.r[v];
    }
    k = 0;
    s = regs;
    d = (int *)&DAT_003402f4;
    do {
      d[8] = *s;
      s++;
      d++;
    } while (++k < 8);
  }
  return 0;
}
