/* HSTMATCH module=hstp_trans_main.prx start=0x001a3e54 end=0x001a3f24 symbol=FUN_001a3e54 lang=c99 */
/* Hand-written reconstruction: clone of the source of table_select_00197504.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/text_draw.h"

#define PALETTE_SIZE 13

extern float DAT_002d2de0[][4];
extern float DAT_002d2de4[][4];
extern float DAT_002d2de8[][4];
extern float DAT_002d2eb0[][4];
extern float DAT_002d2eb4[][4];
extern float DAT_002d2eb8[][4];
extern void FUN_000430e8(void *, TextColor *);

/* Pick the text colour from the palette row for `which` (alternate palette when alt is set) and scale the base colour by it. */
void FUN_001a3e54(TextDraw *self, unsigned int which)
{
    int idx = which % PALETTE_SIZE;
    float c;
    float b;
    float a;
    float d;

    if (self->alt != 0) {
        a = DAT_002d2de0[idx][0];
        b = DAT_002d2de4[idx][0];
        c = DAT_002d2de8[idx][0];
    } else {
        a = DAT_002d2eb0[idx][0];
        b = DAT_002d2eb4[idx][0];
        c = DAT_002d2eb8[idx][0];
    }
    d = self->col.a;
    self->col.r = a;
    self->col.g = b;
    self->col.b = c;
    self->col.a = d;
    self->tint.r = a * self->base.r;
    self->tint.g = b * self->base.g;
    self->tint.b = c * self->base.b;
    self->tint.a = d * self->base.a;
    FUN_000430e8(self->font, &self->tint);
}
