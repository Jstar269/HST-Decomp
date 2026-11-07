/* HSTMATCH module=hstp_trans_main.prx start=0x001fe964 end=0x001fec54 symbol=FUN_001fe964 lang=c99 rodata=0x002d9424 */
/* Hand-written reconstruction: clone of the source of stage_state_step_001f0560.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag a7, via wip/attempt_001f0560.c) corrected by hand: fields of the stage object are real struct members (the raw offset form makes MWCC share &state in a1/v1), `iVar2 > 5` for the count test (slti at), the busy test written busy != 0 -> false else bit test, and the post-case `cur` reload folded into the compare; exact match verified; names and types await human review. */
typedef struct Stage {
  char pad00[0x44];
  int init;
  char pad48[0x510 - 0x48];
  unsigned int unit;
  char pad514[0x58c - 0x514];
  unsigned char busy;
  char pad58d[0x1084 - 0x58d];
  int state;
  int count;
  char pad108c[0x10a0 - 0x108c];
  int target;
  int cur;
} Stage;

extern char *DAT_003751d4;
extern char DAT_003eaad8;
extern void FUN_001bfc3c(void *, int);
extern void FUN_001eec00();
extern void FUN_001f0b34();
extern void FUN_001f0be4();
extern void FUN_001f3cdc();
extern void FUN_001f3d7c();
extern void FUN_001f3fb8();
extern void FUN_001f4008();
extern void FUN_001f4054();
extern void FUN_00201c30();
extern void FUN_00202cdc();
extern void FUN_00202d34();
extern void FUN_002031c4();

void FUN_001fe964(Stage *self)
{
  unsigned char ready;
  int next;
  int prev;

  if (self->init == 0) {
    self->state = 0;
    self->count = 0;
    FUN_00202cdc(self, 0);
    FUN_001eec00(self->unit, 2);
    FUN_001f0be4(self->unit);
    FUN_001f3cdc(self->unit);
    FUN_001f3fb8(self->unit);
  }
  switch (self->state) {
  case 0:
    next = self->count + 1;
    self->count = next;
    if (10 < next) {
      self->state = 1;
      self->count = 0;
      FUN_001f3d7c(self->unit, self->state);
      FUN_001f4054(self->unit, self->state);
      FUN_001bfc3c(&DAT_003eaad8, 8);
    }
    break;
  case 1:
  case 2:
  case 3:
  case 4:
    next = self->count + 1;
    self->count = next;
    if (next > 5) {
      if (self->state != 4) {
        FUN_001bfc3c(&DAT_003eaad8, 8);
      }
      next = self->state;
      self->state = next + 1;
      self->count = 0;
      FUN_001f3d7c(self->unit, self->state);
      FUN_001f4054(self->unit, self->state);
    } else {
      if (self->busy != 0) {
        ready = 0;
      } else {
        ready = ((int)*(char *)(DAT_003751d4 + 0x8b) << 0x1c) >> 0x1e == 0;
      }
      if (ready) {
        FUN_001bfc3c(&DAT_003eaad8, 3);
        self->cur = self->target;
        self->state = 6;
        self->count = 0;
        FUN_001f4008(self->unit);
        FUN_001f3d7c(self->unit, self->state);
        FUN_001f0b34(self->unit);
      }
    }
    break;
  case 5:
    prev = self->cur;
    next = prev + 0x14;
    self->cur = next;
    if (next >= self->target) {
      self->cur = self->target;
      self->state = 6;
      self->count = 0;
      FUN_001f0b34(self->unit);
    } else {
      if (self->busy != 0) {
        ready = 0;
      } else {
        ready = ((int)*(char *)(DAT_003751d4 + 0x8b) << 0x1c) >> 0x1e == 0;
      }
      if (ready) {
        FUN_001bfc3c(&DAT_003eaad8, 3);
        self->cur = self->target;
        self->state = 6;
        self->count = 0;
        FUN_001f0b34(self->unit);
        FUN_001f4008(self->unit);
      }
    }
    if (prev != self->cur) {
      FUN_001bfc3c(&DAT_003eaad8, 0x1a);
    }
    FUN_001f3d7c(self->unit, self->state);
    break;
  case 6:
    if (self->busy != 0) {
      ready = 0;
    } else {
      ready = ((int)*(char *)(DAT_003751d4 + 0x8b) << 0x1c) >> 0x1e == 0;
    }
    if (ready) {
      FUN_001bfc3c(&DAT_003eaad8, 3);
      FUN_00201c30(self);
    }
  }
  FUN_002031c4(self);
  FUN_00202d34(self);
}
