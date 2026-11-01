/* HSTMATCH module=EBOOT.BIN start=0x002019cc end=0x002019fc symbol=FUN_002019cc lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Child {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
};

struct Obj {
    int f0;
    Child *child_4;
};

extern "C" void FUN_002019cc(Obj *self)
{
    if (self->child_4 != 0) {
        self->child_4->m3();
    }
}
