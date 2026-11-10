/* HSTMATCH module=EBOOT.BIN start=0x0023650c end=0x00236750 symbol=FUN_0023650c lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode corrected by hand: pointer to the csv buffer cast through int so MWCC keeps the base register for the +32 ctor (retail addiu a0,s0,32); exact match verified; names and types await human review. */
#pragma opt_strength_reduction off

struct Tag {};

struct ShotRecord {
    int ai_no;
    int form_a;
    int form_b;
    int character;
    int aim;
    int button;
    int freq;

    ShotRecord() {}
    ShotRecord(const ShotRecord &o)
        : ai_no(o.ai_no), form_a(o.form_a), form_b(o.form_b),
          character(o.character), aim(o.aim), button(o.button), freq(o.freq) {}
};

inline void *operator new(unsigned int, void *p) {
    return p;
}

struct ShotVector {
    ShotRecord *data;
    unsigned size;
    unsigned capacity;
};

extern "C" {
    extern void *DAT_003070bc;
    extern char DAT_002f4a64;
    extern const char DAT_002f5248[];
    extern const char DAT_002f5274[];
    extern const char DAT_002f5284[];
    extern const char DAT_002f5298[];
    extern const char DAT_002f52ac[];
    extern const char DAT_002f52c4[];
    extern const char DAT_002f52d4[];
    extern const char DAT_002f52e8[];

    void FUN_0008b1b8(ShotVector *v, unsigned int n, Tag tag);
    void FUN_00237330(ShotVector *v, unsigned int n);
    void FUN_002372ec(ShotVector *v, const ShotRecord *val, Tag tag);
    void FUN_000114c0(void *dst, int val, unsigned int size);

    void FUN_000649c0(void *csv);
    void FUN_00064a08(void *csv, int);
    void FUN_0008c230(void *buf);
    void FUN_00066338(void *buf, int);
    int FUN_000651b8(void *csv, const char *path, int a, int b, const void *c);
    int FUN_000650e0(void *csv);
    int FUN_00065554(void *csv, int row, const char *col);
}

#define PUSH_BACK(vec, val) do { \
    if ((vec).size < (vec).capacity) { \
        new (&(vec).data[(vec).size]) ShotRecord(val); \
        (vec).size++; \
    } else { \
        FUN_002372ec(&(vec), &(val), Tag()); \
    } \
} while (0)

extern "C" void FUN_0023650c(ShotVector *vec) {
    FUN_0008b1b8(vec, vec->size, Tag());

    char csv[48];
    ShotRecord rec;

    char *s0_ptr = (char *)(int)csv;

    FUN_000649c0(s0_ptr);
    *(void **)csv = &DAT_003070bc;
    FUN_0008c230(csv + 16);
    FUN_0008c230((char *)s0_ptr + 32);

    int ok = FUN_000651b8(s0_ptr, DAT_002f5248, 0, 1, &DAT_002f4a64);
    if (ok != 0) {
        int count = FUN_000650e0(s0_ptr);
        unsigned int n = count - 1;
        if (vec->capacity < n) {
            FUN_00237330(vec, n);
        }
        for (int i = 1; i < count; i++) {
            FUN_000114c0(&rec, 0, 28);
            rec.ai_no = FUN_00065554(csv, i, DAT_002f5274);
            rec.form_a = FUN_00065554(csv, i, DAT_002f5284);
            rec.form_b = FUN_00065554(csv, i, DAT_002f5298);
            rec.character = FUN_00065554(csv, i, DAT_002f52ac);
            rec.aim = FUN_00065554(csv, i, DAT_002f52c4);
            rec.button = FUN_00065554(csv, i, DAT_002f52d4);
            rec.freq = FUN_00065554(csv, i, DAT_002f52e8);
            PUSH_BACK(*vec, rec);
        }
    }

    *(void **)csv = &DAT_003070bc;
    FUN_00066338(csv + 32, 0);
    FUN_00066338(csv + 16, 0);
    FUN_00064a08(csv, 0);
}
