/* HSTMATCH module=EBOOT.BIN start=0x000ffc3c end=0x000ffc70 symbol=FUN_000ffc3c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Child {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
};

struct Obj {
    char pad0[0xc];
    short val_c;
    char pad[0x20 - 0xe];
    Child *child_20;
};

extern "C" void FUN_000ffc3c(Obj *self, short val)
{
    self->val_c = val;
    if (self->child_20 != 0) {
        self->child_20->m5();
    }
}
