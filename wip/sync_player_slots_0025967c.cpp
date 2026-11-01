/* HSTMATCH module=EBOOT.BIN start=0x0025967c end=0x00259a1c symbol=FUN_0025967c lang=c++ */
#include "hst/types.h"

struct Tail {
    unsigned char b0;       /* 0xc8 = 200 */
    unsigned char b1;       /* 0xc9 = 201 */
    unsigned char b2;       /* 0xca = 202 */
    unsigned char b3;       /* 0xcb = 203 */
    char str32[32];         /* 0xcc = 204 */
    unsigned char b4;       /* 0xec = 236 */
    unsigned char b5;       /* 0xed = 237 */
    unsigned char b6;       /* 0xee = 238 */
    unsigned char b7;       /* 0xef = 239 */
    unsigned char b8;       /* 0xf0 = 240 */
    int w0;                 /* 0xf4 = 244 */
    int w1;                 /* 0xf8 = 248 */
    float f0;               /* 0xfc = 252 */
    unsigned char b9;       /* 0x100 = 256 */
    unsigned char b10;      /* 0x101 = 257 */
    int w2;                 /* 0x104 = 260 */
    char str16[16];         /* 0x108 = 264 */
    float f1;               /* 0x118 = 280 */
    float f2;               /* 0x11c = 284 */
};

struct TableEntry {
    char subA[200];
    Tail tail;
};

extern "C" {
extern char DAT_003417b8;
void *memcpy(void *dest, const void *src, unsigned int n);
void FUN_002543a4(void *);
void FUN_0025a548(void *, int *);
void FUN_00254bd0(void *, int);
void FUN_0027fc70(void *, void *, int);
void FUN_00259a1c(void *, const void *);
}

extern "C" void FUN_0025967c(char *self)
{
    FUN_002543a4(self + 0x60);

    bool flag = true;
    int i = 0;
    int off = 0;
    char *s2 = self;
    char *table = &DAT_003417b8;
    char *s6 = self;

    while (i < (*(unsigned char *)(self + 0x644) ? 2 : 4)) {
        unsigned char *a0 = *(unsigned char **)(self + 0x640);
        if (a0[1] == 0) {
            goto do_compare;
        }
        if (*(int *)(a0 + 0x2c0c) <= 0) {
            flag = false;
            goto next_iter;
        }
        if (i == 1) {
            off += 1112;
            goto next_iter_skip_off;
        }

    do_compare:
        a0 += off;
        {
            bool match = (*(short *)(s2 + 1840) == *(short *)(a0 + 6880) &&
                          *(short *)(s2 + 1842) == *(short *)(a0 + 6882) &&
                          *(short *)(s2 + 1844) == *(short *)(a0 + 6884) &&
                          *(short *)(s2 + 1846) == *(short *)(a0 + 6886) &&
                          *(short *)(s2 + 1848) == *(short *)(a0 + 6888) &&
                          *(short *)(s2 + 1850) == *(short *)(a0 + 6890) &&
                          *(unsigned char *)(s2 + 1852) == *(unsigned char *)(a0 + 6892) &&
                          *(unsigned char *)(s2 + 1853) == *(unsigned char *)(a0 + 6893) &&
                          *(unsigned char *)(s2 + 1942) == *(unsigned char *)(a0 + 6982));

            if (!match && *(unsigned char *)(a0 + 7118) != 0) {
                if (*(unsigned char *)(a0 + 7137) != 0) {
                    goto next_iter;
                }
                memcpy(s2 + 1840, a0 + 6880, 200);

                TableEntry *s0 = (TableEntry *)(*(char **)(self + 1600) + off + 6880);
                FUN_00259a1c(table + 24, s0);
                ((TableEntry *)(table + 24))->tail = s0->tail;

                int local = i;
                FUN_0025a548(self + 0x24c, &local);
                FUN_00254bd0(self + 0x60, local);
            }

            if (*(unsigned char *)(self + i + 100) == 0 || *(int *)(s6 + 0x23c) != 0) {
                flag = false;
            }
        }

    next_iter:
        off += 1112;
    next_iter_skip_off:
        s2 += 1112;
        table += 288;
        s6 += 4;
        i++;
    }

    char *p640 = *(char **)(self + 0x640);
    if (flag) {
        *(unsigned char *)(p640 + 0x617) = 1;
    }
    if (p640[1] != '\0') {
        if (flag) {
            *(unsigned char *)(p640 + 0x617) = (*(int *)(self + 0x260) == 0);
        } else {
            *(unsigned char *)(p640 + 0x617) = 0;
        }
    }

    for (int j = 0; j < (*(unsigned char *)(self + 0x644) ? 2 : 4); j++) {
        FUN_0027fc70(self + 0x6e8, *(void **)(self + 0x284 + j * 0xa4), j == 0);
    }
}
// NEAR MISS: 223/232 words, 183 differ.
// What differs:
// 1. MWCC hoists v1 = a0 + 6816 (offset to SeqInfo within a0) into delay slot of the first test
// 2. Short-circuit comparison chain register allocation (a1 vs a0)
// 3. String-copy loop register allocation for char str32[32] and char str16[16]
// 4. Branch-likely beql delay slot schedule at loop preheader.
// near miss from lane dsn5 (unreviewed)
