/* HSTMATCH module=EBOOT.BIN start=0x0022fcdc end=0x0022ff60 rodata=0x002c9bfc symbol=FUN_0022fcdc lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
struct W {
    virtual void v0();
    virtual void v1(int);
};

/*
   Enter-state handler for a gameplay object (state cached at +0x414).  The function is a switch
   over the requested state; each arm does its own setup (spawn, camera, notice, flags) and the
   last statement records the new state.  The vtable calls at +0xc are real C++ virtuals, which is
   why they are spelled with an inline class cast: a C function-pointer struct emits jalr v0.
   Matching idioms kept here: (a) DAT_0034179c is passed at POINTER-typed call sites so MWCC
   rematerialises lui/addiu instead of caching the address in a callee-saved register (which cost
   one extra save and a 2-word shift); (b) FUN_0015de34 really takes a third argument -- the 1
   that the callee ignores -- because retail materialises it once in a2 and reuses it for the
   byte store at obj+4 (see wip note history); (c) the float4 block is loaded/stored as a plain
   local array in source order.
*/
extern "C" {
extern float DAT_002f34ac;
extern float DAT_002f34b0;
extern float DAT_002f34b4;
extern float DAT_002f34b8;
extern char DAT_0034179c[];
extern unsigned int DAT_0034a818;
extern unsigned int DAT_0034a81c;
extern unsigned int DAT_003415d4;
extern unsigned int DAT_003415ac;
extern void FUN_0022ff60(void);
extern void FUN_0022ffe4(void);
extern void FUN_0022d848(int param_1, int a);
extern unsigned int FUN_0015de34(unsigned int a, unsigned int b, int c);
extern void FUN_00103628(int a, int b);
extern void FUN_0022dd04(int param_1);
extern void FUN_0022d67c(int param_1);
extern void FUN_001b3e2c(char *a);
extern void FUN_001b2430(char *a);
extern void *FUN_00000bcc(unsigned int bytes);
extern void FUN_002914c0(void *mem, int a);
extern unsigned int FUN_0008d9ac(unsigned int a);
extern void FUN_00291158(unsigned int a, unsigned int b, unsigned int c);
extern void FUN_0022e64c(int param_1);
extern void FUN_001905b8(unsigned int a, int b);
extern unsigned int FUN_001fea10(int a, int b, int c);
extern void FUN_00114168(unsigned int a, int b, unsigned int c, int d, int e);

}

extern "C" void FUN_0022fcdc(int param_1, int param_2)
{
    int iVar1;
    unsigned int uVar2;
    float local_10[4];

    if (param_2 == *(int *)(param_1 + 0x414)) {
        FUN_0022ff60();
    }
    else {
        FUN_0022ffe4();
        switch (param_2) {
        case 0:
            break;
        case 1:
            FUN_0022d848(param_1, 1);
            break;
        case 2:
            *(unsigned char *)(*(int *)(param_1 + 0x21c) + 4) = 1;
            uVar2 = FUN_0015de34(*(unsigned int *)(param_1 + 0x21c), *(unsigned int *)(param_1 + 0x254), 1);
            *(unsigned int *)(param_1 + 0x254) = uVar2;
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00103628(*(int *)(param_1 + 0x214), 1);
            }
            break;
        case 3:
            FUN_0022dd04(param_1);
            break;
        case 4:
            FUN_0022d67c(param_1);
            break;
        case 5:
            local_10[0] = DAT_002f34ac;
            local_10[1] = DAT_002f34b0;
            local_10[2] = DAT_002f34b4;
            local_10[3] = DAT_002f34b8;
            FUN_001b3e2c(DAT_0034179c);
            FUN_001b2430(DAT_0034179c);
            iVar1 = (int)FUN_00000bcc(0x48);
            if (iVar1 != 0) {
                FUN_002914c0((void *)iVar1, 1);
            }
            iVar1 = *(int *)(param_1 + 0x260);
            uVar2 = FUN_0008d9ac(*(unsigned int *)&local_10[*(int *)(*(int *)(param_1 + 0x2e4) + iVar1 * 4)]);
            FUN_00291158(DAT_0034a818, uVar2, DAT_0034a81c);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00103628(*(int *)(param_1 + 0x214), 1);
            }
            break;
        case 6:
            FUN_0022e64c(param_1);
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00103628(*(int *)(param_1 + 0x214), 1);
            }
            FUN_001905b8(DAT_003415d4, 6);
            break;
        case 7:
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00103628(*(int *)(param_1 + 0x214), 1);
            }
            FUN_001905b8(DAT_003415d4, 8);
            break;
        case 8:
            *(unsigned char *)(param_1 + 0xb8) = 1;
            uVar2 = FUN_001fea10(0x11, 0x14, 5);
            FUN_00114168(DAT_003415ac, 2, uVar2, 0, 0);
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00103628(*(int *)(param_1 + 0x214), 1);
            }
            break;
        }
        *(int *)(param_1 + 0x414) = param_2;
    }
}
