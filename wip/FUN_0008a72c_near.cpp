/* HSTMATCH module=EBOOT.BIN start=0x0008a72c end=0x0008ab10 symbol=FUN_0008a72c lang=c++ */

extern "C" {
extern unsigned char DAT_003386b8;
extern void *DAT_00338ac4;
extern void *DAT_00338b44;
extern void *DAT_00338c20;

int FUN_001181c0(int);
void *FUN_0022cd60(void);
int FUN_000def58(void *);
int FUN_000de208(void *, int);
int FUN_000de238(void *, int);
int FUN_000de268(void *, int);
int FUN_000de298(void *, int);
unsigned char FUN_002b79e8(void);
int FUN_000fb7b8(void *);
void FUN_00123470(void *, int);
}

extern "C" int FUN_0008a72c(char *param_1, int param_2)
{
    unsigned char s1;
    int bVar8;
    bool s3_b3;
    bool bVar4;
    bool s6_same;
    int bVar8_init;
    int a1_node;
    int s0_node;
    int a3_node;
    int val148;
    int val234;
    int *cd60;
    char *ac4;
    int idx;
    char *p8c;
    char *p80;
    unsigned char c4c;
    int mode;
    unsigned char bVar4_2;
    int r;

    if (DAT_003386b8 != 0) {
        if (*(int *)(param_1 + 0x6c) <= 0) {
            return 0;
        }
    }
    if (FUN_001181c0(*(int *)(param_1 + 0x9c)) >= 0) {
        s1 = *(unsigned char *)(param_1 + 0x59);
        bVar8_init = 0;
        if (s1 != 0 && *(int *)(param_1 + 0x3c) == 7) {
            bVar8_init = 1;
        }
        bVar4 = 1;
        if (s1 == 0) {
            if (!bVar8_init) {
                bVar4 = 0;
            }
        }

        bVar8 = 0;
        cd60 = (int *)FUN_0022cd60();
        if (cd60[1] == 0) {
            param_2 = 0;
        }

        if (!bVar4 && param_2 != 0) {
            a1_node = 0;
            if (DAT_003386b8 != 0) {
                ac4 = (char *)DAT_00338ac4;
                idx = *(int *)(ac4 + 0x198);
                if (*(unsigned char *)(ac4 + idx * 7 + 0x5e) != 0) {
                    int off = idx * 4;
                    off += (int)param_1;
                    a1_node = *(int *)(off + 0x70);
                }
            }
            if (a1_node == 0) {
                a1_node = *(int *)(param_1 + 0x70);
            }
            s0_node = *(int *)(a1_node + 0x144);

            val234 = *(int *)(*(int *)(param_1 + 0x88) + 0x234);

            a3_node = 0;
            if (DAT_003386b8 != 0) {
                ac4 = (char *)DAT_00338ac4;
                idx = *(int *)(ac4 + 0x198);
                if (*(unsigned char *)(ac4 + idx * 7 + 0x5e) != 0) {
                    int off = idx * 4;
                    off += (int)param_1;
                    a3_node = *(int *)(off + 0x70);
                }
            }
            if (a3_node == 0) {
                a3_node = *(int *)(param_1 + 0x70);
            }
            val148 = *(int *)(a3_node + 0x148);

            s6_same = (val234 == val148);

            s3_b3 = (FUN_000def58(DAT_00338b44) != 0 ||
                     *(int *)((char *)DAT_00338b44 + 0xa4) == s0_node ||
                     *(int *)((char *)DAT_00338b44 + 0xa0) == s0_node);

            cd60 = (int *)FUN_0022cd60();
            if (cd60[1] == 2) {
                s3_b3 = s3_b3 | (*(int *)((char *)DAT_00338b44 + 0x9c) == s0_node);
            }

            if (s3_b3 && (FUN_000def58(DAT_00338b44) != 0 ||
                          (s6_same && (FUN_000de208(DAT_00338b44, -1) != 0 ||
                                       FUN_000de238(DAT_00338b44, -1) != 0 ||
                                       FUN_000de268(DAT_00338b44, -1) != 0 ||
                                       FUN_000de298(DAT_00338b44, -1) != 0)))) {
                bVar8 = 1;
            }

            if ((FUN_002b79e8() & 0x10) != 0) {
                bVar8 = 1;
            }
        }

        if (bVar4 || bVar8) {
            bVar4_2 = 0;
            if (DAT_003386b8 != 0 && FUN_000fb7b8(DAT_00338c20) != 0) {
                bVar4_2 = 0;
            } else if (*(unsigned char *)(param_1 + 0x4c) == 0) {
                switch (*(int *)(param_1 + 0x3c)) {
                case 5:
                case 7:
                    bVar4_2 = (FUN_001181c0(*(int *)(param_1 + 0x9c)) >= 0);
                    break;
                case 6:
                    bVar4_2 = (FUN_001181c0(*(int *)(param_1 + 0x9c)) >= 0) &&
                              !FUN_000def58(DAT_00338b44);
                    break;
                }
            }

            if ((unsigned char)bVar4_2 != 0) {
                *(int *)(param_1 + 0x40) = 7;
                *(unsigned char *)(param_1 + 0x58) = bVar8;
                p8c = *(char **)(param_1 + 0x8c);
                p80 = *(char **)(p8c + 0x80);
                *(unsigned char *)(p80 + 0x7a) = (s1 != 0 || bVar8 != 0);
                p8c = *(char **)(param_1 + 0x8c);
                p80 = *(char **)(p8c + 0x80);
                *(unsigned char *)(p80 + 0x7b) = bVar8;
                p8c = *(char **)(param_1 + 0x8c);
                p80 = *(char **)(p8c + 0x80);
                FUN_00123470(p80, 1);
                return 1;
            }
        }

        p8c = *(char **)(param_1 + 0x8c);
        p80 = *(char **)(p8c + 0x80);
        FUN_00123470(p80, 0);
    }
    return 0;
}

// Differs: 3 words out of 249 words (246/249 match, 98.8% exact):
// Retail threads the branch around the switch directly to 0x8aa84 with 'move v0, zero'
// in the delay slot, which allows the second branch at 0x8aa00 to become 'bnezl v0, 0x8aa84'
// with hoisted 'andi v0, s0, 0xff' in its delay slot. Our code routes both exits to
// 0x8aa80 where 'andi v0, s0, 0xff' sits at the end of the switch block.
// Ruled out:
// - goto label right before test (collapses duplicate lbu load)
// - single compound boolean (A || B) && C (loses dual lbu branch structure)
// - separate temporary ok variable (adds register allocation frame overhead)
// near miss from lane dsn6 (unreviewed)
