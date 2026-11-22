/* HSTMATCH module=EBOOT.BIN start=0x001ee998 end=0x001eed14 symbol=FUN_001ee998 lang=c++ */
/* Ghidra pseudocode (hstgen.py tag fm10, wip draft) corrected by hand: types, delete/if nesting, bool helper, camera struct, DAT_ extern arrays; exact match verified; names and types await human review. */
/* matching idiom: the four repeated `p != 0` tests per member destructor (inlined nested member dtors) and the bool helper with if/else assignment. */
typedef unsigned int undefined4;
struct VC { virtual ~VC(); };

extern char DAT_0030ebf4[];
extern char DAT_003094e0[];
extern char DAT_003094fc[];
extern char DAT_00309510[];
extern char DAT_003070bc[];
extern char DAT_00341d93;
extern char DAT_00341d7d;
extern char DAT_00341d7e;
struct Sub { char pad[0x60]; float f60; float f64; };
struct Cam { int pad0; Sub **sub; char pad2[0x6c]; float f74; float f78; };
extern Cam *DAT_003415ac;

extern "C" void FUN_0001cb88(void *);
extern "C" void FUN_001f2f08(...);
extern "C" void FUN_00219184(void *, int);
extern "C" void FUN_00216bc4(void *, int);
extern "C" void FUN_00194110(...);
extern "C" void FUN_002ad74c(...);
extern "C" void FUN_00054554(...);
extern "C" void FUN_00000788(...);
extern "C" void FUN_00066338(...);
extern "C" void FUN_00064a08(...);
extern "C" void FUN_0001f0f0(...);
extern "C" void FUN_00000a00(...);
extern "C" void FUN_001ee87c(...);
extern "C" void FUN_001ee8ec(...);

static inline bool Pred(unsigned *p)
{
  bool r;
  if (p[0x205f] == 1 && p[0x205e] == 1)
    r = true;
  else
    r = false;
  return r;
}

extern "C" undefined4 *FUN_001ee998(undefined4 *param_1, short param_2)
{
  undefined4 *puVar3;

  if (param_1 != (undefined4 *)0x0) {
    *param_1 = (undefined4)DAT_003094e0;
    param_1[0xf] = (undefined4)DAT_003094fc;
    FUN_0001cb88(DAT_0030ebf4);
    FUN_001f2f08(param_1);
    if (param_1[0x161] != 0) {
      delete (VC *)param_1[0x161];
      param_1[0x161] = 0;
    }
    if (param_1[0x159] != 0) {
      FUN_00219184((void *)param_1[0x159], 1);
      param_1[0x159] = 0;
    }
    if (param_1[0x158] != 0) {
      FUN_00216bc4((void *)param_1[0x158], 1);
      param_1[0x158] = 0;
    }
    FUN_00194110();
    if (Pred(param_1))
      DAT_00341d93 = 1;
    else
      DAT_00341d93 = 0;
    DAT_00341d7d = 0xff;
    DAT_00341d7e = 0xff;
    {
      Cam *c = DAT_003415ac;
      if (c != 0) {
        c->f74 = 1.0f;
        c->f78 = 1.0f;
        Cam *d = DAT_003415ac;
        (*d->sub)->f60 = 240.0f;
        (*d->sub)->f64 = 136.0f;
      }
    }
    FUN_002ad74c();
    puVar3 = param_1 + 0x2061;
    if (puVar3 != 0 && puVar3 != 0) {
      FUN_00054554(puVar3, 0);
    }
    FUN_00000788(param_1 + 0x1ddb, FUN_001ee87c, 0x14, 0x80);
    FUN_00000788(param_1 + 0x4db, FUN_001ee87c, 0x14, 0x500);
    FUN_00000788(param_1 + 0x43f, FUN_001ee8ec, 0xe4, 2);
    if (param_1 + 0x433 != 0) {
      param_1[0x433] = (undefined4)DAT_003070bc;
      if (param_1 + 0x43b != 0 && param_1 + 0x43b != 0 && param_1 + 0x43b != 0 && param_1 + 0x43b != 0) {
        FUN_00066338(param_1 + 0x43b, 0);
      }
      if (param_1 + 0x437 != 0 && param_1 + 0x437 != 0 && param_1 + 0x437 != 0 && param_1 + 0x437 != 0) {
        FUN_00066338(param_1 + 0x437, 0);
      }
      FUN_00064a08(param_1 + 0x433, 0);
    }
    if (param_1 + 0x415 != 0) {
      param_1[0x415] = (undefined4)DAT_003070bc;
      if (param_1 + 0x41d != 0 && param_1 + 0x41d != 0 && param_1 + 0x41d != 0 && param_1 + 0x41d != 0) {
        FUN_00066338(param_1 + 0x41d, 0);
      }
      if (param_1 + 0x419 != 0 && param_1 + 0x419 != 0 && param_1 + 0x419 != 0 && param_1 + 0x419 != 0) {
        FUN_00066338(param_1 + 0x419, 0);
      }
      FUN_00064a08(param_1 + 0x415, 0);
    }
    if (param_1 + 0x17e != 0) {
      param_1[0x17e] = (undefined4)DAT_003070bc;
      if (param_1 + 0x186 != 0 && param_1 + 0x186 != 0 && param_1 + 0x186 != 0 && param_1 + 0x186 != 0) {
        FUN_00066338(param_1 + 0x186, 0);
      }
      if (param_1 + 0x182 != 0 && param_1 + 0x182 != 0 && param_1 + 0x182 != 0 && param_1 + 0x182 != 0) {
        FUN_00066338(param_1 + 0x182, 0);
      }
      FUN_00064a08(param_1 + 0x17e, 0);
    }
    if (param_1 + 0xf != 0) {
      param_1[0xf] = (undefined4)DAT_00309510;
    }
    FUN_0001f0f0(param_1, 0);
    if (0 < param_2) {
      FUN_00000a00(param_1);
    }
  }
  return param_1;
}
// near miss from lane fm10 (unreviewed)
