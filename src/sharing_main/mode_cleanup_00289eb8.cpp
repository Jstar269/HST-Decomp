/* HSTMATCH module=hstp_trans_main.prx start=0x00289eb8 end=0x0028a0a4 symbol=FUN_00289eb8 lang=c++ */
/* Hand-written reconstruction: clone of the source of mode_cleanup_00278bd4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   matching idiom: the "source = self->source" copy is written in both arms of the mode==3 test (it must stay a shared
   local), the count guard is spelled "<= 3" (a "< 4" test reuses the load register and loses retail's slti $at),
   the skipped-store path uses goto tail, and the mode chain is a descending ==-chain, not a switch. */
#include "hst/shop_scene.h"

extern int *DAT_003e1a08;

extern "C" int FUN_0008f170(int *);
extern "C" void *FUN_0008f108(int *, int);
extern "C" int FUN_00048e38(int);
extern "C" void FUN_0005ffd8(void *, void *);

/* Layout widget, seen through its virtual calls (slot 3 draws it). */
struct ShopWidget {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
};

/* Draws the shop scene: the layout widgets, then (depending on the state and the item counts) the model, then the front widget. */
extern "C" void FUN_00289eb8(ShopScene *self)
{
  ShopWidget *p;
  int mode;
  void *model;
  ShopSource *source;
  int r;

  if (self->state == 6) {
    self->widget_100->v3();
    self->widget_a0c->v3();
    return;
  }
  self->widget_a3c->v3();
  ((ShopWidget *)self->pane)->v3();
  p = (ShopWidget *)FUN_0008f170(DAT_003e1a08);
  p->v3();
  mode = self->state;
  if (mode == 4 || mode == 2 || mode == 1 || mode == 3 || mode == 7)
    model = self->unkf0;
  else
    goto tail;
  if (model != 0) {
    if (mode == 4 && self->unk94 == 0)
      return;
    if (mode == 2 && self->unkac == 0)
      return;
    if (mode == 1 && self->unkb8 == 0)
      return;
    if (mode == 3) {
      if (self->unkc4 == 0)
        return;
      source = self->source;
    } else {
      source = self->source;
    }
    if (source->count <= 3)
      return;
    if (self->unk8c5 == 0) {
      self->widget_fc->v3();
      return;
    }
    r = FUN_00048e38(0);
    if (r != 0) {
      FUN_0005ffd8(self->unkf0, &self->camera);
      if (self->unkf8 != 0)
        FUN_0005ffd8(self->model, &self->camera);
    } else {
      FUN_0005ffd8(FUN_0008f108(DAT_003e1a08, 0), &self->camera);
    }
  }
tail:
  self->widget_fc->v3();
}
