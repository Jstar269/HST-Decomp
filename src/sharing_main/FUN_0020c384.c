/* HSTMATCH module=hstp_trans_main.prx start=0x0020c384 end=0x0020c520 symbol=FUN_0020c384 lang=c99 */
/* Hand-written reconstruction: clone of the source of FUN_001fdbb4.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

/* Two-bit state of four pad buttons packed in one byte. */
typedef struct KeyBits {
    signed char k0 : 2;
    signed char k2 : 2;
    signed char k4 : 2;
    signed char k6 : 2;
} KeyBits;

/* Pad state block: only the packed button bytes this function reads. */
typedef struct PadState {
    u8 pad[0x89];
    KeyBits f89;
    u8 gap8a;
    KeyBits f8b;
    KeyBits f8c;
} PadState;

extern PadState *DAT_003751d4;  /* current pad state */

/* Regulation screen object (sub-objects at 0xc8 and 0x1250 are opaque here). */
typedef struct RegulationScreen {
    char pad0[0x40];
    int mode;             /* 0x40 */
    int state;            /* 0x44 */
    char pad48[8];
    int flag50;           /* 0x50 */
    int flag54;           /* 0x54 */
    char pad58[0x6c];     /* 0x58..0xc3 */
    unsigned char flagc4; /* 0xc4 */
    char padc5[3];        /* 0xc5..0xc7 */
    char subc8[0x1188];   /* 0xc8..0x124f */
    char rot[0xf4];       /* 0x1250..0x1343 */
    float angle;          /* 0x1344 */
} RegulationScreen;

extern int DAT_003eaad8;  /* first argument of FUN_001bfc3c */
extern int FUN_001bfc3c();
extern void FUN_0020f5c0();
extern void FUN_0020fca8();
extern void FUN_00210984(void *p, float delta);

/* Per-frame input handling for mode 2: pad buttons switch state, toggle flags or rotate the model. */
void FUN_0020c384(RegulationScreen *self)
{
    float radians;

    switch (self->mode) {
    case 2:
        if (DAT_003751d4->f8b.k4 == 0) {
            FUN_001bfc3c((int *)&DAT_003eaad8, 4);
            self->state = 6;
        } else if (DAT_003751d4->f8b.k2 == 0) {
            if (self->flag50 == 0) {
                FUN_001bfc3c((int *)&DAT_003eaad8, 4);
                self->state = 6;
            } else {
                FUN_001bfc3c((int *)&DAT_003eaad8, 3);
                self->state = 7;
            }
        } else if (DAT_003751d4->f8b.k0 == 0) {
            if (self->flagc4 != 0) {
                self->flag54 ^= 1;
                FUN_001bfc3c((int *)&DAT_003eaad8, 6);
                FUN_0020fca8(self->subc8);
            }
        } else if (DAT_003751d4->f89.k6 == 0 || DAT_003751d4->f89.k2 == 0) {
            FUN_001bfc3c((int *)&DAT_003eaad8, 1);
            self->flag50 ^= 1;
            FUN_0020f5c0(self->subc8);
        }
        radians = self->angle * 0.017453292f;
        if (DAT_003751d4->f8c.k6 == 1)
            FUN_00210984(self->rot, radians);
        else if (DAT_003751d4->f8c.k2 == 1)
            FUN_00210984(self->rot, -radians);
    }
}
