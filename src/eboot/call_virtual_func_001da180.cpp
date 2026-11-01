/* HSTMATCH module=EBOOT.BIN start=0x001da180 end=0x001da1bc symbol=FUN_001da180 lang=c++ */
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

extern "C" void FUN_001031e8(Base *self);

extern "C" void FUN_001da180(Base *self)
{
    if (self->v8()) {
        FUN_001031e8(self);
    }
}
