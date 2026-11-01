/* HSTMATCH module=hstp_trans_main.prx start=0x00105570 end=0x001055a4 symbol=FUN_00105570 lang=c++ */
/* Clone of src/eboot/set_val_and_notify_000ffc3c.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
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

extern "C" void FUN_00105570(Obj *self, short val)
{
    self->val_c = val;
    if (self->child_20 != 0) {
        self->child_20->m5();
    }
}
