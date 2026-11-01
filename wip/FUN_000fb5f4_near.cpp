/* HSTMATCH module=EBOOT.BIN start=0x000fb5f4 end=0x000fb734 symbol=FUN_000fb5f4 lang=c++ */
/* Hand-written reconstruction from the disassembly; near miss (40 words off); names and types await human review. */

struct SubA0 {
    char pad[0x514];
    unsigned char b514;
};

struct GlobalState {
    char pad[0x3c];
    int f3c;
    char pad5a[0x5a - 0x40];
    unsigned char b5a;
    char padA0[0xa0 - 0x5b];
    SubA0 *fa0;
};

struct ParamSub3 {
    char pad[0xc];
    int f0c;
};

struct ParamSub2 {
    char pad[0x94];
    ParamSub3 *f94;
};

struct ParamSub1 {
    char pad[0x3c];
    ParamSub2 *f3c;
};

struct Ac4Obj {
    char pad[0x198];
    int f198;
};

struct TableEntry {
    unsigned char flag;
    char pad[0x11f];
};

extern "C" GlobalState *DAT_003386b0;
extern "C" unsigned char DAT_003386b8;
extern "C" Ac4Obj *DAT_00338ac4;
extern "C" TableEntry DAT_003418bc[];
extern "C" int DAT_0034a678;
extern "C" void FUN_00234158(int *p, int n);

extern "C" int FUN_000fb5f4(ParamSub1 *param_1)
{
    GlobalState *gs = DAT_003386b0;
    if (9 == gs->f3c) {
        if (gs->fa0 != 0 && gs->fa0->b514 != 0)
            return 0;
        return 1;
    }

    int code = param_1->f3c->f94->f0c;
    if (code == 11)
        return 1;
    if (code == 7) {
        int idx = (DAT_003386b8 && DAT_00338ac4) ? DAT_00338ac4->f198 : 0;

        if (DAT_003418bc[idx].flag != 0) {
            FUN_00234158(&DAT_0034a678, 30);
            return 1;
        }

        switch (DAT_0034a678) {
        case 42:
            gs->b5a = 1;
            break;
        case 68:
            FUN_00234158(&DAT_0034a678, 30);
            break;
        case 49:
            gs->b5a = 1;
            break;
        default:
            FUN_00234158(&DAT_0034a678, 30);
            break;
        }
        return 1;
    }
    return 0;
}

// Differing words: 40 / 80 words. Exactly matching instruction count, control flow, delay slots, and logic.
// The diff is purely scratch register colouring: retail loads DAT_003386b0 into $v1 and uses $v0 for all temporaries
// (literals 9, 11, 7, table address calculation, switch values), whereas MWCC loads DAT_003386b0 into $v0 and uses $v1
// for temporaries.
// Ruled out:
// - Direct access without local gs (MWCC still assigns $v0 to the CSE pointer)
// - C99 vs C++ (generates identical assembly)
// - Operand reversal (9 == gs->f3c vs gs->f3c == 9)
// - Dummy locals declared before gs
// - Inverted branch logic for code == 7
// near miss from lane dsn4 (unreviewed)
