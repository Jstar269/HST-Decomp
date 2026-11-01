/* HSTMATCH module=EBOOT.BIN start=0x0005f32c end=0x0005f47c symbol=FUN_0005f32c lang=c99 */

typedef struct BitRef {
    unsigned int *word;
    unsigned int mask;
} BitRef;

extern char DAT_002bc5e0[];
extern int FUN_0005f7d0(int *set, unsigned int n);
extern void *FUN_00000bcc(unsigned int bytes);
extern int FUN_0000fe3c(void);
extern void FUN_0005f300(int handle, const char *msg);
extern void FUN_0000d518(void);
extern void FUN_000111c0(void *dst, const void *src, int bytes);
extern void FUN_00000a00(void *p);
extern void FUN_0005f6f0(BitRef *ref, unsigned int count, unsigned char *value);

/* 2026-10-31 re-try: the arithmetic levers do not apply here -- `param_1[2] << 5` instead of
   `* 0x20` costs 2 words (85/84), and declaring the BitRef before the byte does NOT move the
   address-taken byte from sp+47 to retail's sp+39. The whole remaining gap is the old-pointer
   branch: retail `beqzl a1,0x5f414; sw s1,0(s3)` (store in the delay) with the free/copy path
   following, ours `bnezl a1,...; lw a2,4(s3)`. Next idea: write that arm as
   `if (old == 0) { *param_1 = new; } else { ... }` so the store lands in the beqzl delay.
   NEW LEAD (2026-10-31, 3rd try): retail stores `sw s0,8(s3)` for param_1[2] but also has an
   `addiu v0,s0,1` right before it at 0x5f40c, i.e. the source keeps TWO values (n and n+1) where
   our draft keeps one. Drafts tried this pass: (a) iVar4=((iVar1-1)>>5)+1 stored directly (this
   file, 83/84), (b) iVar4=(iVar1-1)>>5 with `(iVar4+1)*4` alloc and `param_1[2]=iVar4+1`
   (81/84), (c) if(old==0){*param_1=new;}else{...} arms (no change). Next: make the count n and
   n+1 BOTH live -- allocate with n and store n+1, or vice versa -- to reproduce the extra addiu.
   NEAR MISS 83/84 words. Residual: retail fills the beqzl delay slot with lw a2,4(s3) and
   keeps to 4 s-regs; every other instruction shape (grow, alloc, error report, memcpy size
   ((count-1)>>5)*4+4 then >>2<<2, free, tail BitRef) is reproduced. Ruled out: loading
   param_1[1] before the size test (kills the delay-slot fill and adds a register), a local
   for the new pointer, and an if/else on the old pointer written with separate stores. */
void FUN_0005f32c(int *param_1, unsigned int param_2, unsigned char param_3)
{
    unsigned char local_9 = param_3;
    BitRef local;
    unsigned int uVar3;
    int iVar1;
    int iVar2;
    int iVar4;

    if ((unsigned int)param_1[1] < param_2) {
        if ((unsigned int)(param_1[2] * 0x20) < param_2) {
            iVar1 = FUN_0005f7d0(param_1, param_2 - param_1[2] * 0x20);
            iVar4 = (unsigned int)(iVar1 - 1) >> 5;
            iVar4 = iVar4 + 1;
            iVar1 = (int)FUN_00000bcc(iVar4 * 4);
            if (iVar1 == 0) {
                iVar2 = FUN_0000fe3c();
                FUN_0005f300(*(int *)(iVar2 + 0xc), DAT_002bc5e0);
                FUN_0000d518();
                iVar2 = *param_1;
            }
            else {
                iVar2 = *param_1;
            }
            if (iVar2 == 0) {
                *param_1 = iVar1;
            }
            else {
                if (param_1[1] == 0) {
                    iVar2 = *param_1;
                }
                else {
                    FUN_000111c0((void *)iVar1, (void *)iVar2, (((int)((param_1[1] - 1U) >> 5) * 4) + 4));
                    iVar2 = *param_1;
                }
                FUN_00000a00((void *)iVar2);
                *param_1 = iVar1;
            }
            param_1[2] = iVar4;
            uVar3 = param_1[1];
        }
        else {
            uVar3 = param_1[1];
        }
        local.word = (unsigned int *)(*param_1 + (uVar3 >> 5) * 4);
        param_1[1] = param_2;
        local.mask = 1 << (uVar3 & 0x1f);
        FUN_0005f6f0(&local, param_2 - uVar3, &local_9);
    }
    else {
        param_1[1] = param_2;
    }
}
// near miss from lane dsn8 (unreviewed)
