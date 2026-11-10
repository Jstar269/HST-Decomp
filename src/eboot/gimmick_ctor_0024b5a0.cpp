/* HSTMATCH module=EBOOT.BIN start=0x0024b5a0 end=0x0024b8e8 symbol=__ct__10C_0024b5a0Fi lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The model list is an STL-style vector (data/size/cap) whose grow helper takes an empty tag struct by value. */

struct Vec4 {
    float x, y, z, w;
};

struct Model {
    char *name;
};

struct Rig {
    char pad[8];
    void *skel;
};

struct B_00056b98 {
    B_00056b98();
    virtual void v0();
    virtual void SetModel(Model *m);
    char pad4[0x88];
    Rig *rig;
    char pad90[0x1b0 - 0x90];
};

struct B_0001f06c {
    B_0001f06c();
    virtual void v0();
    char pad4[0x38];
};

struct ModelVec {
    Model **data;
    unsigned size;
    unsigned cap;
    Model *&operator[](int i) { return data[i]; }
};

struct C_0024b5a0 : B_0001f06c {
    int f3c;
    int f40;
    char pad44[0x50 - 0x44];
    B_00056b98 body;
    char pad200[4];
    Model *model;
    int f208;
    int kind;
    Vec4 pos;
    char pad220[0x230 - 0x220];
    Vec4 home;
    int f240;
    int f244;
    char pad248;
    unsigned char f249;
    char pad24a[0x254 - 0x24a];
    unsigned char f254;
    char pad255[3];
    int eff1;
    int eff2;
    int eff3;
    int f264;
    unsigned char f268;
    char pad269[3];
    int f26c;
    int f270;
    C_0024b5a0(int kind);
    virtual void v1();
};

void *operator new(unsigned int size);

extern int DAT_002fbcc0[];
extern char *DAT_002fb6e4[];
extern char *DAT_002fb880[];
extern char *DAT_002fbc24[];
extern char DAT_002fbcf4[];
extern char DAT_002fbcfc[];
extern char DAT_002fbd04[];
extern char DAT_002fbd0c[];
extern char DAT_002fbd14[];
extern ModelVec DAT_0034a768;
extern int DAT_0034a774;

struct False {};

extern "C" {
int FUN_00014934(const char *a, const char *b);
Model *FUN_000535d8(void *mem);
void FUN_000537bc(Model *m, char *name, char *snd, int a, int b, int c, int d);
void FUN_00146698(ModelVec *v, Model *const *x, False f);
void FUN_000573a0(B_00056b98 *b, int a, char *name, int c, int d);
int FUN_0002fe80(void *skel, char *name);
}

static inline int Find(B_00056b98 *b, char *name)
{
    return b->rig->skel != 0 ? FUN_0002fe80(b->rig->skel, name) : -1;
}

static inline void Push(ModelVec *v, Model *const &x)
{
    if (v->size < v->cap) {
        v->size++;
        (*v)[v->size - 1] = x;
    } else
        FUN_00146698(v, &x, False());
}

C_0024b5a0::C_0024b5a0(int k) : f3c(0)
{
    model = 0;
    kind = 0;
    f249 = 1;
    f254 = 0;
    f264 = -1;
    f268 = 0;
    f40 = DAT_002fbcc0[k];
    for (unsigned i = 0; i < DAT_0034a768.size; i++) {
        if (FUN_00014934(DAT_002fb6e4[k], DAT_0034a768[i]->name) == 0) {
            model = DAT_0034a768[i];
            break;
        }
    }
    if (model == 0) {
        Model *m = (Model *)operator new(48);
        if (m != 0)
            m = FUN_000535d8(m);
        model = m;
        FUN_000537bc(m, DAT_002fb6e4[k], DAT_002fb880[k], 0, 0, 0, 0);
        Push(&DAT_0034a768, model);
    }
    f208 = -1;
    body.SetModel(model);
    f240 = 0;
    kind = k;
    FUN_000573a0(&body, 0, DAT_002fbc24[k], 0, 0);
    home = pos;
    f244 = 0;
    DAT_0034a774++;
    if (kind == 1) {
        eff1 = Find(&body, DAT_002fbcf4);
        eff2 = Find(&body, DAT_002fbcfc);
        eff3 = Find(&body, DAT_002fbd04);
    } else if (kind == 4 || kind == 2) {
        eff1 = Find(&body, DAT_002fbcf4);
    }
    if (kind == 5) {
        f26c = Find(&body, DAT_002fbd0c);
        f270 = Find(&body, DAT_002fbd14);
    }
}
