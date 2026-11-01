/* HSTMATCH module=EBOOT.BIN start=0x00100d54 end=0x00100d8c symbol=FUN_00100d54 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

struct Base {
    virtual ~Base();
};

struct Obj_00100d54 : public Base {
    char pad[0x48 - 4];
    int id;
};

extern "C" void FUN_00100d54(Obj_00100d54 *self, int target_id)
{
    if (self->id == target_id) {
        delete self;
    }
}
