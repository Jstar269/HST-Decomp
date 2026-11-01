/* HSTMATCH module=EBOOT.BIN start=0x0017b554 end=0x0017b6b0 symbol=FUN_0017b554 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Alloc {};

struct Item {
    char pad[16];
    char sub16[1];
};

struct It {
    Item **p;
    It(Item **x) : p(x) {}
    It(const It& x) : p(x.p) {}
    It& operator++() { ++p; return *this; }
    Item*& operator*() const { return *p; }
};

static inline int operator!=(It a, It b) {
    return (a.p == b.p) ^ 1;
}

struct Member20 {
    char data[1];
};

struct Member8 {
    Item **items_data;
    int items_size;
    int items_cap;
};

struct Self_0017b554 {
    int vptr;
    int pad4;
    Member8 m8;
    Member20 m20;

    It begin() { return It(m8.items_data); }
    It end() { return It(m8.items_data + m8.items_size); }
    void shrink(int n, Alloc) { m8.items_size -= n; }
};

extern "C" {
extern char DAT_003090c8;
void FUN_001fe0f4(int);
int FUN_00204290(int);
void FUN_00092c38(void *, int);
void FUN_000918ac(void *, int);
void FUN_00000a00(void *);
void FUN_0017bc60(void *);
void FUN_000b42f8(void *, int);
void FUN_0017e08c(void *, int);

static inline void destroy_m20(void *p) {
    if (p) {
        FUN_000b42f8(p, 0);
    }
}

static inline void destroy_m8(void *p) {
    if (p) {
        FUN_0017e08c(p, 0);
    }
}

Self_0017b554 *FUN_0017b554(Self_0017b554 *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_003090c8;
        FUN_001fe0f4(0);
        for (It it = self->begin(); it != self->end(); ++it) {
            Item *item = *it;
            for (int i = 0; i < 6; i++) {
                int u = FUN_00204290(i);
                FUN_00092c38(&item->sub16, u);
            }
            if (item != 0) {
                if (item != 0) {
                    FUN_000918ac(&item->sub16, -1);
                    FUN_00000a00(item);
                }
            }
        }
        int n = self->m8.items_size;
        self->shrink(n, Alloc());
        FUN_0017bc60(self);
        if (&self->m20) {
            destroy_m20(&self->m20);
        }
        if (&self->m8) {
            destroy_m8(&self->m8);
        }
        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
}
