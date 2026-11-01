/* HSTMATCH module=hstp_trans_main.prx start=0x0016a8c8 end=0x0016a9fc symbol=FUN_0016a8c8 lang=c++ */
/* Clone of src/eboot/dtor_clear_vec_0015e33c.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
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

struct Self_0016a8c8 {
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
extern char DAT_003b2358;
extern char DAT_003b0678;
void FUN_000009e4(void *);
void FUN_0016c868(void *, int);
void FUN_00000a00(void *);

static inline void destroy_vec(void *p) {
    if (p) {
        FUN_0016c868(p, 0);
    }
}

Self_0016a8c8 *FUN_0016a8c8(Self_0016a8c8 *self, short flags) {
    if (self) {
        self->vptr = (int)&DAT_003b2358;
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
            self->vptr = (int)&DAT_003b0678;
        }
        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
}
