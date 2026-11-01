/* HSTMATCH module=hstp_trans_main.prx start=0x00187c48 end=0x00187da4 symbol=FUN_00187c48 lang=c++ */
/* Clone of src/eboot/dtor_clear_vec_0017b554.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
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

struct Self_00187c48 {
    int vptr;
    int pad4;
    Member8 m8;
    Member20 m20;

    It begin() { return It(m8.items_data); }
    It end() { return It(m8.items_data + m8.items_size); }
    void shrink(int n, Alloc) { m8.items_size -= n; }
};

extern "C" {
extern char DAT_003b2394;
void FUN_0020c8c4(int);
int FUN_00212af8(int);
void FUN_00094a98(void *, int);
void FUN_00093544(void *, int);
void FUN_00000a00(void *);
void FUN_00188354(void *);
void FUN_000b75e8(void *, int);
void FUN_0018a780(void *, int);

static inline void destroy_m20(void *p) {
    if (p) {
        FUN_000b75e8(p, 0);
    }
}

static inline void destroy_m8(void *p) {
    if (p) {
        FUN_0018a780(p, 0);
    }
}

Self_00187c48 *FUN_00187c48(Self_00187c48 *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_003b2394;
        FUN_0020c8c4(0);
        for (It it = self->begin(); it != self->end(); ++it) {
            Item *item = *it;
            for (int i = 0; i < 6; i++) {
                int u = FUN_00212af8(i);
                FUN_00094a98(&item->sub16, u);
            }
            if (item != 0) {
                if (item != 0) {
                    FUN_00093544(&item->sub16, -1);
                    FUN_00000a00(item);
                }
            }
        }
        int n = self->m8.items_size;
        self->shrink(n, Alloc());
        FUN_00188354(self);
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
