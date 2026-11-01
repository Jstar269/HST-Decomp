/* HSTMATCH module=EBOOT.BIN start=0x00266fd4 end=0x00267bd8 symbol=FUN_00266fd4 lang=c99 */

typedef unsigned char undefined1;
typedef unsigned int uint;

struct SubItem {
    char pad0[0xc0];
    unsigned char flag_c0;
    char padc1[3];
    float val_c4;
    char padc8[8];
    float val_d0;
};

struct Vec4 {
    float x, y, z, w;
};

struct Item {
    char pad0[0x10];
    struct Vec4 pos;
    struct SubItem *sub;
};

struct Context {
    char pad0[0x3c];
    int state;
    char pad40[8];
    int counter;
    char flag4c;
    char pad4d[3];
    struct Item **items;
    int count;
    char pad58[12];
    char flag64;
    char flag65;
};

extern int DAT_002fdefc;
extern int DAT_0034179c;
extern int DAT_003313c4;
extern int DAT_00338c20;
extern int *DAT_003402a0;

extern int FUN_001b1f7c(int, int);
extern int FUN_001b1f84(int, int*);
extern int FUN_001b2bb8(int, int);
extern void FUN_0005f030(struct SubItem *, int, ...);
extern void FUN_0005f110(int, struct SubItem *);
extern void FUN_0005f0d8(struct SubItem *);
extern void FUN_000fb4d0(int, int, void*);
extern double FUN_0000b16c(double);
extern void FUN_00267f24(struct Context *);
extern void FUN_00268ca0(void);

