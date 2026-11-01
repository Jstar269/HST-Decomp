/* HSTMATCH module=hstp_trans_main.prx start=0x001dca04 end=0x001dca30 symbol=FUN_001dca04 lang=c++ */
/* Clone of src/eboot/is_vfunc_five_001ce328.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
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

extern "C" bool FUN_001dca04(Obj *self) {
    return self->m11() == 5;
}
