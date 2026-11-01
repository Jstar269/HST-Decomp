/* HSTMATCH module=hstp_trans_main.prx start=0x001e8264 end=0x001e82a0 symbol=FUN_001e8264 lang=c++ */
/* Clone of src/eboot/call_virtual_func_001da180.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Base {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual int v8();
};

extern "C" void FUN_00108aec(Base *self);

extern "C" void FUN_001e8264(Base *self)
{
    if (self->v8()) {
        FUN_00108aec(self);
    }
}
