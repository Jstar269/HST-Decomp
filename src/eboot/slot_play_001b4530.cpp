/* HSTMATCH module=EBOOT.BIN start=0x001b4530 end=0x001b464c symbol=FUN_001b4530 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj {
  int slot[0x1da];
  char pad[0x7bc - 0x1da * 4];
  int handle;
  int idx;
};

struct Obj2 {
  int slot[0x1da];
  char pad[0x7bc - 0x1da * 4];
  int handle;
  int idx;
};

struct Wrap {
  Obj *o;
};

extern "C" {
extern float DAT_002e515c[];
void FUN_00062904(int, int);
int FUN_000627e8(int, int);
int FUN_00062a6c(int);
void FUN_00062a9c(int, int, int);
}

static inline int HasSlot(Obj *o)
{
  int ok = o->idx < 0x1da;
  if (!ok)
    return ok;
  return o->slot[o->idx] != 0;
}

extern "C" void FUN_001b4530(Wrap *w)
{
  Obj *o = w->o;
  if (HasSlot(o) && o->handle >= 0) {
    FUN_00062904(o->slot[((Obj2 *)o)->idx], o->handle);
    o->handle = -1;
  }
  if (HasSlot(o)) {
    int idx = o->idx;
    o->handle = FUN_000627e8(o->slot[idx], 0);
    float vol = DAT_002e515c[idx];
    int ok;
    if (o->handle < 0 || FUN_00062a6c(o->handle) == 0) {
      o->handle = -1;
      ok = 0;
    } else {
      ok = 1;
    }
    if (ok) {
      int iv = (int)(vol * 128.0f);
      FUN_00062a9c(o->handle, iv, iv);
    }
  }
}
