/* HSTMATCH module=hstp_trans_main.prx start=0x000d6ef4 end=0x000d7070 symbol=FUN_000d6ef4 lang=c++ */
/* Clone of src/eboot/deleting_dtor_000d3298.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
// deleting_dtor from retail 0x000d6ef4 (380 bytes)
#pragma inline_depth(8)

struct Tag {};

extern "C" {
extern void *DAT_003b0750;
extern char DAT_00057904;
void FUN_00000788(void *, void *, int, int);
void FUN_00000a00(void *);
void FUN_0001ed28(void *, int);
void FUN_000541d8(void *, int);
void FUN_000569f4(void *, int);
void FUN_000702c4(void *, int);
void FUN_000d933c(void *, int, Tag = Tag());
void FUN_000d9420(void *, int);
}

struct Item {
    virtual ~Item();
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

struct LayerM60_2 {
    ~LayerM60_2() {
        FUN_000702c4(this, 0);
    }
};
struct LayerM60_1 : LayerM60_2 {};

struct Member60 : LayerM60_1 {
    Item **items_data;
    int items_size;
    int items_cap;

    It begin() { return It(items_data); }
    It end() { return It(items_data + items_size); }
};

struct LayerM72_2 {
    ~LayerM72_2() {
        FUN_000d9420(this, 0);
    }
};
struct LayerM72_1 : LayerM72_2 {};

struct Member72 : LayerM72_1 {
    int field_0;
    int field_4;
};

struct Self {
    void *vtable;
    char pad0[56];
    Member60 m60;  // 60
    Member72 m72;  // 72
    char pad1[144]; // 80 to 224: 224 - 80 = 144
    char m_e0[432]; // 224 (0xe0) to 656
    char pad2[1300]; // 656 to 1956 (0x7a4): 1956 - 656 = 1300
    void *m_7a4; // 1956
    void *m_7a8; // 1960

    It begin() { return It(m60.items_data); }
    It end() { return It(m60.items_data + m60.items_size); }
};

static inline void destroy_m72(void *p) {
    if (p) {
        FUN_000d9420(p, 0);
    }
}
static inline void destroy_m60(void *p) {
    if (p) {
        FUN_000702c4(p, 0);
    }
}

static inline void shrink(int count, Member60 *m) { m->items_size -= count; }

extern "C" Self *FUN_000d6ef4(Self *self, short flags)
{
    if (self != 0) {
        self->vtable = &DAT_003b0750;
        for (It it = self->begin(); it != self->end(); ++it) {
            Item *item = *it;
            if (item != 0) {
                delete item;
                *it = 0;
            }
        }
        shrink(self->m60.items_size, &self->m60);
        FUN_000d933c(&self->m72, self->m72.field_4, Tag());
        if (self->m_7a4 != 0) {
            FUN_000541d8(self->m_7a4, 1);
            self->m_7a4 = 0;
        }
        if (self->m_7a8 != 0) {
            FUN_000569f4(self->m_7a8, 1);
            self->m_7a8 = 0;
        }
        FUN_00000788(self->m_e0, &DAT_00057904, 432, 4);
        if (&self->m72) {
            destroy_m72(&self->m72);
        }
        if (&self->m60) {
            destroy_m60(&self->m60);
        }
        FUN_0001ed28(self, 0);
        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
