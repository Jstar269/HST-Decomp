/* HSTMATCH module=hstp_trans_main.prx start=0x001bc7b0 end=0x001bc8f0 symbol=FUN_001bc7b0 lang=c++ */
/* Clone of src/eboot/clamp_node_001af0d8.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */

extern int DAT_00389a68;
extern int DAT_00389a6c;

extern "C" {
void FUN_00108f54(void *p, int n);
void FUN_00105470(void *p, int n);
void FUN_001b9fd8(void *p);
int FUN_001aa91c(void);
int FUN_001b1fa8(int a, int b);
}

static inline int Clamp(int v, const int &lo, const int &hi)
{
    return v < lo ? lo : (hi < v ? hi : v);
}

#pragma opt_strength_reduction off
extern "C" void FUN_001bc7b0(int p)
{
    int *child = *(int **)(p + 4);
    FUN_00108f54(*(void **)child, 1);
    FUN_00105470(*(void **)((char *)child + 32), 0);
    FUN_00108f54(**(void ***)(p + 8), 1);
    FUN_001b9fd8(*(void **)(p + 0xc));
    int *q = *(int **)(p + 0x10);
    FUN_00108f54(*(void **)q, 1);
    int a = FUN_001aa91c();
    int b = FUN_001aa91c();
    int n = FUN_001b1fa8(b, *(int *)(a + 0x2c4)) - 1;
    int min = Clamp(n, DAT_00389a68, DAT_00389a6c);
    int *r = q + 7;
    int *node = *(int **)(q + 8);
    int i = 0;
    while (i < node[5]) {
        FUN_00105470(*(void **)(node[4] + i * 4), i != min);
        ++i;
    }
    **(int **)(*(int *)(r[1] + 0x10) + min * 4) = r[2];
}
