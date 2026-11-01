/* HSTMATCH module=EBOOT.BIN start=0x00140d58 end=0x001414e4 symbol=FUN_00140d58 lang=c++ */
#pragma opt_strength_reduction off

struct Tag {};

struct BaseObj {
    virtual ~BaseObj();
};

extern "C" {
void FUN_00053718(void *, int);
void FUN_0005d818(void *, int);
void FUN_00055e48(void *, int);
void FUN_00056cb0(void *, int);
void FUN_00000a00(void *);
void FUN_00146c4c(void *, int, Tag);
void FUN_00146ee0(void *, int, Tag);
void FUN_001472b8(void *, int, Tag);
void FUN_00146acc(void *, int, Tag);

void FUN_00140d58(char *self);
}

void FUN_00140d58(char *self)
{
    // Part 1: 6 arrays of objects at 0x40
    char *s2 = self;
    for (unsigned int s4 = 0; s4 < 6; s4++, s2 += 12) {
        if (*(int *)(s2 + 68) != 0) {
            unsigned int s3 = 0;
            int s1 = 0;
            do {
                BaseObj *a0 = *(BaseObj **)(*(int *)(s2 + 64) + s1);
                if (a0 != 0) {
                    delete a0;
                    *(int *)(*(int *)(s2 + 64) + s1) = 0;
                }
                s3++;
                s1 += 4;
            } while (s3 < *(unsigned int *)(s2 + 68));
        }
        int v1 = *(int *)(s2 + 68);
        *(int *)(s2 + 68) = v1 - v1;
    }

    // Part 2: single vector at 0x28
    if (*(int *)(self + 44) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            BaseObj *a0 = *(BaseObj **)(*(int *)(self + 40) + s1);
            if (a0 != 0) {
                delete a0;
                *(int *)(*(int *)(self + 40) + s1) = 0;
            }
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 44));
    }
    int v0_2 = *(int *)(self + 44);
    *(int *)(self + 44) = v0_2 - v0_2;

    // Part 3: single vector at 0x10
    if (*(int *)(self + 20) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            void *a0 = *(void **)(*(int *)(self + 16) + s1);
            if (a0 != 0) {
                FUN_00053718(a0, 1);
                *(int *)(*(int *)(self + 16) + s1) = 0;
            }
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 20));
    }
    int v0_3 = *(int *)(self + 20);
    *(int *)(self + 20) = v0_3 - v0_3;

    // Part 4: 6 arrays of objects at 0xd0
    s2 = self;
    for (unsigned int s4 = 0; s4 < 6; s4++, s2 += 12) {
        if (*(int *)(s2 + 212) != 0) {
            unsigned int s3 = 0;
            int s1 = 0;
            do {
                BaseObj *a0 = *(BaseObj **)(*(int *)(s2 + 208) + s1);
                if (a0 != 0) {
                    delete a0;
                    *(int *)(*(int *)(s2 + 208) + s1) = 0;
                }
                s3++;
                s1 += 4;
            } while (s3 < *(unsigned int *)(s2 + 212));
        }
        int v1 = *(int *)(s2 + 212);
        *(int *)(s2 + 212) = v1 - v1;
    }

    // Part 5: single vector at 0x1c
    if (*(int *)(self + 32) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            void *a0 = *(void **)(*(int *)(self + 28) + s1);
            if (a0 != 0) {
                FUN_00053718(a0, 1);
                *(int *)(*(int *)(self + 28) + s1) = 0;
            }
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 32));
    }
    int v0_5 = *(int *)(self + 32);
    *(int *)(self + 32) = v0_5 - v0_5;

    // Part 6: vector at 0x124
    if (*(int *)(self + 296) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            char *s3 = *(char **)(*(int *)(self + 292) + s1);
            if (*(void **)(s3 + 1044) != 0) {
                FUN_0005d818(*(void **)(s3 + 1044), 1);
                *(void **)(s3 + 1044) = 0;
            }
            if (s3 != 0) {
                if (s3 != 0) {
                    FUN_00055e48(s3 + 1028, -1);
                    FUN_00055e48(s3 + 1012, -1);
                    FUN_00055e48(s3 + 996, -1);
                    FUN_00055e48(s3 + 896, -1);
                    FUN_00056cb0(s3 + 464, -1);
                    FUN_00056cb0(s3 + 32, -1);
                    FUN_00055e48(s3 + 8, -1);
                    FUN_00000a00(s3);
                }
            }
            *(int *)(*(int *)(self + 292) + s1) = 0;
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 296));
    }
    int v0_6 = *(int *)(self + 296);
    *(int *)(self + 296) = v0_6 - v0_6;
    *(int *)(self + 308) = 0;

    // Part 7: vector at 0x13c
    if (*(int *)(self + 320) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            char *s3 = *(char **)(*(int *)(self + 316) + s1);
            if (s3 != 0) {
                if (s3 != 0) {
                    FUN_00055e48(s3 + 1028, -1);
                    FUN_00055e48(s3 + 1012, -1);
                    FUN_00055e48(s3 + 996, -1);
                    FUN_00055e48(s3 + 896, -1);
                    FUN_00056cb0(s3 + 464, -1);
                    FUN_00056cb0(s3 + 32, -1);
                    FUN_00055e48(s3 + 8, -1);
                    FUN_00000a00(s3);
                }
            }
            *(int *)(*(int *)(self + 316) + s1) = 0;
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 320));
    }
    int v0_7 = *(int *)(self + 320);
    *(int *)(self + 320) = v0_7 - v0_7;

    // Part 8: vector at 0x148
    if (*(int *)(self + 332) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            char *s3 = *(char **)(*(int *)(self + 328) + s1);
            if (s3 != 0) {
                if (s3 != 0) {
                    FUN_00055e48(s3 + 1028, -1);
                    FUN_00055e48(s3 + 1012, -1);
                    FUN_00055e48(s3 + 996, -1);
                    FUN_00055e48(s3 + 896, -1);
                    FUN_00056cb0(s3 + 464, -1);
                    FUN_00056cb0(s3 + 32, -1);
                    FUN_00055e48(s3 + 8, -1);
                    FUN_00000a00(s3);
                }
            }
            *(int *)(*(int *)(self + 328) + s1) = 0;
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 332));
    }
    int v0_8 = *(int *)(self + 332);
    *(int *)(self + 332) = v0_8 - v0_8;

    // Part 9: vector at 0x118
    if (*(int *)(self + 284) != 0) {
        unsigned int s2_idx = 0;
        int s1 = 0;
        do {
            char *s3 = *(char **)(*(int *)(self + 280) + s1);
            if (s3 != 0) {
                if (s3 != 0) {
                    FUN_00055e48(s3 + 1028, -1);
                    FUN_00055e48(s3 + 1012, -1);
                    FUN_00055e48(s3 + 996, -1);
                    FUN_00055e48(s3 + 896, -1);
                    FUN_00056cb0(s3 + 464, -1);
                    FUN_00056cb0(s3 + 32, -1);
                    FUN_00055e48(s3 + 8, -1);
                    FUN_00000a00(s3);
                }
            }
            *(int *)(*(int *)(self + 280) + s1) = 0;
            s2_idx++;
            s1 += 4;
        } while (s2_idx < *(unsigned int *)(self + 284));
    }
    int v0_9 = *(int *)(self + 284);
    *(int *)(self + 284) = v0_9 - v0_9;

    // Part 10: 0x7bc groups
    if (*(int *)(self + 1984) != 0) {
        unsigned int s4 = 0;
        int s2_off = 0;
        do {
            char *v1 = *(char **)(self + 1980) + s2_off;
            if (*(int *)(v1 + 4) != 0) {
                unsigned int s3 = 0;
                int s1 = 0;
                do {
                    void *a0 = *(void **)(*(int *)v1 + s1);
                    if (a0 != 0) {
                        FUN_00000a00(a0);
                    }
                    *(int *)(*(int *)(*(int *)(self + 1980) + s2_off) + s1) = 0;
                    s3++;
                    v1 = *(char **)(self + 1980) + s2_off;
                    s1 += 4;
                } while (s3 < *(unsigned int *)(v1 + 4));
            }
            int v0_10 = *(int *)(v1 + 4);
            *(int *)(v1 + 4) = v0_10 - v0_10;
            s4++;
            s2_off += 12;
        } while (s4 < *(unsigned int *)(self + 1984));
    }
    FUN_00146c4c(self + 1980, *(int *)(self + 1984), Tag());

    // Part 11: vector at 0x154
    if (*(int *)(self + 344) != 0) {
        unsigned int s1 = 0;
        do {
            int s2_off = s1 * 4;
            char *s3 = *(char **)(*(int *)(self + 340) + s2_off);
            if (s3 != 0) {
                if (s3 != 0) {
                    FUN_00055e48(s3 + 64, -1);
                    FUN_00055e48(s3 + 48, -1);
                    FUN_00055e48(s3 + 32, -1);
                    FUN_00055e48(s3 + 8, -1);
                    FUN_00000a00(s3);
                }
            }
            *(int *)(*(int *)(self + 340) + s2_off) = 0;
            s1++;
        } while (s1 < *(unsigned int *)(self + 344));
    }
    int v0_11 = *(int *)(self + 344);
    *(int *)(self + 344) = v0_11 - v0_11;

    // Part 12: vector at 0x34
    if (*(int *)(self + 56) != 0) {
        unsigned int s1 = 0;
        do {
            int s2_off = s1 * 4;
            void *a0 = *(void **)(*(int *)(self + 52) + s2_off);
            if (a0 != 0) {
                FUN_0005d818(a0, 1);
            }
            *(int *)(*(int *)(self + 52) + s2_off) = 0;
            s1++;
        } while (s1 < *(unsigned int *)(self + 56));
    }
    int v0_12 = *(int *)(self + 56);
    *(int *)(self + 56) = v0_12 - v0_12;

    // Part 13
    BaseObj *f_4 = *(BaseObj **)(self + 4);
    if (f_4 != 0) {
        *(int *)(self + 4) = 0;
        delete f_4;
        *(int *)(self + 4) = 0;
    }
    BaseObj *f_c = *(BaseObj **)(self + 12);
    if (f_c != 0) {
        *(int *)(self + 12) = 0;
        delete f_c;
        *(int *)(self + 12) = 0;
    }
    *(char *)(self + 3356) = 0;
    *(char *)(self + 3357) = 0;
    *(char *)(self + 1952) = 0;

    // Part 14
    FUN_00146ee0(self + 2248, *(int *)(self + 2252), Tag());
    FUN_001472b8(self + 2260, *(int *)(self + 2264), Tag());
    FUN_00146acc(self + 1956, *(int *)(self + 1960), Tag());
}
// NEAR MISS: 464/483 words, 450 differ.
// What differs:
// 1. Frame size: -80 vs retail -112 (needs 32 additional frame bytes from inlined caller/callee or escaping temporary)
// 2. Loop registers: s4/s2 vs s2/s3 for array-stepping loops
// 3. Register colouring across 14 container cleanup stages in XformHost::Clear
// near miss from lane dsn5 (unreviewed)
