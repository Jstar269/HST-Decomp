/* HSTMATCH module=EBOOT.BIN start=0x001521c8 end=0x001522b4 symbol=FUN_001521c8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual int v13();
};
struct PBase { char pad0[0xe0]; };
struct Player : PBase, Obj {};
struct Mgr { char pad0[0x4c]; Obj **objs; };
struct Ctx { int unk0; int unk4; int unk8; Obj *obj; };
struct Cmd { void *vt; int id; int wait; };
extern "C" {
extern Mgr *DAT_0034a114;
extern Player *DAT_0034a118;
Obj *FUN_001c6f70(Mgr *m, int id, int z);
Obj *FUN_001c70c0(Mgr *m);
}
inline Obj *Find(int id, Obj *obj)
{
    if (id == -1)
        return DAT_0034a118;
    if (id < 0)
        return obj;
    if (id < 100)
        return DAT_0034a114->objs[id];
    if (id < 1000)
        return FUN_001c6f70(DAT_0034a114, id, 0);
    return FUN_001c70c0(DAT_0034a114);
}
extern "C" int FUN_001521c8(Cmd *self, Ctx *ctx)
{
    if (self->wait > 0) {
        self->wait--;
        return 0;
    }
    Obj *obj = Find(self->id, ctx->obj);
    return obj ? obj->v13() : 1;
}
