/* HSTMATCH module=EBOOT.BIN start=0x001f10ec end=0x001f14a4 symbol=FUN_001f10ec lang=c++ */
/* Hand-written reconstruction from the disassembly (Active/Leave idioms from src/eboot/active_call_001e70e8.cpp and scene_intro_update_001f25a4.cpp); exact match verified; names and types await human review. */
struct Bits {
    signed char lo : 2;
    signed char mode : 2;
    signed char hi : 4;
};

struct Config {
    char pad[0x8b];
    Bits bits;
};

struct View {
    char pad[0x1b30];
    int state;
};

struct Entry {
    int kind;
    int arg;
    unsigned char flag;
};

struct Scene {
    char pad0[0x44];
    int started;
    char pad1[0x3c4 - 0x48];
    int f3c4;
    char pad2[0x510 - 0x3c8];
    View *view;
    char pad3[0x58c - 0x514];
    unsigned char busy;
    char pad4[0x5b8 - 0x58d];
    int sound;
    char pad5[0x12dc - 0x5bc];
    int state;
    Entry entries[10];
    int pad1358;
    int sel;
    int count;
    unsigned char is5;
};

extern "C" {
extern char DAT_0034179c[];
extern Config *DAT_002cfc8c;
extern int DAT_002ee858;
extern int DAT_002ee85c;
void FUN_001f4584(Scene *, int);
void FUN_001f4e40(Scene *, int);
void FUN_001e09dc(View *, int);
void FUN_001e2990(View *);
void FUN_001e5f50(View *, int, int, unsigned char);
void FUN_001e6ffc(View *, int);
int FUN_001b2af0(void *, int);
void FUN_001e611c(View *, unsigned char);
void FUN_001e70e0(View *);
void FUN_001e28e0(View *);
int FUN_001e7140(View *);
void FUN_001f44ec(Scene *, int);
int FUN_001e70e8(View *);
void FUN_001b1c44(void *, int);
void FUN_001f0fb4(Scene *);
void FUN_001f4bfc(Scene *);
void FUN_001f47a4(Scene *);
}

inline bool Active(View *o)
{
    switch (o->state) {
    case 1:
    case 2:
    case 3:
        return true;
    }
    return false;
}

inline int Clamp(const int &v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

inline void Leave(Scene *self)
{
    bool go;
    if (self->busy != 0)
        go = false;
    else
        go = DAT_002cfc8c->bits.mode == 0;
    if (go) {
        FUN_001b1c44(DAT_0034179c, 3);
        FUN_001f0fb4(self);
    }
}

extern "C" void FUN_001f10ec(Scene *self)
{
    if (self->started == 0) {
        self->state = 1;
        self->count = 0;
        FUN_001f4584(self, 0);
        FUN_001f4e40(self, self->f3c4);
        FUN_001e09dc(self->view, 2);
        FUN_001e2990(self->view);
        if (self->entries[self->sel].kind != 5)
            self->is5 = 0;
        else
            self->is5 = 1;
        FUN_001e5f50(self->view, self->entries[self->sel].kind, self->entries[self->sel].arg,
                     self->entries[self->sel].flag);
        FUN_001e6ffc(self->view, self->entries[self->sel].kind);
        if (self->entries[self->sel].flag != 0)
            self->sound = FUN_001b2af0(DAT_0034179c, 13);
        else
            self->sound = FUN_001b2af0(DAT_0034179c, 12);
    }
    switch (self->state) {
    case 1:
        if (++self->count == 5)
            FUN_001e611c(self->view, self->is5);
        if (self->count > 30) {
            self->state = 2;
            self->count = 0;
        }
        break;
    case 2:
        self->count++;
        self->count = Clamp(self->count, DAT_002ee858, DAT_002ee85c);
        if (self->count == 6) {
            if (Active(self->view))
                FUN_001e70e0(self->view);
        }
        if (self->count == 10) {
            if (!Active(self->view))
                FUN_001e28e0(self->view);
        }
        if (Active(self->view)) {
            if (FUN_001e7140(self->view))
                FUN_001f44ec(self, 0);
            if (self->count > 10 && FUN_001e70e8(self->view)) {
                FUN_001e28e0(self->view);
                Leave(self);
            }
        } else if (self->count > 10) {
            Leave(self);
        }
        break;
    }
    FUN_001f4bfc(self);
    FUN_001f47a4(self);
}
