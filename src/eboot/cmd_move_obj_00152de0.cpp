/* HSTMATCH module=EBOOT.BIN start=0x00152de0 end=0x00153168 symbol=FUN_00152de0 lang=c++ */
/* Hand-written reconstruction from the disassembly (structs from the far draft of lane ol2; both id lookups written out in the function, the start vector built from the four DAT_002bc440..4c floats); exact match verified; names and types await human review. */
struct V4 {
    float x, y, z, w;
    V4() {}
    V4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
} __attribute__((aligned(16)));

struct Obj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual const V4 *GetPos();
    virtual void SetPos(const V4 *);
};
struct PBase { char pad0[0xe0]; };
struct Player : PBase, Obj {};
struct Mgr { char pad0[0x4c]; Obj **objs; };
struct Ctx { int a; int b; int c; Obj *obj; };
struct Cmd { void *vt; int a; int id; int mode; int arg; int dur; unsigned char wait; };

struct TaskBase {
    void *vt;
    TaskBase();
};

struct Task : TaskBase {
    int id;
    int count;
    int frames;
    Task(int i);
};

extern "C" {
extern unsigned char DAT_003402f0;
extern char DAT_002dc714[];
extern char DAT_002dc734[];
extern char DAT_002dc4c0[];
extern char DAT_003083cc[];
extern char DAT_00308af4[];
extern float DAT_002bc440, DAT_002bc444, DAT_002bc448, DAT_002bc44c;
extern Mgr *DAT_0034a114;
extern Player *DAT_0034a118;
void FUN_0014d044(Ctx *);
void FUN_0014fd94(char *, int, int, int, char *, int);
Obj *FUN_001c6f70(Mgr *, int, int);
Obj *FUN_001c70c0(Mgr *);
void FUN_001caee4(Mgr *, int, V4 *, V4 *);
float FUN_0000bc34(float);
float FUN_0000bf34(float);
void FUN_00022418(V4 *, const V4 *, const V4 *);
void FUN_002876a0(Obj *, int, int, int);
void *FUN_00000bcc(unsigned int);
}

inline void *operator new(unsigned int, void *p) { return p; }
inline TaskBase::TaskBase() { vt = DAT_003083cc; }
inline Task::Task(int i) : TaskBase() { vt = DAT_00308af4; id = i; count = 1; frames = 30; }

inline V4 operator+(const V4 &a, const V4 &b)
{
    V4 r;
    FUN_00022418(&r, &a, &b);
    return r;
}

extern "C" Task *FUN_00152de0(Cmd *self, Ctx *ctx)
{
    if (DAT_003402f0 != 0) {
        FUN_0014d044(ctx);
        FUN_0014fd94(DAT_002dc714, self->id, self->mode, self->arg, self->wait ? DAT_002dc734 : DAT_002dc4c0, self->dur);
    }
    Obj *obj;
    Obj *def = ctx->obj;
    int id = self->id;
    if (id == -1)
        obj = DAT_0034a118;
    else if (id < 0)
        obj = def;
    else if (id < 100)
        obj = DAT_0034a114->objs[id];
    else if (id < 1000)
        obj = FUN_001c6f70(DAT_0034a114, id, 0);
    else
        obj = FUN_001c70c0(DAT_0034a114);
    if (obj == 0)
        return 0;
    V4 pos(DAT_002bc440, DAT_002bc444, DAT_002bc448, DAT_002bc44c);
    switch (self->mode) {
    case 0: {
        V4 tmp;
        FUN_001caee4(DAT_0034a114, self->arg, &pos, &tmp);
        break;
    }
    case 1:
    {
        id = self->arg;
        Obj *o;
        if (id == -1)
            o = DAT_0034a118;
        else if (id < 0)
            o = def;
        else if (id < 100)
            o = DAT_0034a114->objs[id];
        else if (id < 1000)
            o = FUN_001c6f70(DAT_0034a114, id, 0);
        else
            o = FUN_001c70c0(DAT_0034a114);
        pos = *o->GetPos();
    }
        break;
    case 2: {
        float a = 0.017453292f * (float)self->arg;
        pos = *obj->GetPos() + V4(FUN_0000bf34(a), 0.0f, FUN_0000bc34(a), 0.0f);
        break;
    }
    default:
        break;
    }
    if (self->dur <= 0)
        obj->SetPos(&pos);
    else
        FUN_002876a0(obj, self->mode, self->arg, self->dur);
    if (self->wait == 0) {
        Task *t = (Task *)FUN_00000bcc(16);
        new (t) Task(self->id);
        return t;
    }
    return 0;
}
