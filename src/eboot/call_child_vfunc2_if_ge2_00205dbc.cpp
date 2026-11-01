/* HSTMATCH module=EBOOT.BIN start=0x00205dbc end=0x00205df4 symbol=FUN_00205dbc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Child {
    virtual void m0();
    virtual void m1();
    virtual void m2();
};

struct Obj {
    Child *child_0;
    char pad[0x4c - 4];
    int f4c;
};

extern "C" void FUN_00205dbc(Obj *self)
{
    if (self->f4c >= 2) {
        self->child_0->m2();
    }
}
