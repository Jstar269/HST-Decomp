/* HSTMATCH module=EBOOT.BIN start=0x00058748 end=0x00058b78 symbol=FUN_00058748 lang=c++ data=0x002d0f48 */

typedef unsigned char u8;

struct Mgr {
    char pad[8];
    void *f8;
};

struct Object;

struct Entry {
    int f00;
    float **f04;
    u8 f08;
    char pad09[3];
    float f0c;
    float f10;
    float f14;
    float f18;
    int f1c;
    int f20;
    int f24;
    float f28;
    float f2c;
    float f30;

    inline void SetF20(Object *self, int val);
};

template <typename T>
static inline const T& Min(const T& a, const T& b) {
    return b < a ? b : a;
}

template <typename T>
static inline const T& Max(const T& a, const T& b) {
    return a < b ? b : a;
}

struct Object {
    char pad00[0x8c];
    Mgr *mgr;
    char pad90[0xec - 0x90];
    u8 flag_ec;
    char paded[3];
    Entry recs[2];
    int start_idx;
    int end_idx;
    void *f160;
    char pad164[8];
    void *f16c;
    char pad170[0x180 - 0x170];
    int f180;
    char pad184[0x224 - 0x184];
    int *model_data;
    int count_e4;
};

inline void Entry::SetF20(Object *self, int val) {
    if (f20 != val) {
        f20 = val;
        if (val < 0) {
            f24 = 0;
        } else if (val < self->count_e4) {
            f24 = self->model_data[val];
        }
        float zero1 = 0.0f;
        float zero2 = 0.0f;
        f28 = zero2;
        f2c = Min(zero1, zero2);
        f30 = 0.0f;
    }
}

extern "C" {
void FUN_000351c8(void *, void *);
int FUN_00057c60(Object *, int);
void FUN_0005691c(float **, void *, int, int, float, float, float);
int FUN_00057934(Object *, float, int, float, int);
float FUN_0000bc34(float);
void FUN_00058b78(Object *);
void FUN_0005922c(Object *);
}

extern "C" void FUN_00058748(Object *self, int idx, int flag, float value)
{
    int offset = idx * 52;
    Entry *entry = (Entry *)((char *)self + offset + 240);
    entry->f18 = value;

    int cond = self->f180 != 0;
    if (entry->f04 == 0 || flag == 0) {
        goto finish;
    }

    float limit = (*entry->f04)[2];
    if (entry->f08 != 0) {
        if (limit == 0.0f) {
            goto clamp_mode;
        }
        if (entry->f24 != 0) {
            goto clamp_mode;
        }
        if (entry->f18 >= limit) {
            do {
                entry->f18 -= limit - entry->f0c;
            } while (limit <= entry->f18);
        } else if (entry->f18 < entry->f0c) {
            do {
                entry->f18 += limit - entry->f0c;
            } while (entry->f18 < entry->f0c);
        }
    } else {
clamp_mode:
        if (!(entry->f18 > limit)) {
            goto clamp_lo;
        }
        if (entry->f24 != 0) {
            goto clamp_lo;
        }
        entry->f18 = limit;
        goto clamp_done;

clamp_lo:
        if (entry->f18 < 0.0f) {
            entry->f18 = 0.0f;
        }
    }

clamp_done:
    if (idx == self->start_idx) {
        if (self->f16c != 0) {
            FUN_000351c8(self->mgr->f8, self->f160);
        }
    }

    if (entry->f14 != entry->f18) {
        entry->f14 = entry->f18;
    } else if (self->f16c != 0) {
        entry->f14 = entry->f18;
    } else {
        goto check_end;
    }

    if (FUN_00057c60(self, idx) == 0) {
        FUN_0005691c(entry->f04, self->f160, 0x222, 0, entry->f18, 0.0f, 0.0f);
    } else {
        float diff = entry->f2c - entry->f28;
        float t;
        if (entry->f2c == entry->f28) {
            t = 1.0f;
        } else {
            t = (entry->f18 - (limit + entry->f28)) / diff;
            t = Min(t, 1.0f);
        }

        float rem = entry->f18 - (limit + entry->f30);
        if (t == 1.0f && rem >= 0.0f) {
            entry->SetF20(self, -1);

            int a0 = *(int *)((char *)self + offset + 272);
            float arr1[2];
            int arr2[2];
            for (int i = 0; i < 2; i++) {
                if (i == idx) {
                    arr2[i] = a0;
                    arr1[i] = rem;
                } else {
                    arr2[i] = 32767;
                    arr1[i] = 0.0f;
                }
            }
            FUN_00057934(self, arr1[0], arr2[0], arr1[1], arr2[1]);
            return;
        }

        rem = Max(rem, 0.0f);
        float c = FUN_0000bc34(t * 3.1415927f);
        t = c * -0.5f + 0.5f;
        FUN_0005691c(entry->f04, self->f160, 0x222, entry->f24, entry->f18, rem, t);
    }

check_end:
    if (idx == self->end_idx) {
        if (self->f16c != 0) {
            FUN_00058b78(self);
        }
    }
    cond = 1;

finish:
    if (cond && idx == self->end_idx) {
        FUN_0005922c(self);
    }
}
// Differs: 182 words differ; s1/s0 register allocation around entry indexing and branch structure around entry->f14 update vs FUN_00057c60.
// Ruled out: flat vs nested if/else for clamp and check_end branches; both leave minor delay-slot scheduling differences.
// near miss from lane dsn7 (unreviewed)
