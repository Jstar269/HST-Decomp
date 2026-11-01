/* HSTMATCH module=EBOOT.BIN start=0x0015e33c end=0x0015e470 symbol=FUN_0015e33c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Alloc {};

struct Elem {
    virtual void destroy(int flag);
    virtual int probe();
};

struct It {
    Elem **p;
    It(Elem **x) : p(x) {}
    It(const It& x) : p(x.p) {}
    It& operator++() { ++p; return *this; }
    Elem*& operator*() const { return *p; }
};

static inline int operator!=(It a, It b) {
    return (a.p == b.p) ^ 1;
}

struct Vector {
    Elem **data;
    int size;
    int cap;
};

struct Self_0015e33c {
    int vptr;
    int pad4;
    void *p8;
    int padc;
    int pad10;
    Vector vec;

    It begin() { return It(vec.data); }
    It end() { return It(vec.data + vec.size); }
    void shrink(int n, Alloc) { vec.size -= n; }
};

extern "C" {
extern char DAT_0030908c;
extern char DAT_003073ac;
void FUN_000009e4(void *);
void FUN_001602dc(void *, int);
void FUN_00000a00(void *);

static inline void destroy_vec(void *p) {
    if (p) {
        FUN_001602dc(p, 0);
    }
}

Self_0015e33c *FUN_0015e33c(Self_0015e33c *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_0030908c;
        for (It it = self->begin(); it != self->end(); ++it) {
            if ((*it)->probe()) {
                if (*it != 0) {
                    (*it)->destroy(1);
                }
            }
        }
        int n = self->vec.size;
        self->shrink(n, Alloc());
        if (self->p8 != 0) {
            FUN_000009e4(self->p8);
            self->p8 = 0;
        }
        if (&self->vec) {
            destroy_vec(&self->vec);
        }
        if (self != 0) {
            self->vptr = (int)&DAT_003073ac;
        }
        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
}
