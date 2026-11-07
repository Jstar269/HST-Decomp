/* HSTMATCH module=hstp_trans_main.prx start=0x0008bcf4 end=0x0008bdec symbol=FUN_0008bcf4 lang=c99 */
/* Hand-written reconstruction: clone of the source of state_step_0008a634.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/play_flow.h"

extern void FUN_002be7ec(void *ctx);
extern int FUN_00209c14(void);
extern void FUN_001697ec(int value, int mode);
extern void FUN_00244258(int *timer, int frames);
extern int DAT_003f39a8;

/* Two-step handler: step 0 resets and advances; step 1 polls the context and picks the follow-up from the helper's flags. */
void FUN_0008bcf4(PlayFlowObj *self)
{
  switch (self->step) {
  case 0:
    self->unk48 = 0;
    self->step = self->step + 1;
    break;
  case 1:
    FUN_002be7ec(self->ctx);
    if (self->sub->unk515 != 0) {
      if (FUN_00209c14() != 0) {
        if (self->sub->unk5a2 != 0) {
          FUN_001697ec(DAT_003f39a8, 1);
          FUN_00244258(&DAT_003f39a8, 75);
        }
        else {
          FUN_00244258(&DAT_003f39a8, 30);
        }
      }
      else {
        FUN_001697ec(DAT_003f39a8, 0);
        FUN_00244258(&DAT_003f39a8, 75);
      }
    }
    else if (self->sub->unk516 != 0) {
      self->nextState = 10;
    }
    break;
  }
}
