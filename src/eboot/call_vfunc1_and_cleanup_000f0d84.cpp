/* HSTMATCH module=EBOOT.BIN start=0x000f0d84 end=0x000f0db8 symbol=FUN_000f0d84 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Base {
    virtual void v0();
    virtual void v1();
};

extern "C" void FUN_000f10c0(Base *self);

extern "C" void FUN_000f0d84(Base *self)
{
    self->v1();
    FUN_000f10c0(self);
}
