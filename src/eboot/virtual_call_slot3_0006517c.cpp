/* HSTMATCH module=EBOOT.BIN start=0x0006517c end=0x000651b0 symbol=FUN_0006517c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

#pragma exceptions off

struct Target {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

struct Inner {
    Target **items;
    int pad[2];
};

struct Container {
    void *pad;
    Inner *rows;
};

extern "C" void FUN_0006517c(Container *c, int a1, int a2) {
    c->rows[a1].items[a2]->v3();
}
