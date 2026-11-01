/* HSTMATCH module=EBOOT.BIN start=0x000ffc70 end=0x000ffca4 symbol=FUN_000ffc70 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Child {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
};

struct Obj {
    int val_0;
    char pad[0x20 - 4];
    Child *child_20;
};

extern "C" void FUN_000ffc70(Obj *self, int val)
{
    self->val_0 = val;
    if (self->child_20 != 0) {
        self->child_20->m4();
    }
}
