/* HSTMATCH module=EBOOT.BIN start=0x000d2004 end=0x000d25c8 symbol=FUN_000d2004 lang=c99 */
#pragma opt_strength_reduction off

typedef unsigned int uint;

extern char DAT_00348b04;
extern int FUN_0001342c();
extern void FUN_000d31d8(int);

typedef void (*dtor_fn)(void *, int);

static inline void cleanup_block(int self)
{
    void *a0 = *(void **)(self + 72);
    if (a0 != 0) {
        if (a0 != 0) {
            *(int *)(self + 72) = 0;
            (**(dtor_fn *)(*(int *)a0 + 8))(a0, 1);
            *(int *)(self + 72) = 0;
        }
    }
    a0 = *(void **)(self + 64);
    if (a0 != 0) {
        if (a0 != 0) {
            *(int *)(self + 64) = 0;
            (**(dtor_fn *)(*(int *)a0 + 8))(a0, 1);
            *(int *)(self + 64) = 0;
        }
    }
    a0 = *(void **)(self + 68);
    if (a0 != 0) {
        if (a0 != 0) {
            *(int *)(self + 68) = 0;
            (**(dtor_fn *)(*(int *)a0 + 8))(a0, 1);
            *(int *)(self + 68) = 0;
        }
    }
    FUN_000d31d8(self);
    if (*(int *)(self + 428) != 0) {
        int s1 = 0;
        do {
            int s2 = s1 * 4;
            void *item = *(void **)(*(int *)(self + 424) + s2);
            if (item != 0) {
                if (item != 0) {
                    (**(dtor_fn *)(*(int *)item + 8))(item, 1);
                }
                *(int *)(*(int *)(self + 424) + s2) = 0;
            }
            s1++;
        } while (s1 < *(uint *)(self + 428));
    }
    *(int *)(self + 428) -= *(int *)(self + 428);
    *(int *)(self + 412) = 0;

    if (*(int *)(self + 440) != 0) {
        int s1 = 0;
        do {
            int s2 = s1 * 4;
            void *item = *(void **)(*(int *)(self + 436) + s2);
            if (item != 0) {
                if (item != 0) {
                    (**(dtor_fn *)(*(int *)item + 8))(item, 1);
                }
                *(int *)(*(int *)(self + 436) + s2) = 0;
            }
            s1++;
        } while (s1 < *(uint *)(self + 440));
    }
    *(int *)(self + 440) -= *(int *)(self + 440);
    *(char *)(self + 448) = 0;

    a0 = *(void **)(self + 392);
    if (a0 != 0) {
        *(char *)(self + 408) = 0;
        if (a0 != 0) {
            *(int *)(self + 392) = 0;
            (**(dtor_fn *)(*(int *)a0 + 8))(a0, 1);
            *(int *)(self + 392) = 0;
            *(char *)(self + 408) = 0;
        }
    }
    DAT_00348b04 = 0;
}

int FUN_000d2004(int self, int action)
{
    if (action == 24) goto L_24;
    if (action == 13) goto L_13;
    if (action == 14) goto L_14_22;
    if (action == 22) goto L_14_22;
    if (action == 12) goto L_12;
    if (action == 11) goto L_11;
    if (action == 10) goto L_ret;
    if (action == 9) goto L_ret;
    if (action == 8) goto L_8;
    if (action == 1) goto L_ret;
    if (action == 0) goto L_ret;
    goto L_ret;

L_8:
    cleanup_block(self);
    goto L_ret;

L_12:
    cleanup_block(self);
    goto L_ret;

L_14_22:
    *(int *)(self + 460) = FUN_0001342c() % 3;
    cleanup_block(self);
    goto L_ret;

L_13:
    FUN_000d31d8(self);
    {
        void *a0 = *(void **)(self + 392);
        if (a0 != 0) {
            if (a0 != 0) {
                *(int *)(self + 392) = 0;
                (**(dtor_fn *)(*(int *)a0 + 8))(a0, 1);
                *(int *)(self + 392) = 0;
            }
        }
    }
    goto L_ret;

L_24:
    FUN_000d31d8(self);
    goto L_ret;

L_11:
    return 0;

L_ret:
    return 0;
}
// Near miss: 336 words differ out of 369 words.
// Dispatch order verified as if (action == N) goto L_N cascade matching retail branch order (24, 13, 14, 22, 12, 11, 10, 9, 8, 1, 0).
// Retail stack frame is -48 saving ra, s2, s1, s0 at 12, 8, 4, 0(sp) with 32 unreferenced bytes above, likely from inlined class container context.
// Inlined cleanup_block handles triple virtual dtor calls and two array dtor loops.
// near miss from lane dsn5 (unreviewed)
