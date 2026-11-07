/* HSTMATCH module=hstp_trans_main.prx start=0x00100fbc end=0x00101164 symbol=FUN_00100fbc lang=c99 */
/* Hand-written reconstruction: clone of the source of dispatch_game_mode_event_000fb880.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode corrected by hand: symbolized global references and expressed mode dispatch with switches; exact match verified; names and types await human review. */
#include "hst/mode_screen.h"
#include "hst/game_mode_state.h"

extern GameModeState * DAT_003e19f0;
extern u32 DAT_003f3b50;
extern char DAT_0037f014[];
extern u8 DAT_003b7ebc;
extern void FUN_0001c684(void *, int, char *);
extern void FUN_0001c73c(void *);
extern void FUN_000fd0ac();
extern void FUN_000fd524();
extern u32 FUN_000febfc();
extern void FUN_00101230();
extern void FUN_00101334();
extern void FUN_0020aaf4();
extern void FUN_0020ac00();
extern void FUN_002a3198();
extern void FUN_002a32e8();

enum {
  MODE_EVENT_START = 0,
  MODE_EVENT_STOP = 0x12,
  MODE_EVENT_START_ALT = 0x18
};

#define MODE_BLOCKING 9 /* in this mode only the stop and alternate-start events are handled */

/* Run the start/stop side effects around the generic event handler. */
u32 FUN_00100fbc(ModeScreen *self,int event,u32 arg3,u32 arg4)
{
  u32 result;
  
  if (((DAT_003e19f0->mode == MODE_BLOCKING) && (event != MODE_EVENT_STOP)) && (event != MODE_EVENT_START_ALT)) {
    return 0;
  }
  switch (event) {
    case MODE_EVENT_START:
      FUN_0001c684(&DAT_003b7ebc,1,DAT_0037f014);
      FUN_000fd0ac(self->core);
      FUN_00101230(self->slot[1]);
      if (DAT_003f3b50 != 0) FUN_002a3198(DAT_003f3b50,2);
      FUN_0001c73c(&DAT_003b7ebc);
      FUN_0020aaf4(self->slot[0],0);
      break;
    case MODE_EVENT_START_ALT:
      FUN_0001c684(&DAT_003b7ebc,1,DAT_0037f014);
      FUN_000fd0ac(self->core);
      FUN_00101230(self->slot[1]);
      if (DAT_003f3b50 != 0) FUN_002a3198(DAT_003f3b50,2);
      FUN_0001c73c(&DAT_003b7ebc);
      FUN_0020aaf4(self->slot[0],1);
      break;
  }
  result = (u32)self->core;
  result = FUN_000febfc(result,event,arg3,arg4);
  switch (event) {
    case MODE_EVENT_STOP:
      FUN_0020ac00(self->slot[0],1);
      FUN_00101334(self->slot[1]);
      FUN_000fd524(self->core,0);
      if (DAT_003f3b50 != 0) FUN_002a32e8(DAT_003f3b50);
  }
  return result;
}
