/* HSTMATCH module=EBOOT.BIN start=0x000fb590 end=0x000fb5f4 symbol=FUN_000fb590 lang=c++ */
typedef struct S { char pad[0x54]; unsigned char flag; char pad2[0x94 - 0x55]; float f; } S;
typedef struct O { char pad[0x68]; S *s; } O;
typedef struct Self { char pad[0x3c]; O *o; } Self;

extern "C" unsigned char FUN_000fb590(Self *self)
{
    unsigned char r;
    O *o = self->o;
    S *s;
    if (o != 0 && (s = o->s) != 0)
        r = s->flag != 0 || (s->f <= 0.0f) != 0;
    else
        r = 1;
    return r;
}
// NEAR MISS (lane w3n1, 2026-11-10): 25/25 words, 15 differ (was 17 in teach_state_pred_000fb590.c). The `if(o&&(s=o->s)) r=..; else r=1;
// return r;` shape with a uchar r reproduces retail's tail exactly (andi join, `b EPI`, the unreachable `li v0,1`).
// Left: retail evaluates the float operand into a separate register preset early (`li a0,1` in the lw delay, then
// `bc1tl; move v0,a0 / move a0,zero; move v0,a0`) with no sltu, and the second null test branches straight to EPI
// with `li v0,1` in its slot; ours puts `li v0,1/move v0,zero` straight into v0 and then `andi;sltu` for the `!=0`.
// Without `!= 0` on the float operand the length drops to 22-23 words. Ruled out: bool flag field, `|`, ternaries
// (`flag?1:f<=0`, `?1:0`), inline Le0 helpers returning int/bool/uchar (with and without a local), z computed first,
// split bool d/z locals, C (lang=c99) forms, early-return and nested-if shapes, inline IsOver member.
// near miss from lane w3n1 (unreviewed)
