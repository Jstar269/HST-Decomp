/* HSTMATCH module=hstp_trans_main.prx start=0x001f0ed8 end=0x001f0ffc symbol=FUN_001f0ed8 lang=c++ */
/* Hand-written reconstruction: clone of the source of teardown_scene_001e2c84.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane). Notes: a virtual call to retail's slot 0xc
   must be written as the SECOND declared virtual (mwcc puts declared virtual #0 at vtable offset 8
   and #1 at 0xc in this build), and the C++ TU needs every extern inside `extern "C" { }` or hstmatch
   reports MISSING_SYMBOLS with the mangled names. FUN_001f2d64's second argument is a float 0.0f
   (mtc1 zero,$f12), not an int. The score pair repeats init_score_pair_001e29e0 with DAT read first
   in both arms. Exact match verified; names and types await human review. */
#include "hst/result_screen.h"

extern "C" {
extern int DAT_00393850;
extern unsigned char DAT_003eaaf0;
extern void FUN_00105470(void *, int);
extern void FUN_001eec00(ResultScreen *, int);
extern void FUN_001f08e8();
extern void FUN_001f12fc(ResultScreen *, int);
extern void FUN_001f2d64(ResultScreen *, float);
}

/* Object whose virtual call at vtable slot 0xc is the second declared virtual. */
struct UiTarget {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

/* Switch object: w1 is the integer setter. */
struct ResultSwitch {
    virtual void w0();
    virtual void w1(int);
    virtual void w2();
    virtual void w3();
};

extern "C" void FUN_001f0ed8(ResultScreen *self)
{
    int value;
    ScoreBox *box;
    ResultSwitch *sw;

    FUN_001f08e8();
    FUN_001eec00(self, 1);
    ((UiTarget *)self->ui)->v1();
    FUN_00105470(self->ui->unk60, 0);
    if (DAT_003eaaf0 == 1) {
        value = DAT_00393850 + 1;
        box = self->ui->unk60;
        box->unk24 = value;
        box->unk28 = value;
    } else {
        value = DAT_00393850;
        box = self->ui->unk60;
        box->unk24 = value;
        box->unk28 = value;
    }
    sw = self->ui->unk58;
    sw->w1(1);
    sw = self->ui->unk5c;
    sw->w1(0);
    FUN_00105470(self->ui->unk7c, 0);
    FUN_00105470(self->ui->unk84, 0);
    FUN_00105470(self->ui->unk80, 0);
    FUN_00105470(self->ui->unk88, 0);
    FUN_001f12fc(self, 1);
    FUN_001f2d64(self, 0.0f);
}
