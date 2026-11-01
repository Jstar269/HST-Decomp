/* HSTMATCH module=EBOOT.BIN start=0x001cdcb0 end=0x001cdcdc symbol=FUN_001cdcb0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual void m6();
    virtual void m7();
    virtual void m8();
    virtual void m9();
    virtual void m10();
    virtual int m11();
};

extern "C" bool FUN_001cdcb0(Obj *self) {
    return self->m11() == 4;
}
