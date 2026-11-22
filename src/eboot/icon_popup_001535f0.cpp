/* HSTMATCH module=EBOOT.BIN start=0x001535f0 end=0x0015371c symbol=FUN_001535f0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad0[4]; };
struct PBase { char pad0[0xe0]; };
struct Player : PBase, Obj {};
struct Mgr { char pad0[0x4c]; Obj **objs; };
struct Ctx { int unk0; int unk4; int unk8; Obj *obj; };
struct Cmd { void *vt; int unk4; int id; int a; int b; unsigned char flag; };
extern "C" {
extern unsigned char DAT_003402f0;
extern char DAT_002dc774[];
extern char DAT_002dc7a4[];
extern char DAT_002dc7a8[];
extern Mgr *DAT_0034a114;
extern Player *DAT_0034a118;
void FUN_0014d044(Ctx *ctx);
void FUN_0014fd94(char *fmt, ...);
void FUN_002875f8(Obj *o, int a, unsigned char f, int b);
Obj *FUN_001c6f70(Mgr *m, int id, int z);
Obj *FUN_001c70c0(Mgr *m);
}
inline Obj *Find(int id, Obj *obj, int arg)
{
    if (id == -1)
        return DAT_0034a118;
    if (id < 0)
        return obj;
    if (id < 100)
        return DAT_0034a114->objs[id];
    if (id < 1000)
        return FUN_001c6f70(DAT_0034a114, id, arg);
    return FUN_001c70c0(DAT_0034a114);
}
extern "C" int FUN_001535f0(Cmd *self, Ctx *ctx)
{
    if (DAT_003402f0) {
        FUN_0014d044(ctx);
        FUN_0014fd94(DAT_002dc774, self->id, self->a, self->b, self->flag ? DAT_002dc7a4 : DAT_002dc7a8);
    }
    Obj *obj = Find(self->id, ctx->obj, 0);
    if (obj)
        FUN_002875f8(obj, self->a, self->flag, self->b);
    return 0;
}
