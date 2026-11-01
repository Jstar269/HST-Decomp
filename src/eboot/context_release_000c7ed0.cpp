/* HSTMATCH module=EBOOT.BIN start=0x000c7ed0 end=0x000c7fc0 symbol=FUN_000c7ed0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    virtual ~Obj();
};

struct Context {
    char pad0[0x154];
    int field_154;
    int field_158;
    int field_15c;
    int field_160;
    char pad1[0x28];
    Obj *field_18c[10];
    Obj **field_1b4;
    int field_1b8;
};

struct False {};

extern "C" void FUN_000c7ed0(Context *self, False) {
    self->field_154 = 0;
    self->field_158 = 0;
    self->field_15c = 0;
    self->field_160 = 0;

    int n = self->field_1b8;
    Obj **p = self->field_1b4;
    int m = (p == (Obj **)((char *)self->field_1b4 + n * 4)) ^ 1;
    if (m) {
        do {
            Obj *o = *p;
            if (o != 0) {
                delete o;
                *p = 0;
            }
            p = p + 1;
            n = self->field_1b8;
            m = (p == (Obj **)((char *)self->field_1b4 + n * 4)) ^ 1;
        } while (m);
    }

    self->field_1b8 -= n;

    for (int i = 0; i < 10; ++i) {
        Obj *o = self->field_18c[i];
        if (o != 0) {
            delete o;
            self->field_18c[i] = 0;
        }
    }
}
