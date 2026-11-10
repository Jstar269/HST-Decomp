/* HSTMATCH module=EBOOT.BIN start=0x001f6108 end=0x001f64d4 symbol=FUN_001f6108 lang=c++ */
/* Hand-written reconstruction from the disassembly (started from the unreviewed draft wip/hole_advance_001f6108.cpp, 39 words off); exact match verified; names and types await human review. Clamp takes the value and the lower bound by value (bounds are const globals), the typeB test is a one-case switch. */
struct Course {
    unsigned char holes;
    char pad[288 - 1];
};

struct Stats {
    char pad[0x1054];
    int hole;
    char pad1058[0x33fc - 0x1058];
    int tries;
};

struct Round {
    char pad0[0x3d4];
    int course;
    char pad3d8[0x521 - 0x3d8];
    unsigned char online;
    char pad522[0x58f - 0x522];
    unsigned char locked;
    char pad590[0x599 - 0x590];
    unsigned char changed;
    char pad59a[0x103c - 0x59a];
    int cur;
    int next;
    int prev;
    unsigned char back;
    char pad1049[0x1050 - 0x1049];
    unsigned char fwd;
    unsigned char skip;
    char pad1052[2];
    char holes[4];
};

extern "C" {
extern Course DAT_00341834[];
extern int DAT_0034a678;
extern char DAT_002ee944[];
extern char DAT_002ee950[];
extern char DAT_002ee964[];
extern const int DAT_002ee934;
extern const int DAT_002ee938;
extern const int DAT_002ee93c;
extern const int DAT_002ee940;
Stats *FUN_0016bb88(void);
int FUN_0006531c(void *, const char *);
int FUN_00065104(void *, int, int);
void *FUN_0024e064(void);
int FUN_0024e268(void *, int, int);
int FUN_0024e2e8(void);
int FUN_0024e2d0(void);
int FUN_0024e2dc(void);
void FUN_001ef3f8(const char *, ...);
}

inline int Clamp(int v, int lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

extern "C" void FUN_001f6108(Round *self, int force)
{
    self->fwd = 0;
    self->skip = 0;
    int h = FUN_0016bb88()->hole;
    self->cur = h;
    self->next = h;
    int base = FUN_0006531c(self->holes, DAT_002ee944);
    int typeA = FUN_00065104(self->holes, base + self->cur, 1);
    int typeB = FUN_00065104(self->holes, base + self->cur, 2);
    int last = DAT_00341834[self->course].holes;
    bool ok = true;
    for (int i = 0; i < 15; i++) {
        int st = FUN_0024e268(FUN_0024e064(), 1, i);
        if (DAT_0034a678 == 49 && (unsigned char)FUN_0024e2e8() == 1 && FUN_0024e2d0() == i && FUN_0024e2dc() == 3) {
            if (self->online == 0) {
                ok = false;
                FUN_001ef3f8(DAT_002ee950);
            }
        } else if (st != 15) {
            ok = false;
            FUN_001ef3f8(DAT_002ee964, 1, i, st);
        }
    }
    int go = self->online;
    if (force) {
        if (self->locked)
            go = 0;
        else
            go = 1;
    }
    if (go) {
        if (self->cur < last)
            self->fwd = 1;
        if (force)
            self->fwd = 1;
        switch (typeA) {
        case 1:
            if (self->fwd)
                self->next = self->cur + 1;
            break;
        case 2:
            if (self->fwd) {
                if (FUN_0016bb88()->tries + 1 >= 5)
                    self->next = self->cur + 1;
            }
            break;
        case 3:
            break;
        }
        if (ok)
            self->next = 16;
        self->next = Clamp(self->next, DAT_002ee934, DAT_002ee938);
    } else {
        switch (typeB) {
        case 1: {
            int lt = self->cur < last;
            unsigned char back = 0;
            if (!lt)
                back = 1;
            if (force)
                back = true;
            if (back)
                self->next = self->cur - 1;
            break;
        }
        }
    }
    self->next = Clamp(self->next, DAT_002ee93c, DAT_002ee940);
    self->prev = self->cur;
    if (self->next != self->cur)
        self->changed = 1;
    if (self->next < self->cur)
        self->back = 1;
    else
        self->back = 0;
    if (self->cur < self->next) {
        if (FUN_00065104(self->holes, base + self->next, 1) == 2)
            self->skip = 1;
    }
    if (force) {
        self->changed = 1;
        self->back = 0;
    }
}
