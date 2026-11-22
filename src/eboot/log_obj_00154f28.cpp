/* HSTMATCH module=EBOOT.BIN start=0x00154f28 end=0x00155040 symbol=FUN_00154f28 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad0[4]; };
struct PBase { char pad0[0xe0]; };
struct Player : PBase, Obj {};
struct Mgr { char pad0[0x4c]; Obj **objs; };
struct Ctx { int unk0; int unk4; int unk8; Obj *obj; };
struct Cmd { void *vt; int unk4; int id; float amount; int arg1; int arg2; };
extern "C" {
extern unsigned char DAT_003402f0;
extern char DAT_002dca38[];
extern Mgr *DAT_0034a114;
extern Player *DAT_0034a118;
void FUN_0014d044(Ctx *ctx);
void FUN_0014fd94(char *fmt, ...);
long long FUN_00020f5c(float);
void FUN_00288470(Obj *o, float a, int b);
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
extern "C" int FUN_00154f28(Cmd *self, Ctx *ctx)
{
    if (DAT_003402f0) {
        FUN_0014d044(ctx);
        long long amount = FUN_00020f5c(self->amount);
        FUN_0014fd94(DAT_002dca38, self->id, self->arg2, amount, self->arg1);
    }
    Obj *obj = Find(self->id, ctx->obj, self->arg2);
    if (obj)
        FUN_00288470(obj, self->amount, self->arg1);
    return 0;
}
