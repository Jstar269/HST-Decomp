/* HSTMATCH module=hstp_trans_main.prx start=0x00263310 end=0x002633a8 symbol=FUN_00263310 lang=c++ */
/* Hand-written reconstruction: clone of the source of FUN_00252fbc.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/disp_ctx.h"

struct Inner { char pad[0x318]; unsigned char f318; };
extern "C" DispCtx *DAT_003f3ac4;
extern "C" char DAT_003f39a8[];
extern "C" void FUN_00244258(void *, int);
extern "C" void FUN_0024476c(void *);
extern "C" void FUN_00262c00(void *);

/* Reacts to the display controller's current state (3: one of two messages by a flag of the owned object; 5: a different call), then refreshes it. */
extern "C" void FUN_00263310(void)
{
    DispCtx *c = DAT_003f3ac4;
    if (c != 0) {
        if (c->state == 3) {
            bool t = !((Inner *)c->obj)->f318;
            if (t)
                FUN_00244258(DAT_003f39a8, 0x2d);
            else
                FUN_00244258(DAT_003f39a8, 0x2c);
        } else if (c->state == 5) {
            FUN_0024476c(DAT_003f39a8);
        }
        FUN_00262c00(DAT_003f3ac4);
    }
}
