/* HSTMATCH module=hstp_trans_main.prx start=0x00051500 end=0x000516a0 symbol=FUN_00051500 lang=c++ */
/* Ghidra pseudocode (hstgen.py tag sh2) corrected by hand: struct-typed vector clear (lane ag6 near miss), the post-loop count read through a cast to the same struct so it reloads like retail (matching idiom: identical-type cast), trailing empty by-value Tag for the frame; exact match verified; names and types await human review. */

#pragma opt_strength_reduction off

extern "C" {
extern int DAT_003d4584;
extern int DAT_003d458c;
extern int FUN_000045d8(int);
extern int FUN_000049e4(int);
extern void FUN_00004c3c(int, int, int);
extern void FUN_00004f44(void);
extern void FUN_0000511c(int);
extern void FUN_00005174(unsigned int);
extern void FUN_00005188(int);
extern void FUN_00005198(int);
extern void FUN_000051a8(int);
extern void FUN_000051d0(int, int);
extern void FUN_00005294(int, int, int, int);
extern int FUN_0003994c(void *);
extern int FUN_002c77f0(void);

struct Array {
    int *items;
    unsigned int count;
    void Clear() {
        for (unsigned int i = 0; i < count; i++) {
            FUN_000049e4(items[i]);
        }
        int n = (int)((Array *)this)->count;
        if (n != 0) {
            count -= n;
        }
    }
};

struct MyStruct {
    char pad_0[0x1a0a8];
    Array arr;
    char pad_1[4];
    unsigned char bytes[4];
};

struct Tag {};

void FUN_00051500(struct MyStruct *param_1, int param_2, int param_3, int param_4, Tag = Tag())
{
    FUN_000045d8(0);
    if (param_1->arr.count != 0) {
        param_1->arr.Clear();
    }
    {
        int iVar1 = DAT_003d4584;
        int uVar3 = DAT_003d458c;
        int iVar2 = FUN_002c77f0();
        FUN_00004c3c(uVar3, iVar1 - iVar2, 0x200);
    }
    FUN_000051d0(0x710, 0x778);
    FUN_00005294(0, 0, 0x1e0, 0x110);
    FUN_0000511c(2);
    {
        unsigned int val = param_1->bytes[0] | (param_1->bytes[1] << 8) |
                           (param_1->bytes[2] << 16) | (param_1->bytes[3] << 24);
        FUN_00005174(val);
    }
    FUN_00005198(0);
    FUN_00005188(0);
    if (param_4 != 0) {
        FUN_000051a8(23);
    } else {
        FUN_000051a8(22);
    }
    FUN_000049e4(FUN_0003994c(param_1));
    FUN_00004f44();
}
}
