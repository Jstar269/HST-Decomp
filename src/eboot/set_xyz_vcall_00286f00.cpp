/* HSTMATCH module=EBOOT.BIN start=0x00286f00 end=0x00286f2c symbol=FUN_00286f00 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
struct Obj {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(int a, int b, int c, int d);
    char pad[0x30 - 4]; int x; int y; int z;
};
extern "C" void FUN_00286f00(Obj *self, int x, int y, int z, int a, int b, int c, int d) { self->x = x; self->y = y; self->z = z; self->v4(a, b, c, d); }
