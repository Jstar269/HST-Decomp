/* HSTMATCH module=EBOOT.BIN start=0x00065104 end=0x00065138 symbol=FUN_00065104 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

#pragma exceptions off

struct Target {
    virtual void v0();
    virtual void v1();
};

struct Inner {
    Target **items;
    int pad[2];
};

struct Container {
    void *pad;
    Inner *rows;
};

extern "C" void FUN_00065104(Container *c, int a1, int a2) {
    c->rows[a1].items[a2]->v1();
}
