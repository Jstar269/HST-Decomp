/* HSTMATCH module=hstp_trans_main.prx start=0x000f644c end=0x000f6480 symbol=FUN_000f644c lang=c++ */
/* Clone of src/eboot/call_vfunc1_and_cleanup_000f0d84.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Base {
    virtual void v0();
    virtual void v1();
};

extern "C" void FUN_000f6788(Base *self);

extern "C" void FUN_000f644c(Base *self)
{
    self->v1();
    FUN_000f6788(self);
}
