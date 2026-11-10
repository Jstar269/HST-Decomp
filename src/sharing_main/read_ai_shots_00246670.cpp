/* HSTMATCH module=hstp_trans_main.prx start=0x00246670 end=0x002468b4 symbol=FUN_00246670 lang=c++ */
/* Clone of src/eboot/read_ai_shots_0023650c.cpp (hstclone.py tag twin11100306-hstp_trans_main.prx): addresses substituted; exact match verified; names and types await human review. */
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
    extern void *DAT_003b0388;
    extern char DAT_0039d364;
    extern const char DAT_0039db70[];
    extern const char DAT_0039db9c[];
    extern const char DAT_0039dbac[];
    extern const char DAT_0039dbc0[];
    extern const char DAT_0039dbd4[];
    extern const char DAT_0039dbec[];
    extern const char DAT_0039dbfc[];
    extern const char DAT_0039dc10[];

    void FUN_0008c8a8(ShotVector *v, unsigned int n, Tag tag);
    void FUN_00247494(ShotVector *v, unsigned int n);
    void FUN_00247450(ShotVector *v, const ShotRecord *val, Tag tag);
    void FUN_000114c0(void *dst, int val, unsigned int size);

    void FUN_00065884(void *csv);
    void FUN_000658cc(void *csv, int);
    void FUN_0008d97c(void *buf);
    void FUN_0006778c(void *buf, int);
    int FUN_00066260(void *csv, const char *path, int a, int b, const void *c);
    int FUN_00065fa4(void *csv);
    int FUN_00066700(void *csv, int row, const char *col);
}

#define PUSH_BACK(vec, val) do { \
    if ((vec).size < (vec).capacity) { \
        new (&(vec).data[(vec).size]) ShotRecord(val); \
        (vec).size++; \
    } else { \
        FUN_00247450(&(vec), &(val), Tag()); \
    } \
} while (0)

extern "C" void FUN_00246670(ShotVector *vec) {
    FUN_0008c8a8(vec, vec->size, Tag());

    char csv[48];
    ShotRecord rec;

    char *s0_ptr = (char *)(int)csv;

    FUN_00065884(s0_ptr);
    *(void **)csv = &DAT_003b0388;
    FUN_0008d97c(csv + 16);
    FUN_0008d97c((char *)s0_ptr + 32);

    int ok = FUN_00066260(s0_ptr, DAT_0039db70, 0, 1, &DAT_0039d364);
    if (ok != 0) {
        int count = FUN_00065fa4(s0_ptr);
        unsigned int n = count - 1;
        if (vec->capacity < n) {
            FUN_00247494(vec, n);
        }
        for (int i = 1; i < count; i++) {
            FUN_000114c0(&rec, 0, 28);
            rec.ai_no = FUN_00066700(csv, i, DAT_0039db9c);
            rec.form_a = FUN_00066700(csv, i, DAT_0039dbac);
            rec.form_b = FUN_00066700(csv, i, DAT_0039dbc0);
            rec.character = FUN_00066700(csv, i, DAT_0039dbd4);
            rec.aim = FUN_00066700(csv, i, DAT_0039dbec);
            rec.button = FUN_00066700(csv, i, DAT_0039dbfc);
            rec.freq = FUN_00066700(csv, i, DAT_0039dc10);
            PUSH_BACK(*vec, rec);
        }
    }

    *(void **)csv = &DAT_003b0388;
    FUN_0006778c(csv + 32, 0);
    FUN_0006778c(csv + 16, 0);
    FUN_000658cc(csv, 0);
}
