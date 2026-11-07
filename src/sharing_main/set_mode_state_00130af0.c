/* HSTMATCH module=hstp_trans_main.prx start=0x00130af0 end=0x00130e80 symbol=FUN_00130af0 lang=c99 */
/* Hand-written reconstruction: clone of the source of set_mode_state_001258bc.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode corrected by hand: signed u8 tables and mode bounds were matched; shared table pointers and branch layout were reconstructed; readability pass; exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/actor_state.h"

/* State ids from STATE_GROUP_FIRST up to (not including) STATE_GROUP_END start the actor with its unk90 timing. */
#define STATE_GROUP_FIRST 0x80
#define STATE_GROUP_END 0xc0

extern int DAT_00381220; /* lower limit for the timing value */
extern StateRow DAT_003e48c4[]; /* state table */
/* The same table seen field by field through the pre-scaled row offset (typed row indexing changes the float
   register order): row.unk1c, row.unk46, row.unk4e, row.unk50. */
extern u8 DAT_003e48e0;
extern u8 DAT_003e490a;
extern char DAT_003e4912;
extern char DAT_003e4914;
extern u32 DAT_003e95c4; /* host object */
extern void FUN_0012fdcc(ActorState *self,int slot,StateRow *row);
extern int FUN_001304d0();
extern void FUN_00130e80(float param_1,ActorState *self);
extern void FUN_0013bf24();
extern void FUN_0013c0f4();
extern int FUN_0013d030();
extern u8 * FUN_0013d6dc();

/* Switches the actor to a new state: resets its timers, picks the state row (or a blend between two rows) and starts it. */
void FUN_00130af0(ActorState *self,int new_state)
{
  short link;
  int *timing;
  int iVar3;
  int iVar4;
  int local_4;
  
  if (new_state == self->stateIdx) return;
    self->stateIdx = new_state;
    self->unk5c = 0;
    FUN_00130e80(0.0f,self);
    if ((self->stateIdx >= STATE_GROUP_FIRST) && (self->stateIdx < STATE_GROUP_END)) {
      if (self->unk90 < DAT_00381220) {
        timing = &DAT_00381220;
      }
      else {
        timing = &self->unk90;
      }
      self->unk64 = (float)*timing;
    }
    else {
      self->unk64 = 0;
    }
    if (DAT_003e48c4[self->stateIdx].unk50 == 7) {
      FUN_0013bf24(DAT_003e95c4,1);
    }
    else {
      FUN_0013bf24(DAT_003e95c4,0);
    }
    iVar3 = FUN_0013d030(DAT_003e95c4,(int)DAT_003e48c4[self->stateIdx].unk4f);
    iVar4 = self->stateIdx * 0x58;
    iVar3 = (int)((*(float *)(&DAT_003e48e0 + iVar4) + (float)iVar3) -
                 (float)self->unk90);
    if ((&DAT_003e4914)[iVar4] == 6) {
      self->unk6c = (float)iVar3;
      self->unk70 = (float)iVar3;
    }
    else {
      self->unk6c = -1.0f;
    }
    iVar4 = self->stateIdx * 0x58;
    if ((&DAT_003e4912)[iVar4] == 0) {
      self->unk84 = -1.0f;
      self->unk88 = 0;
      StateRow *row = &DAT_003e48c4[self->stateIdx];
      self->slotA = row;
      FUN_0012fdcc(self,0,row);
    }
    else {
      link = *(short *)(&DAT_003e490a + iVar4);
      FUN_0013d6dc((int)link,&local_4);
      iVar4 = FUN_001304d0(self,local_4);
      if (iVar3 > 0) {
        StateRow *current_row = &DAT_003e48c4[self->stateIdx];
        self->slotA = 0;
        FUN_0012fdcc(self,1,current_row);
        FUN_0012fdcc(self,2,&DAT_003e48c4[iVar4]);
        self->unk84 = 0;
        self->unk88 = (float)iVar3;
        if (DAT_003e48c4[self->stateIdx].unk50 == 7) {
          FUN_0013c0f4(DAT_003e95c4,iVar3);
        }
      
      }
      else {
        if ((self->unk94 == 0) &&
           (((self->stateIdx == 4 || (self->stateIdx == 7)) || (link == 0x3c)))
           ) {
          self->unk84 = -1.0f;
          self->unk88 = 0;
          StateRow *fallback_row = &DAT_003e48c4[self->stateIdx];
          self->slotA = fallback_row;
          FUN_0012fdcc(self,0,fallback_row);
        }
        else {
          FUN_00130af0(self,iVar4);
        }
      
      }
    }
}
