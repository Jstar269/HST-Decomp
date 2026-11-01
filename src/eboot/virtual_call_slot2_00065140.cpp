/* HSTMATCH module=EBOOT.BIN start=0x00065140 end=0x00065174 symbol=FUN_00065140 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

#pragma exceptions off

struct Target {
    virtual void v0();
    virtual void v1();
    virtual void v2();
};

struct Inner {
    Target **items;
    int pad[2];
};

struct Container {
    void *pad;
    Inner *rows;
};

extern "C" void FUN_00065140(Container *c, int a1, int a2) {
    c->rows[a1].items[a2]->v2();
}
