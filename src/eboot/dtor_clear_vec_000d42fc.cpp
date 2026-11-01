/* HSTMATCH module=EBOOT.BIN start=0x000d42fc end=0x000d4430 symbol=FUN_000d42fc lang=c++ */
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
extern char DAT_00307468;
void FUN_000d5798(void *, int, Tag);
void FUN_000d589c(void *, int);
void FUN_0006eb04(void *, int);
void FUN_0001f0f0(void *, int);
void __dl__FPv(void *);

static inline void destroy_recs(void *p) {
    if (p) {
        FUN_000d589c(p, 0);
    }
}

static inline void destroy_items(void *p) {
    if (p) {
        FUN_0006eb04(p, 0);
    }
}

Self_000d4288 *FUN_000d42fc(Self_000d4288 *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_00307468;
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
        FUN_000d5798(&self->recs_data, self->recs_size, Tag());
        if (&self->recs_data) {
            destroy_recs(&self->recs_data);
        }
        if (&self->items_data) {
            destroy_items(&self->items_data);
        }
        FUN_0001f0f0(self, 0);
        if (flags > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
}
