/* HSTMATCH module=hstp_trans_main.prx start=0x00093d10 end=0x00093ef0 symbol=FUN_00093d10 lang=c99 */
/* Hand-written reconstruction: clone of the source of char_state_apply_00092078.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag v0) corrected by hand: restored the float types, split the table index into two statements and reordered the float/int call parameters; readability pass; exact match verified; names and types await human review. */
#include "hst/char_model.h"

extern float DAT_00378b1c; /* lower bound of the blend value for item 2 */
extern float DAT_00378b20; /* lower bound of the blend value for item 0 */
extern void FUN_00059c1c(ModelItem *, float, int);
extern void FUN_0005fe20(float, ModelItem *, int);
extern void FUN_0005ff00(ModelItem *, float);
extern void FUN_00095674(float, CharModel *, int, int);
extern void FUN_0009569c(CharModel *, int, int);
extern void FUN_00095b04(CharModel *, int);
extern void FUN_00095b2c(CharModel *, int);

/* Applies state to one channel of a character: switches item 2's channel when its state differs
   (state < 0 clears it), mirrors the state onto item 0 for channel 0, and when blend and state are
   non-negative updates the items with the blend value. */
void FUN_00093d10(float blend_in, CharModel *self, int state, int flag, int channel)
{
    float blend;
    float blend_copy;
    float *clamped;
    int cur_state;
    int changed;
    ModelItem *item;

    blend = blend_in;
    changed = 0;
    if (self->item[2] != 0) {
        cur_state = self->item[2]->chan[channel].state;
    } else
        cur_state = -1;
    if (state != cur_state) {
        changed = 1;
        FUN_0009569c(self, flag, channel);
        if (DAT_00378b1c < blend)
            clamped = &blend;
        else
            clamped = &DAT_00378b1c;
        FUN_00095674(*clamped, self, channel, state);
        if (state >= 0)
            FUN_00095b04(self, channel);
        else
            FUN_00095b2c(self, channel);
    }
    if (((channel == 0) && (item = self->item[0], item != 0)) &&
        (state != item->unka0) && (item != 0)) {
        if (DAT_00378b20 < blend)
            clamped = &blend;
        else
            clamped = &DAT_00378b20;
        FUN_0005fe20(*clamped, item, state);
        self->item[0]->unkc0 = (char)flag;
        self->item[0]->unkd0 = self->item[0]->unkc4;
    }
    blend_copy = blend;
    if ((blend >= 0.0f) && (state >= 0)) {
        item = self->item[2];
        if ((item != 0) && (!changed)) {
            FUN_00059c1c(item, blend_copy, channel);
            FUN_0005ff00(item, blend_copy);
        }
        if ((channel == 0) && (self->item[0] != 0))
            FUN_0005ff00(self->item[0], blend);
    }
}
