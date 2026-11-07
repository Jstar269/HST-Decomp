/* HSTMATCH module=hstp_trans_main.prx start=0x000f5e24 end=0x000f6000 symbol=FUN_000f5e24 lang=c99 */
/* Hand-written reconstruction: clone of the source of render_params_setup_000f075c.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/ingame_panes.h"

extern PaneItemDef DAT_0037e1d0[];
extern int *DAT_003e1e84;

extern int FUN_000e23f4(int *, int);
extern int FUN_000e2424(int *, int);
extern int FUN_000e2454(int *, int);
extern int FUN_000e2484(int *, int);
extern void FUN_00105470(PaneQuad *, int);

/* Set up both slots of the ace pane: colour, shown number and vertical position of each item pair. */
void FUN_000f5e24(AcePane *pane)
{
  int i;
  AcePane *slot; /* the pane viewed from slot i on: item[0]/item[2] are slot i's pair */
  PaneItemDef *def;
  PaneQuad *p;
  int base;
  short top;
  short old;
  short d;

  slot = pane;
  i = 0;
  def = DAT_0037e1d0;
  for (; i < 2; i++) {
    FUN_00105470(slot->item[0], 0);
    slot->item[0]->color = 0x80808080;
    base = def->value;
    if (FUN_000e23f4(DAT_003e1e84, i) != 0) {
      p = slot->item[0];
      p->value = base;
      p->base = base;
    } else if (FUN_000e2424(DAT_003e1e84, i) != 0) {
      p = slot->item[0];
      p->value = base + 1;
      p->base = base + 1;
    } else if (FUN_000e2454(DAT_003e1e84, i) != 0) {
      p = slot->item[0];
      p->value = base + 2;
      p->base = base + 2;
    } else {
      FUN_00105470(slot->item[0], 1);
      slot->item[0]->color = 0x808080;
    }
    FUN_00105470(slot->item[2], 0);
    slot->item[2]->color = 0x80808080;
    if (FUN_000e2484(DAT_003e1e84, i) != 0) {
      p = slot->item[2];
      p->value = base + 3;
      p->base = base + 3;
      if ((slot->item[0]->flags & 3) != 0) {
        PaneQuad *q = slot->item[2];
        top = (short)(int)slot->item_y[0];
        old = q->y0;
        q->y0 = top;
        d = top - old;
        q->y1 = q->y1 + d;
      } else {
        PaneQuad *q = slot->item[2];
        top = (short)(int)slot->item_y[2];
        old = q->y0;
        q->y0 = top;
        d = top - old;
        q->y1 = q->y1 + d;
      }
    } else {
      FUN_00105470(slot->item[2], 1);
      slot->item[2]->color = 0x808080;
    }
    slot = (AcePane *)((char *)slot + 4);
    def++;
  }
}
