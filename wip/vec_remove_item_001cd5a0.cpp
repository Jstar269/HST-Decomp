/* HSTMATCH module=EBOOT.BIN start=0x001cd5a0 end=0x001cd6e8 symbol=FUN_001cd5a0 lang=c++ */

struct Item {
    virtual void _0();
    virtual void destroy(int flag);
};

struct It {
    Item **p;
    It() {}
    It(Item **x) : p(x) {}
    It(const It& x) : p(x.p) {}
    It& operator++() { ++p; return *this; }
    Item*& operator*() const { return *p; }
};

static inline int operator!=(It a, It b) {
    return (a.p == b.p) ^ 1;
}

static inline int operator==(It a, It b) {
    return a.p == b.p;
}

struct Tag {};

struct Self_001cd5a0 {
    char pad0[64];
    Item **v1_data;   // 64 (0x40)
    int v1_size;      // 68 (0x44)
    int v1_cap;       // 72 (0x48)
    Item **v2_data;   // 76 (0x4c)
    int v2_size;      // 80 (0x50)
    int v2_cap;       // 84 (0x54)

    It v1_begin() { return It(v1_data); }
    It v1_end() { return It(v1_data + v1_size); }
    It v2_begin() { return It(v2_data); }
    It v2_end() { return It(v2_data + v2_size); }
};

extern void *DAT_0034a2e0;

extern "C" {
void FUN_001cd970(It *out, Item **first, Item **last, Item **dest, Item **val);
void FUN_001cd8dc(void *vec, Item **first, Item **last, char tag);
void FUN_00225b6c(void *p, int zero);
}

extern "C" void FUN_001cd5a0(Self_001cd5a0 *self, Item *item) {
    if (item != 0) {
        It end = self->v1_end();
        It it = self->v1_begin();
        It end_copy = end;
        while (it == end ? false : true) {
            if (*it == item) break;
            ++it;
        }

        It res;
        It *pRes;
        if (it != end) {
            FUN_001cd970(&res, it.p + 1, end.p, it.p, &item);
            pRes = &res;
        } else {
            pRes = &it;
        }
        FUN_001cd8dc(&self->v1_data, pRes->p, end_copy.p, (char)0);

        It end2 = self->v2_end();
        It it2 = self->v2_begin();
        It end2_copy = end2;
        It it2_copy = it2;
        while (it2_copy == end2_copy ? false : true) {
            if (*it2_copy == item) break;
            ++it2_copy;
        }

        if (it2_copy != end2) {
            *it2_copy = 0;
        }

        if (item != 0) {
            if (item != 0) {
                item->destroy(1);
            }
            item = 0;
        }

        FUN_00225b6c(DAT_0034a2e0, 0);
    }
}
// Near miss: 79/82 words, 81 differ.
// Differs: Iterator/Range copy ABI through $f0/$f1 (sw end; lwc1; swc1 to sp+60) and stack slot allocation for it/end.
// Ruled out: modeling it/end as plain pointers drops the float copy completely; modeling as 4-byte It objects spills differently in caller-saved registers.
// near miss from lane dsn6 (unreviewed)
