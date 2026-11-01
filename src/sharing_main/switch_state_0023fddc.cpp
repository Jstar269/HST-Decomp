/* HSTMATCH module=hstp_trans_main.prx start=0x0023fddc end=0x00240060 rodata=0x002da1bc symbol=FUN_0023fddc lang=c++ */
/* Clone of src/eboot/switch_state_0022fcdc.cpp (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
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
   Matching idioms kept here: (a) DAT_003eaad8 is passed at POINTER-typed call sites so MWCC
   rematerialises lui/addiu instead of caching the address in a callee-saved register (which cost
   one extra save and a 2-word shift); (b) FUN_0016a3c0 really takes a third argument -- the 1
   that the callee ignores -- because retail materialises it once in a2 and reuses it for the
   byte store at obj+4 (see wip note history); (c) the float4 block is loaded/stored as a plain
   local array in source order.
*/
extern "C" {
extern float DAT_0039bd70;
extern float DAT_0039bd74;
extern float DAT_0039bd78;
extern float DAT_0039bd7c;
extern char DAT_003eaad8[];
extern unsigned int DAT_003f3b48;
extern unsigned int DAT_003f3b4c;
extern unsigned int DAT_003ea914;
extern unsigned int DAT_003ea8ec;
extern void FUN_00240060(void);
extern void FUN_002400e4(void);
extern void FUN_0023d948(int param_1, int a);
extern unsigned int FUN_0016a3c0(unsigned int a, unsigned int b, int c);
extern void FUN_00108f54(int a, int b);
extern void FUN_0023de04(int param_1);
extern void FUN_0023d77c(int param_1);
extern void FUN_001c20b8(char *a);
extern void FUN_001c04c4(char *a);
extern void *FUN_00000bcc(unsigned int bytes);
extern void FUN_002a2acc(void *mem, int a);
extern unsigned int FUN_0008f188(unsigned int a);
extern void FUN_002a2860(unsigned int a, unsigned int b, unsigned int c);
extern void FUN_0023e74c(int param_1);
extern void FUN_0019cdc8(unsigned int a, int b);
extern unsigned int FUN_0020d1e0(int a, int b, int c);
extern void FUN_0011e940(unsigned int a, int b, unsigned int c, int d, int e);

}

extern "C" void FUN_0023fddc(int param_1, int param_2)
{
    int iVar1;
    unsigned int uVar2;
    float local_10[4];

    if (param_2 == *(int *)(param_1 + 0x414)) {
        FUN_00240060();
    }
    else {
        FUN_002400e4();
        switch (param_2) {
        case 0:
            break;
        case 1:
            FUN_0023d948(param_1, 1);
            break;
        case 2:
            *(unsigned char *)(*(int *)(param_1 + 0x21c) + 4) = 1;
            uVar2 = FUN_0016a3c0(*(unsigned int *)(param_1 + 0x21c), *(unsigned int *)(param_1 + 0x254), 1);
            *(unsigned int *)(param_1 + 0x254) = uVar2;
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00108f54(*(int *)(param_1 + 0x214), 1);
            }
            break;
        case 3:
            FUN_0023de04(param_1);
            break;
        case 4:
            FUN_0023d77c(param_1);
            break;
        case 5:
            local_10[0] = DAT_0039bd70;
            local_10[1] = DAT_0039bd74;
            local_10[2] = DAT_0039bd78;
            local_10[3] = DAT_0039bd7c;
            FUN_001c20b8(DAT_003eaad8);
            FUN_001c04c4(DAT_003eaad8);
            iVar1 = (int)FUN_00000bcc(0x48);
            if (iVar1 != 0) {
                FUN_002a2acc((void *)iVar1, 1);
            }
            iVar1 = *(int *)(param_1 + 0x260);
            uVar2 = FUN_0008f188(*(unsigned int *)&local_10[*(int *)(*(int *)(param_1 + 0x2e4) + iVar1 * 4)]);
            FUN_002a2860(DAT_003f3b48, uVar2, DAT_003f3b4c);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00108f54(*(int *)(param_1 + 0x214), 1);
            }
            break;
        case 6:
            FUN_0023e74c(param_1);
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00108f54(*(int *)(param_1 + 0x214), 1);
            }
            FUN_0019cdc8(DAT_003ea914, 6);
            break;
        case 7:
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00108f54(*(int *)(param_1 + 0x214), 1);
            }
            FUN_0019cdc8(DAT_003ea914, 8);
            break;
        case 8:
            *(unsigned char *)(param_1 + 0xb8) = 1;
            uVar2 = FUN_0020d1e0(0x11, 0x14, 5);
            FUN_0011e940(DAT_003ea8ec, 2, uVar2, 0, 0);
            ((W *)*(int *)(param_1 + 0x210))->v1(1);
            if (*(int *)(param_1 + 0x214) != 0) {
                FUN_00108f54(*(int *)(param_1 + 0x214), 1);
            }
            break;
        }
        *(int *)(param_1 + 0x414) = param_2;
    }
}
