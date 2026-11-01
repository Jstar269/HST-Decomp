/* HSTMATCH module=hstp_trans_main.prx start=0x0010665c end=0x00106694 symbol=FUN_0010665c lang=c++ */
/* Clone of src/eboot/destroy_if_id_00100d54.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Base {
    virtual ~Base();
};

struct Obj_0010665c : public Base {
    char pad[0x48 - 4];
    int id;
};

extern "C" void FUN_0010665c(Obj_0010665c *self, int target_id)
{
    if (self->id == target_id) {
        delete self;
    }
}
