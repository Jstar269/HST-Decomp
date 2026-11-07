/* HSTMATCH module=EBOOT.BIN start=0x001554d0 end=0x001555a0 symbol=FUN_001554d0 lang=c++ data=0x002dcaf4 */
/* Hand-written reconstruction: clone of the source of cmd_factory_001532e4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Data: its string literals (1 strings, 0x002dcaf4-0x002dcaff) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
/* Script-command factory: optional trace hook and debug print (0x14d044, variadic stub 0x14fd94), call 0x1ccf40
   on the manager global, then `new` a 4-byte command object (base vtable 0x3083cc = C_0014c8cc, derived vtable
   0x308ac0 whose slot 0 is the deleting destructor 0x1533b4 and slot 1 the init method 0x153410), run its
   init(t, ctx) and on failure delete it (explicit null test plus delete's own) and return 0. The float is
   declared before the context pointer: EABI gives the same registers either way, but the order of the
   prologue moves and of the call-argument setup follows the declaration order. Names provisional. */
#include "hst/script_cmd.h"

struct Mgr_001532e4;
extern "C" {
extern unsigned char DAT_003402f0; /* script trace flag */
extern Mgr_001532e4 *DAT_0034a114;
void FUN_0014d044(ScriptCtx *ctx);
void FUN_0014fd94(char *fmt, ...);
void FUN_001cd060(Mgr_001532e4 *m);
}
class C_0014c8cc {
public:
    virtual ~C_0014c8cc();
    virtual int Init(float t, ScriptCtx *ctx) = 0;
};
class C_001554d0 : public C_0014c8cc {
public:
    virtual ~C_001554d0();
    virtual int Init(float t, ScriptCtx *ctx);
};
/* Script command "ReturnMenu": notify the manager, then build the command object and run its init. */
extern "C" C_0014c8cc *FUN_001554d0(ScriptCmd *cmd, float t, ScriptCtx *ctx)
{
    if (DAT_003402f0) {
        FUN_0014d044(ctx);
        FUN_0014fd94("ReturnMenu\n");
    }
    FUN_001cd060(DAT_0034a114);
    C_0014c8cc *c = new C_001554d0;
    if (c->Init(t, ctx)) {
        if (c)
            delete c;
        return 0;
    }
    return c;
}
// The derived class's vtable (0x00308694, stored by the inline constructor of `new C_001554d0`) is the symbol __vt__10C_001554d0 in symbols/EBOOT.BIN.txt.
