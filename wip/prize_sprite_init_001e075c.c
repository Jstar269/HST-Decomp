/* HSTMATCH module=EBOOT.BIN start=0x001e075c end=0x001e09e0 symbol=FUN_001e075c lang=c99 */

extern char DAT_002ebaec[];
extern char DAT_002ebaf8[];
extern unsigned int DAT_0034a678;
extern void FUN_00000020(const char *tag);
extern void FUN_00000028(void);
extern void *FUN_00000bcc(unsigned int bytes);
extern unsigned int FUN_00102e38(void *mem);
extern unsigned int FUN_001ee200(void);
extern void FUN_001e1fb0(int param_1);
extern void FUN_0001cad0(int id, int a, const char *name);
extern void FUN_001e1964(int param_1);
extern void FUN_001e1d70(int param_1);
extern void FUN_001e1e24(int param_1);
extern void FUN_001e1ed8(int param_1);
extern unsigned int FUN_002226b4(void *mem, int a);
extern void FUN_001e23ac(int param_1);
extern void FUN_001e22cc(int param_1);
extern void FUN_001e2528(int param_1);
extern void FUN_0001cb88(int id);

/* NEAR MISS 179/161 words. Residual: retail keeps param_1 in s1 (this form in s2), takes
   bVar2 in s0, and formats the global test as `li v0,49; beq v1,v0` before placing the
   default `li s0,1`; the repeated alloc pattern (bcc(0x54) -> 0x102e38 -> store, or 0) is
   18 words shorter here, so one of the two-branch stores is probably a shared temporary in
   the source. Structure: whole body under `if (*(int *)(param_1 + 500) == 0)` with the
   finished object check, two FUN_001ee200() flag gates (ctx+0x599, ctx+0x597), then
   FUN_001e1fb0/0001cad0/FUN_001e1964/1d70/1e24/1ed8, the 0xcc alloc through FUN_002226b4,
   the bVar1/bVar2 extras, FUN_001e2528, FUN_0001cb88, and FUN_00000028 outside the if. */
void FUN_001e075c(int param_1)
{
    unsigned char bVar1;
    unsigned char bVar2;
    int iVar3;
    unsigned int uVar4;

    FUN_00000020(DAT_002ebaec);
    if (*(int *)(param_1 + 500) == 0) {
    bVar2 = 1;
    bVar1 = 0;
    if (DAT_0034a678 == 0x31) {
        bVar2 = 0;
        bVar1 = 1;
    }
    else if (DAT_0034a678 == 0) {
        bVar1 = 1;
    }
    iVar3 = (int)FUN_00000bcc(0x54);
    if (iVar3 == 0) {
        *(unsigned int *)(param_1 + 500) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 500) = FUN_00102e38((void *)iVar3);
    }
    iVar3 = FUN_001ee200();
    if (*(char *)(iVar3 + 0x599) != 0) {
        iVar3 = (int)FUN_00000bcc(0x54);
        if (iVar3 == 0) {
            *(unsigned int *)(param_1 + 0x810) = 0;
        }
        else {
            *(unsigned int *)(param_1 + 0x810) = FUN_00102e38((void *)iVar3);
        }
    }
    iVar3 = FUN_001ee200();
    if (*(char *)(iVar3 + 0x597) != 0) {
        iVar3 = (int)FUN_00000bcc(0x54);
        if (iVar3 == 0) {
            *(unsigned int *)(param_1 + 0x175c) = 0;
        }
        else {
            *(unsigned int *)(param_1 + 0x175c) = FUN_00102e38((void *)iVar3);
        }
    }
    iVar3 = (int)FUN_00000bcc(0x54);
    if (iVar3 == 0) {
        *(unsigned int *)(param_1 + 0x1d68) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 0x1d68) = FUN_00102e38((void *)iVar3);
    }
    iVar3 = (int)FUN_00000bcc(0x54);
    if (iVar3 == 0) {
        *(unsigned int *)(param_1 + 0x1958) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 0x1958) = FUN_00102e38((void *)iVar3);
    }
    iVar3 = (int)FUN_00000bcc(0x54);
    if (iVar3 == 0) {
        uVar4 = 0;
    }
    else {
        uVar4 = FUN_00102e38((void *)iVar3);
    }
    *(unsigned int *)(param_1 + 0x195c) = uVar4;
    if (bVar1) {
        iVar3 = (int)FUN_00000bcc(0x54);
        if (iVar3 == 0) {
            *(unsigned int *)(param_1 + 0x235c) = 0;
        }
        else {
            *(unsigned int *)(param_1 + 0x235c) = FUN_00102e38((void *)iVar3);
        }
    }
    if (bVar2) {
        iVar3 = (int)FUN_00000bcc(0x54);
        if (iVar3 == 0) {
            *(unsigned int *)(param_1 + 0x21e4) = 0;
        }
        else {
            *(unsigned int *)(param_1 + 0x21e4) = FUN_00102e38((void *)iVar3);
        }
    }
    iVar3 = (int)FUN_00000bcc(0x54);
    if (iVar3 == 0) {
        *(unsigned int *)(param_1 + 0x250c) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 0x250c) = FUN_00102e38((void *)iVar3);
    }
    FUN_001e1fb0(param_1);
    FUN_0001cad0(0x30ebf4, 1, DAT_002ebaf8);
    FUN_001e1964(param_1);
    iVar3 = FUN_001ee200();
    if (*(char *)(iVar3 + 0x599) != 0) {
        FUN_001e1d70(param_1);
    }
    iVar3 = FUN_001ee200();
    if (*(char *)(iVar3 + 0x597) != 0) {
        FUN_001e1e24(param_1);
    }
    FUN_001e1ed8(param_1);
    iVar3 = (int)FUN_00000bcc(0xcc);
    if (iVar3 == 0) {
        *(unsigned int *)(param_1 + 0x1954) = 0;
    }
    else {
        *(unsigned int *)(param_1 + 0x1954) = FUN_002226b4((void *)iVar3, 1);
    }
    if (bVar1) {
        FUN_001e23ac(param_1);
    }
    if (bVar2) {
        FUN_001e22cc(param_1);
    }
    FUN_001e2528(param_1);
    FUN_0001cb88(0x30ebf4);
    }
    FUN_00000028();
}
// near miss from lane dsn8 (unreviewed)
