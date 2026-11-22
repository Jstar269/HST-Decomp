/* HSTMATCH module=EBOOT.BIN start=0x001ccf40 end=0x001cd060 symbol=FUN_001ccf40 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode (wip near miss near_miss_001ccf40, lane u6fr1-0) corrected by hand: `sub` declared first and assigned late, `iVar2 = -1` declared after the first calls, case 8 stores iVar2 before c4/c8, `iVar2 >= 0`; exact match verified; names and types await human review. */

extern int DAT_002ea2ec;
extern char DAT_0034a100;
extern void FUN_001c2498(void *);
extern void FUN_001cd88c(void *, int, int);
extern void FUN_0022c94c(int);
extern void FUN_0022cd38(void *, int);
extern void *FUN_0022cd60(void);
extern void FUN_00279b7c(int);
extern void FUN_002afd2c(void);

struct Sub {
    char pad0[248];
    int f_f8;
    int f_fc;
    int f_100;
    char f_104;
};

struct Obj {
    char pad0[196];
    int f_c4;
    int f_c8;
    int f_cc;
    char padc0[348 - 208];
    struct Sub *sub;
};

void FUN_001ccf40(struct Obj *param_1)
{
    struct Sub *sub;
    FUN_0022c94c(param_1->f_cc);
    FUN_00279b7c((param_1->f_cc + 1) * 10);
    DAT_0034a100 = 1;
    int iVar2 = -1;

    switch (param_1->f_cc + 1) {
    case 8:
        iVar2 = 0;
        param_1->f_c4 = 5;
        param_1->f_c8 = 0;
        break;
    case 9:
        param_1->f_c4 = 5;
        iVar2 = 1;
        param_1->f_c8 = 0;
        break;
    }

    if (iVar2 != -1) {
        FUN_0022cd38(FUN_0022cd60(), iVar2);
    }
    sub = param_1->sub;

    if (iVar2 >= 0) {
        sub->f_f8 = iVar2;
        sub->f_fc = DAT_002ea2ec;
        sub->f_100 = 11;
        sub->f_104 = iVar2 >= 0;
        FUN_002afd2c();
        FUN_001cd88c(sub, 12, 1);
    } else {
        FUN_001cd88c(sub, 11, 1);
    }
    FUN_001c2498(sub);
}
// near miss from lane u6fr1-0 (unreviewed)
