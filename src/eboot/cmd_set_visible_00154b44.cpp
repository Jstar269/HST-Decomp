/* HSTMATCH module=EBOOT.BIN start=0x00154b44 end=0x00154c5c symbol=FUN_00154b44 lang=c++ data=0x002dc9b4 */
/* Hand-written reconstruction: clone of the source of cmd_set_visible_001519b4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Data: its string literals (1 strings, 0x002dc9b4-0x002dc9cf) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
/* Script command "SetEnablePatrol %d %s": optional trace hook and debug print (0x14d044, variadic stub 0x14fd94,
   on/off strings), then resolve the target object by id (-1: the global player converted to its Obj base at
   +0xe0, a derived-to-base null-checked add; negative: the context object, read up front; < 100: manager
   table at +0x4c; < 1000: lookup 0x1c6f70; else 0x1c70c0) in an inline helper with early returns, and copy
   the visible flag into it. Names provisional. */
struct Obj_001519b4 { char pad0[0x57]; unsigned char visible; };
struct PBase_001519b4 { char pad0[0xe0]; };
struct Player_001519b4 : PBase_001519b4, Obj_001519b4 {};
struct Mgr_001519b4 { char pad0[0x4c]; Obj_001519b4 **objs; };
struct Ctx_001519b4 { int unk0; int unk4; int unk8; Obj_001519b4 *obj; };
struct Cmd_001519b4 { void *vt; int unk4; int id; unsigned char visible; };
/* Object ids: -1 is the global player, other negatives mean the context object. */
enum { OBJ_ID_PLAYER = -1, OBJ_ID_TABLE_LIMIT = 100, OBJ_ID_LOOKUP_LIMIT = 1000 };
extern "C" {
extern unsigned char DAT_003402f0; /* script trace flag */
extern char DAT_002dc900[];
extern char DAT_002dc904[];
extern Mgr_001519b4 *DAT_0034a114;
extern Player_001519b4 *DAT_0034a118;
void FUN_0014d044(Ctx_001519b4 *ctx);
void FUN_0014fd94(char *fmt, ...);
Obj_001519b4 *FUN_001c6f70(Mgr_001519b4 *m, int id, int z);
Obj_001519b4 *FUN_001c70c0(Mgr_001519b4 *m, int id);
}
inline Obj_001519b4 *Find_001519b4(int id, Ctx_001519b4 *ctx)
{
    Obj_001519b4 *obj = ctx->obj;
    if (id == OBJ_ID_PLAYER)
        return DAT_0034a118;
    if (id < 0)
        return obj;
    if (id < OBJ_ID_TABLE_LIMIT)
        return DAT_0034a114->objs[id];
    if (id < OBJ_ID_LOOKUP_LIMIT)
        return FUN_001c6f70(DAT_0034a114, id, 0);
    return FUN_001c70c0(DAT_0034a114, id);
}
extern "C" int FUN_00154b44(Cmd_001519b4 *self, Ctx_001519b4 *ctx)
{
    if (DAT_003402f0) {
        FUN_0014d044(ctx);
        FUN_0014fd94("SetEnablePatrol [%d:(%s)]\n", self->id, self->visible ? DAT_002dc900 : DAT_002dc904);
    }
    Obj_001519b4 *obj = Find_001519b4(self->id, ctx);
    if (obj)
        obj->visible = self->visible;
    return 0;
}
