/* HSTMATCH module=EBOOT.BIN start=0x001f483c end=0x001f4900 symbol=FUN_001f483c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int DAT_003313c4;
extern "C" void FUN_00049774(void *);
extern "C" void FUN_00064194(int, void *);

struct Inner {
    char pad0[8];
    float val;
};

struct Item {
    int pad0;
    Inner **inner;
    int pad8[3];
    float val;
    char pad18[0x1c];
};

struct Container {
    char pad0[0xf0];
    Item items[2];
    int start;
    int end;

    bool IsFinished() const {
        for (int i = start; i <= end; i++) {
            float limit = items[i].inner != 0 ? (*items[i].inner)->val : 0.0f;
            if (items[i].val < limit) {
                return false;
            }
        }
        return true;
    }
};

struct Handle {
    Container *c;
};

struct Obj {
    char pad0[0x3c0];
    Handle *h;
};

extern "C" void FUN_001f483c(Obj *self)
{
    Handle *h = self->h;
    if (h != 0) {
        if (!h->c->IsFinished()) {
            FUN_00049774(h);
        }
        FUN_00064194(DAT_003313c4, self->h);
    }
}
