/* HSTMATCH module=hstp_trans_main.prx start=0x000d7fc0 end=0x000d80f4 symbol=FUN_000d7fc0 lang=c++ */
/* Clone of src/eboot/dtor_clear_vec_000d42fc.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Alloc {};
struct Tag {};

struct Item {
    virtual void destroy(int flag);
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

struct Self_000d4288 {
    int vptr;
    char pad4[0x38];
    Item **items_data;
    int items_size;
    int items_cap;
    void *recs_data;
    int recs_size;
    int recs_cap;

    It begin() { return It(items_data); }
    It end() { return It(items_data + items_size); }
    void shrink(int n, Alloc) { items_size -= n; }
};

extern "C" {
extern char DAT_003b0734;
void FUN_000d9488(void *, int, Tag);
void FUN_000d958c(void *, int);
void FUN_000702c4(void *, int);
void FUN_0001ed28(void *, int);
void __dl__FPv(void *);

static inline void destroy_recs(void *p) {
    if (p) {
        FUN_000d958c(p, 0);
    }
}

static inline void destroy_items(void *p) {
    if (p) {
        FUN_000702c4(p, 0);
    }
}

Self_000d4288 *FUN_000d7fc0(Self_000d4288 *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_003b0734;
        for (It it = self->begin(); it != self->end(); ++it) {
            Item *item = *it;
            if (item != 0) {
                if (item != 0) {
                    item->destroy(1);
                }
                *it = 0;
            }
        }
        int n = self->items_size;
        self->shrink(n, Alloc());
        FUN_000d9488(&self->recs_data, self->recs_size, Tag());
        if (&self->recs_data) {
            destroy_recs(&self->recs_data);
        }
        if (&self->items_data) {
            destroy_items(&self->items_data);
        }
        FUN_0001ed28(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
}