void FUN_00266fd4(struct Context *ctx)
{
    switch (ctx->state) {
    case 0:
        FUN_00268ca0();
        break;
    case 1: {
        int v = FUN_001b1f7c((int)&DAT_0034179c, DAT_002fdefc);
        if (v != 0) {
            FUN_001b1f84((int)&DAT_0034179c, &DAT_002fdefc);
        }
        int count = ctx->count;
        if (count != 0) {
            struct Item **items = ctx->items;
            for (uint i = 0; i < (uint)count; i++) {
                struct Item *item = items[i];
                struct SubItem *sub = item->sub;
                if (sub->val_d0 == 0.0f) {
                    FUN_0005f030(sub, 0);
                    sub->val_c4 = 1.0f;
                    sub->flag_c0 = 1;
                    sub->val_d0 = sub->val_c4;
                }
                int g = *(int *)&DAT_003313c4;
                float f8 = (float)FUN_0000b16c((double)item->pos.z);
                float f9 = (float)FUN_0000b16c((double)*(float *)(g + 0x1d8));
                if (f8 < f9) {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) > f && f > 0.0f) {
                        if (ctx->flag65 == 0) {
                            FUN_0005f030(0, (int)item->sub, 1);
                        } else {
                            FUN_0005f030(0, (int)item->sub, 0);
                        }
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                } else {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) < f && f < 0.0f) {
                        if (ctx->flag65 == 0) {
                            FUN_0005f030(0, (int)item->sub, 1);
                        } else {
                            FUN_0005f030(0, (int)item->sub, 0);
                        }
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                }
                FUN_0005f0d8(item->sub);
            }
        }
        FUN_00267f24(ctx);
        break;
    }
    case 2: {
        int v = FUN_001b1f7c((int)&DAT_0034179c, DAT_002fdefc);
        if (v == 0) {
            if (ctx->flag4c != 0) {
                DAT_002fdefc = FUN_001b2bb8((int)&DAT_0034179c, 0xbd);
            }
        }
        if (ctx->flag4c == 0) {
            FUN_001b1f84((int)&DAT_0034179c, &DAT_002fdefc);
        }
        int count = ctx->count;
        if (count != 0) {
            struct Item **items = ctx->items;
            for (uint i = 0; i < (uint)count; i++) {
                struct Item *item = items[i];
                struct SubItem *sub = item->sub;
                if (sub->val_d0 == 0.0f) {
                    uint rng = *(uint *)((char *)DAT_003402a0 + 0x1360) * 0x343fd + 0x269ec3;
                    *(uint *)((char *)DAT_003402a0 + 0x1360) = rng;
                    if (((rng & 0x7fffffff) >> 16) % 100 > 4) {
                        if (sub->val_d0 == 0.0f) {
                            FUN_0005f030(sub, 0);
                            sub->val_c4 = 1.0f;
                            sub->flag_c0 = 1;
                            sub->val_d0 = sub->val_c4;
                            FUN_0005f110(0, sub);
                            sub->val_d0 = 0.0f;
                        }
                    } else {
                        FUN_0005f030(sub, 3);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        struct Vec4 loc = item->pos;
                        loc.y -= 1.2f;
                        if (ctx->flag4c != 0 && ctx->flag64 == 0) {
                            FUN_000fb4d0(*(int *)&DAT_00338c20, 0, &loc);
                        }
                    }
                } else {
                    if (sub->val_d0 == 0.0f) {
                        FUN_0005f030(sub, 0);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                }
                int g = *(int *)&DAT_003313c4;
                float f8 = (float)FUN_0000b16c((double)item->pos.z);
                float f9 = (float)FUN_0000b16c((double)*(float *)(g + 0x1d8));
                if (f8 < f9) {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) > f && f > 0.0f) {
                        if (ctx->flag65 == 0) {
                            FUN_0005f030(0, (int)item->sub, 1);
                        } else {
                            FUN_0005f030(0, (int)item->sub, 3);
                        }
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                } else {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) < f && f < 0.0f) {
                        if (ctx->flag65 == 0) {
                            FUN_0005f030(0, (int)item->sub, 1);
                        } else {
                            FUN_0005f030(0, (int)item->sub, 3);
                        }
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                }
                FUN_0005f0d8(item->sub);
            }
        }
        FUN_00267f24(ctx);
        break;
    }
    case 3: {
        int v = FUN_001b1f7c((int)&DAT_0034179c, DAT_002fdefc);
        if (v != 0) {
            FUN_001b1f84((int)&DAT_0034179c, &DAT_002fdefc);
        }
        int count = ctx->count;
        if (count != 0) {
            struct Item **items = ctx->items;
            for (uint i = 0; i < (uint)count; i++) {
                struct Item *item = items[i];
                struct SubItem *sub = item->sub;
                if (sub->val_d0 == 0.0f) {
                    uint rng = *(uint *)((char *)DAT_003402a0 + 0x1360) * 0x343fd + 0x269ec3;
                    *(uint *)((char *)DAT_003402a0 + 0x1360) = rng;
                    if (((rng & 0x7fffffff) >> 16) % 100 > 4 || (ctx->count >> 1) <= ctx->counter) {
                        if (sub->val_d0 == 0.0f) {
                            FUN_0005f030(sub, 0);
                            sub->val_c4 = 1.0f;
                            sub->flag_c0 = 1;
                            sub->val_d0 = sub->val_c4;
                            FUN_0005f110(0, sub);
                            sub->val_d0 = 0.0f;
                        }
                    } else {
                        FUN_0005f030(sub, 2);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        struct Vec4 loc = item->pos;
                        loc.y -= 1.2f;
                        if (ctx->flag4c != 0) {
                            if (ctx->flag64 == 0) {
                                FUN_000fb4d0(*(int *)&DAT_00338c20, 3, &loc);
                            }
                        }
                        ctx->counter++;
                    }
                } else {
                    if (sub->val_d0 == 0.0f) {
                        FUN_0005f030(sub, 0);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                }
                int g = *(int *)&DAT_003313c4;
                float f8 = (float)FUN_0000b16c((double)item->pos.z);
                float f9 = (float)FUN_0000b16c((double)*(float *)(g + 0x1d8));
                if (f8 < f9) {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) > f && f > 0.0f) {
                        FUN_0005f030(0, (int)item->sub, 1);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                } else {
                    float f = item->pos.z;
                    if (*(float *)(g + 0x1d8) < f && f < 0.0f) {
                        FUN_0005f030(0, (int)item->sub, 1);
                        sub->val_c4 = 1.0f;
                        sub->flag_c0 = 1;
                        sub->val_d0 = sub->val_c4;
                        FUN_0005f110(0, sub);
                        sub->val_d0 = 0.0f;
                    }
                }
                FUN_0005f0d8(item->sub);
            }
        }
        FUN_00267f24(ctx);
        break;
    }
    }
}
// Differs: 762 words differ; large 3076-byte switch statement with 4 cases, each containing complex nested loops and floating point comparisons.
// Ruled out: direct array indexing vs cached local pointers; both cause register spill/allocation differences in inner loop blocks.
// near miss from lane dsn7 (unreviewed)
