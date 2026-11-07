/* HSTMATCH module=hstp_trans_main.prx start=0x001619a0 end=0x00161a70 symbol=FUN_001619a0 lang=c++ */
/* Hand-written reconstruction: clone of the source of cmd_factory_0015f514.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (port of src/eboot/cmd_factory_001532e4.cpp, its twin: addresses substituted by try/twins-20261026/port.py, the missing symbol decoded from retail; no other change); exact match verified; names and types await human review. */
/* Script-command factory: optional trace hook and debug print (0x14d044, variadic stub 0x14fd94), call 0x1ccf40
   on the manager global, then `new` a 4-byte command object (base vtable 0x3b1698 = C_0014c8cc, derived vtable
   0x3b1d8c whose slot 0 is the deleting destructor 0x1533b4 and slot 1 the init method 0x153410), run its
   init(t, ctx) and on failure delete it (explicit null test plus delete's own) and return 0. The float is
   declared before the context pointer: EABI gives the same registers either way, but the order of the
   prologue moves and of the call-argument setup follows the declaration order. Names provisional. */
struct Ctx_0015f514;
struct Mgr_0015f514;
extern "C" {
extern unsigned char DAT_003e9630;
extern char DAT_00383fdc[];
extern Mgr_0015f514 *DAT_003f344c;
void FUN_00158b2c(Ctx_0015f514 *ctx);
void FUN_0015bb68(char *fmt, ...);
void FUN_001db7c8(Mgr_0015f514 *m);
}
class C_0014c8cc {
public:
    virtual ~C_0014c8cc();
    virtual int Init(float t, Ctx_0015f514 *ctx) = 0;
};
class C_001619a0 : public C_0014c8cc {
public:
    virtual ~C_001619a0();
    virtual int Init(float t, Ctx_0015f514 *ctx);
};
extern "C" C_0014c8cc *FUN_001619a0(void *self, float t, Ctx_0015f514 *ctx)
{
    if (DAT_003e9630) {
        FUN_00158b2c(ctx);
        FUN_0015bb68(DAT_00383fdc);
    }
    FUN_001db7c8(DAT_003f344c);
    C_0014c8cc *c = new C_001619a0;
    if (c->Init(t, ctx)) {
        if (c)
            delete c;
        return 0;
    }
    return c;
}
// EXACT (hstmatch) only with --symbols-extra holding `__vt__10C_001619a0 = 0x3b1d8c;` (the derived class's
// vtable, stored by the inline constructor of `new C_001619a0`). Needs that entry in symbols/EBOOT.BIN.txt;
// then move to src/eboot/cmd_factory_0015f514.cpp. The family member 0x1554d0 needs its own derived vtable symbol.
