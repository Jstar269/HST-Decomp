/* HSTMATCH module=EBOOT.BIN start=0x00050bec end=0x00050d8c symbol=FUN_00050bec lang=c++ */
/* Clone of src/sharing_main/clear_channel_00051500.cpp (hstclone.py tag c145620): addresses substituted; exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag sh2) corrected by hand: struct-typed vector clear (lane ag6 near miss), the post-loop count read through a cast to the same struct so it reloads like retail (matching idiom: identical-type cast), trailing empty by-value Tag for the frame; exact match verified; names and types await human review. */

#pragma opt_strength_reduction off

extern "C" {
extern int DAT_0032b244;
extern int DAT_0032b24c;
extern int FUN_000045d8(int);
extern int FUN_000049e4(int);
extern void FUN_00004c3c(int, int, int);
extern void FUN_00004f44(void);
extern void FUN_00005104(int);
extern void FUN_0000515c(unsigned int);
extern void FUN_00005170(int);
extern void FUN_00005180(int);
extern void FUN_00005190(int);
extern void FUN_000051b8(int, int);
extern void FUN_0000527c(int, int, int, int);
extern int FUN_00039924(void *);
extern int FUN_002b7a50(void);

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

void FUN_00050bec(struct MyStruct *param_1, int param_2, int param_3, int param_4, Tag = Tag())
{
    FUN_000045d8(0);
    if (param_1->arr.count != 0) {
        param_1->arr.Clear();
    }
    {
        int iVar1 = DAT_0032b244;
        int uVar3 = DAT_0032b24c;
        int iVar2 = FUN_002b7a50();
        FUN_00004c3c(uVar3, iVar1 - iVar2, 0x200);
    }
    FUN_000051b8(0x710, 0x778);
    FUN_0000527c(0, 0, 0x1e0, 0x110);
    FUN_00005104(2);
    {
        unsigned int val = param_1->bytes[0] | (param_1->bytes[1] << 8) |
                           (param_1->bytes[2] << 16) | (param_1->bytes[3] << 24);
        FUN_0000515c(val);
    }
    FUN_00005180(0);
    FUN_00005170(0);
    if (param_4 != 0) {
        FUN_00005190(23);
    } else {
        FUN_00005190(22);
    }
    FUN_000049e4(FUN_00039924(param_1));
    FUN_00004f44();
}
}
