/* HSTMATCH module=hstp_trans_main.prx start=0x002009a8 end=0x00200d38 symbol=FUN_002009a8 lang=c++ */
/* Hand-written reconstruction: clone of the source of scene_intro_update_001f25a4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (generated candidate used only for the pseudocode shape); exact match verified; names and types await human review. */
struct Bits {
    signed char lo : 2;
    signed char mode : 2;
    signed char hi : 4;
};

struct Config {
    char pad[0x8b];
    Bits bits;
};

struct Panel {
    char pad[0x98];
    unsigned char shown;
};

struct Scene {
    char pad0[0x3c];
    char fade[0x44 - 0x3c];
    int started;
    char pad1[0x510 - 0x48];
    void *view;
    char pad2[0x560 - 0x514];
    void *menu;
    char pad3[0x584 - 0x564];
    Panel *panel;
    char pad4[0x58c - 0x588];
    unsigned char busy;
    char pad5[0x5a4 - 0x58d];
    unsigned char skip;
    char pad6[0x81a4 - 0x5a5];
    int state;
    int count;
    char pad7;
    unsigned char ready;
    char pad8;
    unsigned char quick;
    unsigned char alt;
};

extern "C" {
extern char DAT_003eaad8[];
extern Config *DAT_003751d4;
extern void *DAT_003daec4;
void FUN_001f5f40(void *);
void FUN_001f0be4(void *);
void FUN_001f666c(void *);
void FUN_001bfc3c(void *, int);
void FUN_001f5fb8(void *, int, int);
void FUN_001f671c(void *);
void FUN_001f6710(void *);
void FUN_001c0bb0(void *, int);
void FUN_001f0b34(void *);
void FUN_00203184(Scene *);
void FUN_0020a040(void *, int, int);
void FUN_000649e4(void *, int, float);
void FUN_001ee980(void *);
void FUN_00225de4(void *);
void FUN_0020de38(void);
void FUN_002031c4(Scene *);
void FUN_00202dcc(Scene *);
}

extern "C" void FUN_002009a8(Scene *self)
{
    if (self->started == 0) {
        self->state = 0;
        self->count = 0;
        FUN_001f5f40(self->view);
        FUN_001f0be4(self->view);
        FUN_001f666c(self->view);
        FUN_001bfc3c(DAT_003eaad8, 8);
    }
    switch (self->state) {
    case 0:
        if (++self->count > 30) {
            self->state = 1;
            self->count = 0;
            if (self->quick == 0 && self->ready != 0) {
                FUN_001f5fb8(self->view, 1, 0);
                FUN_001f671c(self->view);
                FUN_001c0bb0(DAT_003eaad8, 12);
            }
        }
        break;
    case 1:
        self->count++;
        if (self->quick != 0 || self->ready == 0 || self->count > 40) {
            if (self->alt != 0) {
                self->state = 2;
                self->count = 0;
                FUN_001f5fb8(self->view, 0, 1);
                FUN_001f6710(self->view);
                FUN_001c0bb0(DAT_003eaad8, 12);
            } else {
                self->state = 3;
                self->count = 0;
                FUN_001f0b34(self->view);
            }
        }
        break;
    case 2:
        if (++self->count > 30) {
            self->state = 3;
            self->count = 0;
            FUN_001f0b34(self->view);
        }
        break;
    case 3: {
        bool go;
        if (self->busy != 0)
            go = false;
        else
            go = DAT_003751d4->bits.mode == 0;
        if (go) {
            FUN_001bfc3c(DAT_003eaad8, 3);
            self->state = 4;
            self->count = 0;
        }
        break;
    }
    case 4:
        if (self->skip != 0) {
            FUN_00203184(self);
            FUN_0020a040(self->fade, 15, 1);
        } else {
            int n = ++self->count;
            if (n == 1)
                FUN_000649e4(DAT_003daec4, 5, 65535.0f);
            if (n == 7) {
                FUN_00203184(self);
                FUN_001ee980(self->view);
                self->panel->shown = 1;
                FUN_0020a040(self->fade, 4, 1);
                FUN_00225de4(self->menu);
                FUN_0020de38();
            }
        }
        break;
    }
    FUN_002031c4(self);
    FUN_00202dcc(self);
}
