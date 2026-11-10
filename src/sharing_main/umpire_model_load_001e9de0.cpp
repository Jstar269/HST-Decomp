/* HSTMATCH module=hstp_trans_main.prx start=0x001e9de0 end=0x001eaf5c symbol=FUN_001e9de0 lang=c++ */
/* Clone of src/eboot/umpire_model_load_001dbc40.cpp (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; the eight model-type branches were generated from the call list (try/lane-work/gen1dbc40.py), strings named through DAT_ address externs; exact match verified; names and types await human review. */
struct Skeleton;
struct ModelData { char pad[8]; Skeleton *skel; };
struct Model {
    virtual ~Model();
    virtual void v1();
    virtual int Load(const char *path, const char *dir, int a, int b, int c, int d);
    char pad4[0x8c - 4];
    ModelData *data;
};
struct Umpire {
    char pad0[0x3c];
    Model *model;
    char pad40[4];
    int neck;
    int head;
    char pad4c[4];
    char f50[4];
};
extern "C" {
extern char DAT_00392c84[];
extern char DAT_00392cac[];
extern char DAT_00392ccc[];
extern char DAT_00392cfc[];
extern char DAT_00392d28[];
extern char DAT_00392d54[];
extern char DAT_00392d80[];
extern char DAT_00392dac[];
extern char DAT_00392dd8[];
extern char DAT_00392e04[];
extern char DAT_00392e30[];
extern char DAT_00392e5c[];
extern char DAT_00392e88[];
extern char DAT_00392eb4[];
extern char DAT_00392ec0[];
extern char DAT_00392ecc[];
extern char DAT_00392ef4[];
extern char DAT_00392f14[];
extern char DAT_00392f44[];
extern char DAT_00392f70[];
extern char DAT_00392f9c[];
extern char DAT_00392fc8[];
extern char DAT_00392ff4[];
extern char DAT_0039301c[];
extern char DAT_0039303c[];
extern char DAT_0039306c[];
extern char DAT_00393098[];
extern char DAT_003930c4[];
extern char DAT_003930f0[];
extern char DAT_0039311c[];
extern char DAT_00393144[];
extern char DAT_00393164[];
extern char DAT_00393194[];
extern char DAT_003931c0[];
extern char DAT_003931ec[];
extern char DAT_00393218[];
extern char DAT_00393244[];
extern char DAT_0039326c[];
extern char DAT_0039328c[];
extern char DAT_003932bc[];
extern char DAT_003932e8[];
extern char DAT_00393314[];
extern char DAT_00393340[];
extern char DAT_0039336c[];
extern char DAT_00393398[];
extern char DAT_003933c4[];
extern char DAT_003933f0[];
extern char DAT_0039341c[];
extern char DAT_00393448[];
extern char DAT_00393474[];
extern char DAT_0039349c[];
extern char DAT_003934bc[];
extern char DAT_003934e8[];
extern char DAT_00393514[];
extern char DAT_00393540[];
extern char DAT_0039356c[];
extern char DAT_00393594[];
extern char DAT_003935b4[];
extern char DAT_003935e4[];
extern char DAT_00393610[];
extern char DAT_0039363c[];
extern char DAT_00393668[];
extern char DAT_00393694[];
extern char DAT_003936bc[];
extern char DAT_003936dc[];
extern char DAT_0039370c[];
extern char DAT_00393738[];
extern char DAT_00393764[];
extern char DAT_00393790[];
void *FUN_00000bcc(unsigned);
Model *FUN_000577ec(void *);
void FUN_00057c2c(Model *, const char *);
void FUN_00057ff4(Model *, int, const char *, int, int);
void FUN_0005fa04(Model *, int, const char *, const char *, int, int, int, int);
void FUN_00055b64(Model *, void *);
int FUN_0002fea8(Skeleton *, const char *);
}
static inline int Bone(Model *m, const char *name)
{
    Skeleton *s = m->data->skel;
    return s ? FUN_0002fea8(s, name) : -1;
}
extern "C" void FUN_001e9de0(Umpire *self, int type)
{
    if (self->model) {
        delete self->model;
        self->model = 0;
    }
    void *p = FUN_00000bcc(0x1b0);
    if (p) p = FUN_000577ec(p);
    self->model = (Model *)p;
    if (type == 0) {
        if (self->model->Load(DAT_00392c84, DAT_00392cac, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_00392ccc);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_00392d80, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_00392e30, DAT_00392cac, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_00392e5c, DAT_00392cac, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00392e88, DAT_00392cac, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 1) {
        if (self->model->Load(DAT_00392ecc, DAT_00392ef4, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_00392f14);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_00392f44, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_00392f70, DAT_00392ef4, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_00392f9c, DAT_00392ef4, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00392fc8, DAT_00392ef4, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 2) {
        if (self->model->Load(DAT_00392ff4, DAT_0039301c, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_0039303c);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_0039306c, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_00393098, DAT_0039301c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_003930c4, DAT_0039301c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_003930f0, DAT_0039301c, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 3) {
        if (self->model->Load(DAT_0039311c, DAT_00393144, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_00393164);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_00393194, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_003931c0, DAT_00393144, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_003931ec, DAT_00393144, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00393218, DAT_00393144, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 4) {
        if (self->model->Load(DAT_00393244, DAT_0039326c, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_0039328c);
            FUN_00057ff4(self->model, 0, DAT_003932bc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_003932e8, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00393314, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_00393340, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_0039336c, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00393398, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_003933c4, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_003933f0, DAT_0039326c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_0039341c, DAT_0039326c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00393448, DAT_0039326c, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 5) {
        if (self->model->Load(DAT_00393474, DAT_0039349c, 0, 0, 0, 0)) {
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_003934bc, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_003934e8, DAT_0039349c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_00393514, DAT_0039349c, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00393540, DAT_0039349c, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 6) {
        if (self->model->Load(DAT_0039356c, DAT_00393594, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_003935b4);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_003935e4, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_00393610, DAT_00393594, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_0039363c, DAT_00393594, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00393668, DAT_00393594, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 7) {
        if (self->model->Load(DAT_00393694, DAT_003936bc, 0, 0, 0, 0)) {
            FUN_00057c2c(self->model, DAT_003936dc);
            FUN_00057ff4(self->model, 0, DAT_00392cfc, 0, 0);
            FUN_00057ff4(self->model, 1, DAT_00392d28, 0, 0);
            FUN_00057ff4(self->model, 2, DAT_00392d54, 0, 0);
            FUN_00057ff4(self->model, 3, DAT_0039370c, 0, 0);
            FUN_00057ff4(self->model, 4, DAT_00392dac, 0, 0);
            FUN_00057ff4(self->model, 5, DAT_00392dd8, 0, 0);
            FUN_00057ff4(self->model, 6, DAT_00392e04, 0, 0);
            FUN_0005fa04(self->model, 0, DAT_00393738, DAT_003936bc, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 1, DAT_00393764, DAT_003936bc, 0, 0, 0, 0);
            FUN_0005fa04(self->model, 2, DAT_00393790, DAT_003936bc, 0, 0, 0, 0);
            FUN_00055b64(self->model, self->f50);
            self->neck = Bone(self->model, DAT_00392eb4);
            self->head = Bone(self->model, DAT_00392ec0);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    }
}
