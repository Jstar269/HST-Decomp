/* HSTMATCH module=EBOOT.BIN start=0x001af0d8 end=0x001af218 symbol=FUN_001af0d8 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */

extern int DAT_002e2138;
extern int DAT_002e213c;

extern "C" {
void FUN_00103628(void *p, int n);
void FUN_000ffb3c(void *p, int n);
void FUN_001ac970(void *p);
int FUN_0019d474(void);
int FUN_001a49dc(int a, int b);
}

static inline int Clamp(int v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

#pragma opt_strength_reduction off
extern "C" void FUN_001af0d8(int p)
{
    int *child = *(int **)(p + 4);
    FUN_00103628(*(void **)child, 1);
    FUN_000ffb3c(*(void **)((char *)child + 32), 0);
    FUN_00103628(**(void ***)(p + 8), 1);
    FUN_001ac970(*(void **)(p + 0xc));
    int *q = *(int **)(p + 0x10);
    FUN_00103628(*(void **)q, 1);
    int a = FUN_0019d474();
    int b = FUN_0019d474();
    int n = FUN_001a49dc(b, *(int *)(a + 0x2c4)) - 1;
    int min = Clamp(n, DAT_002e2138, DAT_002e213c);
    int *r = q + 7;
    int *node = *(int **)(q + 8);
    int i = 0;
    while (i < node[5]) {
        FUN_000ffb3c(*(void **)(node[4] + i * 4), i != min);
        ++i;
    }
    **(int **)(*(int *)(r[1] + 0x10) + min * 4) = r[2];
}
