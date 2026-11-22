/* HSTMATCH module=EBOOT.BIN start=0x0011b634 end=0x0011b7f0 symbol=FUN_0011b634 lang=c99 */
/* Hand-written reconstruction from the disassembly (entry-advance block shared with the exact FUN_0011b7f0, via hst/tick_obj.h); exact match verified; names and types await human review. */
#include "hst/tick_obj.h"

extern char DAT_0033b488;
extern void *DAT_0033b48c;
extern char DAT_00307fc4;

extern void FUN_0001f06c();
extern void FUN_0001f1fc();
extern void FUN_0001f21c();
extern void FUN_0005a8ec();
extern void FUN_0011cd48();

TickObj *FUN_0011b634(TickObj *self, DequeOwner *owner)
{
  FUN_0001f06c(self);
  *(void **)self = &DAT_00307fc4;
  self->owner = owner;
  FUN_0005a8ec((char *)self + 0x40);
  self->unkc8 = 0;
  FUN_0001f21c(owner, self);
  DAT_0033b488 = 1;
  DAT_0033b48c = self;
  self->countdown = 0;
  TickQueue *queue = (TickQueue *)&owner->deque2f8;
  if (queue->length == 0) {
    FUN_0001f1fc(self, 0);
    DAT_0033b488 = 0;
    }
  else {
    TickEntry *e = (TickEntry *)((char *)queue->blocks[(queue->head + queue->first / TICK_BLOCK_ELEMS) % queue->capacity] + (queue->first % TICK_BLOCK_ELEMS) * 0xc);
    self->cur.kind = e->kind;
    self->cur.unk4 = e->unk4;
    self->cur.value = e->value;
    FUN_0011cd48(queue);
    if (queue->length != 0) {
      TickEntry *e2 = (TickEntry *)((char *)queue->blocks[(queue->head + queue->first / TICK_BLOCK_ELEMS) % queue->capacity] + (queue->first % TICK_BLOCK_ELEMS) * 0xc);
      self->next.kind = e2->kind;
      self->next.unk4 = e2->unk4;
      self->next.value = e2->value;
      FUN_0011cd48(queue);
    }
    else {
      self->next.unk4 = 0;
    }
    self->countdown = TICK_COUNTDOWN_START;
  
  }
  return self;
}
