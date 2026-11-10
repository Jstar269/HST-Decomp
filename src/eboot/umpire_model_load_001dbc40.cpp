/* HSTMATCH module=EBOOT.BIN start=0x001dbc40 end=0x001dcdbc symbol=FUN_001dbc40 lang=c++ */
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
extern char DAT_002eadd4[];
extern char DAT_002eadfc[];
extern char DAT_002eae1c[];
extern char DAT_002eae4c[];
extern char DAT_002eae78[];
extern char DAT_002eaea4[];
extern char DAT_002eaed0[];
extern char DAT_002eaefc[];
extern char DAT_002eaf28[];
extern char DAT_002eaf54[];
extern char DAT_002eaf80[];
extern char DAT_002eafac[];
extern char DAT_002eafd8[];
extern char DAT_002eb004[];
extern char DAT_002eb010[];
extern char DAT_002eb01c[];
extern char DAT_002eb044[];
extern char DAT_002eb064[];
extern char DAT_002eb094[];
extern char DAT_002eb0c0[];
extern char DAT_002eb0ec[];
extern char DAT_002eb118[];
extern char DAT_002eb144[];
extern char DAT_002eb16c[];
extern char DAT_002eb18c[];
extern char DAT_002eb1bc[];
extern char DAT_002eb1e8[];
extern char DAT_002eb214[];
extern char DAT_002eb240[];
extern char DAT_002eb26c[];
extern char DAT_002eb294[];
extern char DAT_002eb2b4[];
extern char DAT_002eb2e4[];
extern char DAT_002eb310[];
extern char DAT_002eb33c[];
extern char DAT_002eb368[];
extern char DAT_002eb394[];
extern char DAT_002eb3bc[];
extern char DAT_002eb3dc[];
extern char DAT_002eb40c[];
extern char DAT_002eb438[];
extern char DAT_002eb464[];
extern char DAT_002eb490[];
extern char DAT_002eb4bc[];
extern char DAT_002eb4e8[];
extern char DAT_002eb514[];
extern char DAT_002eb540[];
extern char DAT_002eb56c[];
extern char DAT_002eb598[];
extern char DAT_002eb5c4[];
extern char DAT_002eb5ec[];
extern char DAT_002eb60c[];
extern char DAT_002eb638[];
extern char DAT_002eb664[];
extern char DAT_002eb690[];
extern char DAT_002eb6bc[];
extern char DAT_002eb6e4[];
extern char DAT_002eb704[];
extern char DAT_002eb734[];
extern char DAT_002eb760[];
extern char DAT_002eb78c[];
extern char DAT_002eb7b8[];
extern char DAT_002eb7e4[];
extern char DAT_002eb80c[];
extern char DAT_002eb82c[];
extern char DAT_002eb85c[];
extern char DAT_002eb888[];
extern char DAT_002eb8b4[];
extern char DAT_002eb8e0[];
void *FUN_00000bcc(unsigned);
Model *FUN_00056b98(void *);
void FUN_00056fd8(Model *, const char *);
void FUN_000573a0(Model *, int, const char *, int, int);
void FUN_0005ec14(Model *, int, const char *, const char *, int, int, int, int);
void FUN_00055044(Model *, void *);
int FUN_0002fe80(Skeleton *, const char *);
}
static inline int Bone(Model *m, const char *name)
{
    Skeleton *s = m->data->skel;
    return s ? FUN_0002fe80(s, name) : -1;
}
extern "C" void FUN_001dbc40(Umpire *self, int type)
{
    if (self->model) {
        delete self->model;
        self->model = 0;
    }
    void *p = FUN_00000bcc(0x1b0);
    if (p) p = FUN_00056b98(p);
    self->model = (Model *)p;
    if (type == 0) {
        if (self->model->Load(DAT_002eadd4, DAT_002eadfc, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eae1c);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eaed0, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eaf80, DAT_002eadfc, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eafac, DAT_002eadfc, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eafd8, DAT_002eadfc, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 1) {
        if (self->model->Load(DAT_002eb01c, DAT_002eb044, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb064);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb094, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb0c0, DAT_002eb044, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb0ec, DAT_002eb044, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb118, DAT_002eb044, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 2) {
        if (self->model->Load(DAT_002eb144, DAT_002eb16c, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb18c);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb1bc, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb1e8, DAT_002eb16c, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb214, DAT_002eb16c, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb240, DAT_002eb16c, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 3) {
        if (self->model->Load(DAT_002eb26c, DAT_002eb294, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb2b4);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb2e4, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb310, DAT_002eb294, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb33c, DAT_002eb294, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb368, DAT_002eb294, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 4) {
        if (self->model->Load(DAT_002eb394, DAT_002eb3bc, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb3dc);
            FUN_000573a0(self->model, 0, DAT_002eb40c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eb438, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eb464, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb490, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eb4bc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eb4e8, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eb514, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb540, DAT_002eb3bc, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb56c, DAT_002eb3bc, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb598, DAT_002eb3bc, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 5) {
        if (self->model->Load(DAT_002eb5c4, DAT_002eb5ec, 0, 0, 0, 0)) {
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb60c, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb638, DAT_002eb5ec, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb664, DAT_002eb5ec, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb690, DAT_002eb5ec, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 6) {
        if (self->model->Load(DAT_002eb6bc, DAT_002eb6e4, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb704);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb734, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb760, DAT_002eb6e4, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb78c, DAT_002eb6e4, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb7b8, DAT_002eb6e4, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    } else if (type == 7) {
        if (self->model->Load(DAT_002eb7e4, DAT_002eb80c, 0, 0, 0, 0)) {
            FUN_00056fd8(self->model, DAT_002eb82c);
            FUN_000573a0(self->model, 0, DAT_002eae4c, 0, 0);
            FUN_000573a0(self->model, 1, DAT_002eae78, 0, 0);
            FUN_000573a0(self->model, 2, DAT_002eaea4, 0, 0);
            FUN_000573a0(self->model, 3, DAT_002eb85c, 0, 0);
            FUN_000573a0(self->model, 4, DAT_002eaefc, 0, 0);
            FUN_000573a0(self->model, 5, DAT_002eaf28, 0, 0);
            FUN_000573a0(self->model, 6, DAT_002eaf54, 0, 0);
            FUN_0005ec14(self->model, 0, DAT_002eb888, DAT_002eb80c, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 1, DAT_002eb8b4, DAT_002eb80c, 0, 0, 0, 0);
            FUN_0005ec14(self->model, 2, DAT_002eb8e0, DAT_002eb80c, 0, 0, 0, 0);
            FUN_00055044(self->model, self->f50);
            self->neck = Bone(self->model, DAT_002eb004);
            self->head = Bone(self->model, DAT_002eb010);
        } else if (self->model) {
            delete self->model;
            self->model = 0;
        }
    }
}
