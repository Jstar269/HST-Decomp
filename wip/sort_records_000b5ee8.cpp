/* HSTMATCH module=EBOOT.BIN start=0x000b5ee8 end=0x000b62d0 symbol=FUN_000b5ee8 lang=c++ */

struct Record {
    char pad[56];
    float key;
    char pad2[20];
};

struct Pair {
    void *first;
    void *second;
};

struct IterState1 { char data[8]; };
struct IterState2 { char data[8]; };
struct IterState3 { char data[8]; };

struct EndState1 { char data[8]; };
struct EndState2 { char data[8]; };
struct EndState3 { char data[8]; };

struct MergeState { char data[8]; };

extern "C" {
void FUN_000b62e4(void *, void *);
void *FUN_00002f60(int, void *);
void FUN_000b43e8(void *, void *);
int FUN_000b5d78(void *, void *);
void FUN_000b5ed4(void *, void *);
void *FUN_000b62d0(void *);
void *FUN_000b42ac(void *);
void FUN_000b5db8(void *, void *);
void FUN_000b5d90(void *);
void FUN_000b5da4(void *);
void FUN_000b5d60(void *, void *);
void FUN_000b4578(void *, void *, int);
void FUN_000b4710(void *, void *, void *, void *, int);
void FUN_000b657c(void *, void *, void *, void *, void *, void *, int);
}

static inline void run_case1(Record *first, Record *end, Pair pair) {
    Pair p = pair;
    Record *it_begin = first;
    Record *it_end = end;
    while (FUN_000b5d78(&it_begin, &it_end)) {
        IterState1 s;
        FUN_000b5ed4(&s, &p);
        void *v = FUN_000b62d0(&it_end);
        v = FUN_000b42ac(v);
        FUN_000b5db8(&s, v);
        FUN_000b5d90(&p);
    }
    EndState1 s2;
    FUN_000b5d60(&s2, &p);
}

static inline void run_case2(Record *first, Record *end, Pair pair) {
    Pair p = pair;
    Record *it_begin = first;
    Record *it_end = end;
    while (FUN_000b5d78(&it_begin, &it_end)) {
        IterState2 s;
        FUN_000b5ed4(&s, &p);
        void *v = FUN_000b42ac(&it_begin);
        FUN_000b5db8(&s, v);
        FUN_000b5da4(&it_begin);
        FUN_000b5d90(&p);
    }
    EndState2 s2;
    FUN_000b5d60(&s2, &p);
}

static inline void run_case3(Record *first, Record *end, Pair pair, char flag) {
    FUN_000b4578(first, end, (int)flag);
    Pair p = pair;
    Record *it_begin = first;
    Record *it_end = end;
    while (FUN_000b5d78(&it_begin, &it_end)) {
        IterState3 s;
        FUN_000b5ed4(&s, &p);
        void *v = FUN_000b42ac(&it_begin);
        FUN_000b5db8(&s, v);
        FUN_000b5da4(&it_begin);
        FUN_000b5d90(&p);
    }
    EndState3 s2;
    FUN_000b5d60(&s2, &p);
}

extern "C" void FUN_000b5ee8(Record *begin, Record *end, Pair pair, char flag)
{
    Record *first = begin;
    int count = (int)(end - begin);
    if (count <= 0) {
        return;
    }

    if (count == 1) {
        Pair p = pair;
        char *cur = (char *)p.first;
        char *cont = (char *)p.second;
        char *start = *(char **)(cont + 8);
        int idx = (int)(cur - start) / 80;
        int cap = *(int *)(cont + 4);
        if (idx < cap) {
            FUN_000b62e4(p.first, begin);
        } else {
            void *mem = FUN_00002f60(80, p.first);
            if (mem != 0) {
                FUN_000b43e8(mem, begin);
            }
            *(int *)(cont + 4) += 1;
        }
        return;
    }

    if (count == 2) {
        Record elem2 = *(end - 1);
        Record elem1 = *begin;
        if (elem2.key < elem1.key) {
            run_case1(first, end, pair);
        } else {
            run_case2(first, end, pair);
        }
        return;
    }

    if (count < 21) {
        run_case3(first, end, pair, flag);
        return;
    }

    int half = count - (count / 2);
    Record *mid = begin + half;
    Pair pair_mid = pair;
    pair_mid.first = (char *)pair.first + half * 80;
    FUN_000b4710(begin, mid, &pair, &pair_mid, (int)flag);

    Pair pair_mid2 = pair;
    pair_mid2.first = (char *)pair.first + half * 80;
    Pair pair_end = pair;
    pair_end.first = (char *)pair.first + count * 80;
    FUN_000b4710(mid, end, &pair_mid2, &pair_end, (int)flag);

    MergeState m;
    FUN_000b657c(&m, begin, mid, mid, end, &pair, (int)flag);
}

// Differs: 241 words out of 250 words (244/250 words emitted, frame size -336 vs retail -400):
// Inlined MSL/STL merge-sort / stable-sort algorithm on 80-byte records.
// Discovered:
// 1. Parameter 3 is an 8-byte Pair passed by value, copied via lwc1/swc1 due to MWCC struct ABI.
// 2. Elements are 80-byte structs with float key at offset 56 (elem2.key < elem1.key).
// 3. 4-case structure: count == 1 (insert/grow), count == 2 (2-element compare & insert),
//    count < 21 (insertion sort 0xb4578 & insert), count >= 21 (divide and conquer merge sort).
// 4. Retail's -400 frame includes uncoalesced template iterator stack slots (sp+220..292)
//    and phantom slots (sp+292..324) from template inlining levels.
// near miss from lane dsn6 (unreviewed)
