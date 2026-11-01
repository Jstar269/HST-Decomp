/* HSTMATCH module=EBOOT.BIN start=0x00182ea4 end=0x001833f4 symbol=FUN_00182ea4 lang=c++ */

extern "C" {
extern void *DAT_002e0974;
extern void *DAT_002e0978;
extern void *DAT_0034a77c;
extern char DAT_00341524;

int FUN_0016bc8c(void);
void *FUN_0016bc70(void);
int FUN_0016bb88(void);
int FUN_00193474(int);
int FUN_0016bb98(int, int);
void FUN_000ffb3c(void *, int);
int FUN_001fea10(int, int, int);
void FUN_0010d4a8(void *);
int FUN_00216e00(void *);
int FUN_0024dd40(void *, float);
void FUN_000143b0(void *, void *, int);
}

struct VObj {
    virtual void _0();
    virtual void method3(int);
    virtual void _2();
    virtual void _3();
    virtual void method6(int);
};

struct Node : VObj {
    char pad0[0xf4];
    void *data;     /* 0xf8 */
    char pad1[0x3c];
    int flag;       /* 0x138 */
};

struct Item {
    char pad[0x24];
    int val24;      /* 0x24 */
    int val28;      /* 0x28 */
};

extern "C" void FUN_00182ea4(char *param_1, int param_2)
{
    int iVar2 = FUN_0016bc8c();
    char *ctx = (char *)FUN_0016bc70();
    int *a0 = *(int **)(ctx + 0x538);
    Node *s3 = *(Node **)(param_1 + 0x1124);
    Node *s2 = *(Node **)(param_1 + 0x113c);
    Item **piVar4 = (Item **)(param_1 + 0xcd8);

    int a1 = a0[2];
    int v1 = a0[6];
    int v0 = a0[1];

    Node *s1 = *(Node **)(param_1 + 0x1130);
    Node *s6 = *(Node **)(param_1 + 0x1148);

    unsigned char *entry = *(unsigned char **)(*(int *)(v1 + a1 * 12) + v0 * 4);

    char *s7 = param_1 + 0x1114;

    Item **piVar5 = (Item **)(param_1 + 0xcec);
    void **puVar6 = (void **)(param_1 + 0xd3c);
    void **p_d28 = (void **)(param_1 + 0xd28);

    void ***s0 = (void ***)(param_1 + 0xd50);

    if ((unsigned int)(param_2 - 0x7ffe) < 2) {
        VObj *f30 = *(VObj **)(param_1 + 0x30);
        f30->method3(1);
        return;
    }

    int bVar1 = 0;
    if (!entry[8]) {
        bVar1 = (entry[11] == 0 && entry[12] == 0);
    }

    if (!bVar1) {
        VObj *f30 = *(VObj **)(param_1 + 0x30);
        f30->method3(0);

        FUN_000ffb3c(*piVar4, 0);
        (*piVar4)->val24 = (*piVar4)->val28 + 23;

        Node *node = *(Node **)(s7 + 4);
        node->data = &DAT_002e0974;
        node->flag = 0;
        FUN_0010d4a8(node);

        int i = 0;
        do {
            FUN_000ffb3c(*s0, 1);
            i++;
            s0 = (void ***)((char *)s0 + 20);
        } while (i < 3);

        s3->data = &DAT_002e0974;
        s3->flag = 0;
        FUN_0010d4a8(s3);

        int uVar7 = FUN_001fea10(19, 0, 0);
        s2->data = (void *)uVar7;
        s2->flag = 1;
        FUN_0010d4a8(s2);

        FUN_000ffb3c(*piVar5, 1);

        s1->data = &DAT_002e0974;
        s1->flag = 0;
        FUN_0010d4a8(s1);

        uVar7 = FUN_001fea10(19, 0, 0);
        s6->data = (void *)uVar7;
        s6->flag = 1;
        FUN_0010d4a8(s6);

        FUN_000ffb3c(*puVar6, 1);
        FUN_000ffb3c(*p_d28, 1);
        return;
    }

    char flag76 = 0;
    int base_1025 = FUN_0016bb88();
    int c1;
    if (param_2 < 0) {
        c1 = 1;
    } else if (param_2 < 15) {
        c1 = *(unsigned char *)(base_1025 + param_2 + 0x1025);
    } else {
        c1 = 1;
    }
    if (c1 == 2) {
        flag76 = 1;
    }

    int p2_adj = FUN_00193474(param_2);
    int s8 = FUN_0016bb98(param_2, p2_adj + 0x68);

    VObj *f30 = *(VObj **)(param_1 + 0x30);
    f30->method3(0);

    FUN_000ffb3c(*piVar4, 0);
    int uVar7 = FUN_001fea10(11, 2, s8);
    (*piVar4)->val24 = (*piVar4)->val28 + param_2;

    Node *iVar9 = *(Node **)(s7 + 4);
    iVar9->data = (void *)uVar7;
    iVar9->flag = 1;
    FUN_0010d4a8(iVar9);

    if (s8 == 3) {
        int uVar7_3 = FUN_00216e00((char *)iVar2 + 0xc0);
        Node *f_i9 = *(Node **)(s7 + 4);
        f_i9->method6(uVar7_3);
        FUN_000ffb3c(s0[0], 1);
        FUN_000ffb3c(s0[5], 1);
        FUN_000ffb3c(s0[10], 0);
    } else if (s8 == 2) {
        int uVar7_2 = FUN_00216e00((char *)iVar2 + 0xb4);
        Node *f_i9 = *(Node **)(s7 + 4);
        f_i9->method6(uVar7_2);
        FUN_000ffb3c(s0[0], 1);
        FUN_000ffb3c(s0[5], 0);
        FUN_000ffb3c(s0[10], 1);
    } else if (s8 == 1) {
        Node *f_i9 = *(Node **)(s7 + 4);
        f_i9->method6(*(int *)(s7 + 8));
        FUN_000ffb3c(s0[0], 0);
        FUN_000ffb3c(s0[5], 1);
        FUN_000ffb3c(s0[10], 1);
    }

    uVar7 = FUN_001fea10(11, 0, (int)flag76 + 1);
    s3->data = (void *)uVar7;
    s3->flag = 0;
    FUN_0010d4a8(s3);

    s2->data = &DAT_002e0974;
    s2->flag = 0;
    FUN_0010d4a8(s2);

    FUN_000ffb3c(*piVar5, 0);

    if (!flag76) {
        (*piVar5)->val24 = (*piVar5)->val28;
    } else {
        (*piVar5)->val24 = (*piVar5)->val28 + 1;
    }

    char *ctx2 = (char *)FUN_0016bc70();
    int (*fn)(int) = *(int (**)(int))(ctx2 + 1000);
    char *res = (char *)fn(param_2);

    int cond_val = FUN_0024dd40(DAT_0034a77c, *(float *)(res + 4));
    FUN_000ffb3c(*p_d28, 0);

    if (cond_val >= 9) {
        FUN_000ffb3c(*puVar6, 0);
        s1->method3(1);
    } else {
        FUN_000ffb3c(*puVar6, 1);
        FUN_000143b0(&DAT_00341524, &DAT_002e0978, cond_val);
        s1->method3(0);
        s1->data = &DAT_00341524;
        s1->flag = 0;
        FUN_0010d4a8(s1);
    }

    s6->data = &DAT_002e0974;
    s6->flag = 0;
    FUN_0010d4a8(s6);
}

// Differing words: 168 / 340 words.
// Matches prologue (+0x00..+0x70, 30 instructions) and tail destruction sequence (+0x46c..+0x550, ~60 instructions) byte-for-byte.
// Differences:
// 1. 3-way boolean evaluation at +0x74: retail uses branch likely cascade with delay slot zeroing:
//    `lbu v0,8(v1); bnezl v0,0x182f9c; move v0,zero; lbu v0,11(v1); bnez v0,0x182f98; nop; lbu v0,12(v1); beqzl v0,0x182f9c; li v0,1; move v0,zero; bnezl v0,0x1830c4; sb zero,76(sp)`.
// 2. Ruled out: inline ternary helper and nested if constructs both emitted different register allocations (a0 instead of v0) or different branch conditions.
// 3. s8 dispatch cases (3, 2, 1) and delay-slot loads from 72(sp) vs 4(s7) differ in instruction ordering.
// near miss from lane dsn6 (unreviewed)
