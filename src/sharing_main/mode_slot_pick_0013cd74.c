/* HSTMATCH module=hstp_trans_main.prx start=0x0013cd74 end=0x0013ce94 symbol=FUN_0013cd74 lang=c99 */
/* Hand-written reconstruction: clone of the source of mode_slot_pick_001317bc.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Self { char pad[0x128]; int unk128; int unk12c; int unk130; };
struct Cfg  { char pad[0x50]; int unk50; };

extern float DAT_003823c8;
extern float DAT_003823cc;
extern float DAT_003823d0;
extern float DAT_003823d4;
extern u32 *DAT_003e19f0;
extern struct Cfg *DAT_003e1e84;

/* Returns the entry of the global table at 0x70 indexed by the field chosen by mode (5 to 9), or 0. */
u32 FUN_0013cd74(struct Self *self,int mode)
{
  float tbl[4];
  u32 result;
  int off;

  result = 0;
  tbl[0] = DAT_003823c8;
  tbl[1] = DAT_003823cc;
  tbl[2] = DAT_003823d0;
  tbl[3] = DAT_003823d4;
  switch (mode) {
  case 5:
    off = self->unk128 * 4;
    result = *(u32 *)(off + (int)DAT_003e19f0 + 0x70);
    break;
  case 6:
    off = self->unk12c * 4;
    result = *(u32 *)(off + (int)DAT_003e19f0 + 0x70);
    break;
  case 7:
    off = DAT_003e1e84->unk50 * 4;
    result = *(u32 *)(off + (int)DAT_003e19f0 + 0x70);
    break;
  case 8:
    off = (int)tbl[DAT_003e1e84->unk50] * 4;
    result = *(u32 *)(off + (int)DAT_003e19f0 + 0x70);
    break;
  case 9:
    off = self->unk130 * 4;
    result = *(u32 *)(off + (int)DAT_003e19f0 + 0x70);
    break;
  }
  return result;
}
