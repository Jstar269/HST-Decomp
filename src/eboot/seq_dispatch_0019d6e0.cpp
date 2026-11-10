/* HSTMATCH module=EBOOT.BIN start=0x0019d6e0 end=0x0019da6c symbol=FUN_0019d6e0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Seq {
    int state;
};

struct Opts {
    char pad[0x178];
    unsigned char flag;
};

struct Item {
    char pad[0x28];
    unsigned char kind;
};

extern Seq DAT_0034a678;
extern Opts *DAT_00338ac4;

extern "C" {
void FUN_002345f8(Seq *);
void FUN_00234158(Seq *, int);
void FUN_0023466c(Seq *);
}

extern "C" void FUN_0019d6e0(int mode, Item *item, int alt)
{
    switch (mode) {
    case 1:
        FUN_002345f8(&DAT_0034a678);
        break;
    case 2:
        switch (DAT_0034a678.state) {
        case 41:
            if (alt != 0) {
                if (DAT_00338ac4->flag != 0)
                    FUN_00234158(&DAT_0034a678, 39);
                else
                    FUN_00234158(&DAT_0034a678, 56);
            } else {
                switch (item->kind) {
                case 0:
                case 3:
                case 4:
                    FUN_00234158(&DAT_0034a678, 39);
                    break;
                default:
                    FUN_00234158(&DAT_0034a678, 40);
                    break;
                }
            }
            break;
        case 48:
            if (alt != 0) {
                if (DAT_00338ac4->flag != 0)
                    FUN_00234158(&DAT_0034a678, 46);
                else
                    FUN_00234158(&DAT_0034a678, 58);
            } else {
                switch (item->kind) {
                case 0:
                case 3:
                case 4:
                    FUN_00234158(&DAT_0034a678, 46);
                    break;
                default:
                    FUN_00234158(&DAT_0034a678, 47);
                    break;
                }
            }
            break;
        case 52:
            FUN_00234158(&DAT_0034a678, 51);
            break;
        case 67:
            FUN_00234158(&DAT_0034a678, 63);
            break;
        case 86:
            FUN_00234158(&DAT_0034a678, 85);
            break;
        default:
            FUN_0023466c(&DAT_0034a678);
            break;
        }
        break;
    case 3:
        switch (DAT_0034a678.state) {
        case 48:
            switch (item->kind) {
            case 0:
            case 3:
            case 4:
                FUN_00234158(&DAT_0034a678, 44);
                break;
            default:
                FUN_00234158(&DAT_0034a678, 45);
                break;
            }
            break;
        case 41:
            switch (item->kind) {
            case 0:
            case 3:
            case 4:
                FUN_00234158(&DAT_0034a678, 37);
                break;
            default:
                FUN_00234158(&DAT_0034a678, 38);
                break;
            }
            break;
        default:
            FUN_0023466c(&DAT_0034a678);
            break;
        }
        break;
    case 4:
        switch (DAT_0034a678.state) {
        case 41:
            if (DAT_00338ac4->flag != 0)
                FUN_00234158(&DAT_0034a678, 32);
            else
                FUN_00234158(&DAT_0034a678, 30);
            break;
        case 48:
            if (DAT_00338ac4->flag != 0)
                FUN_00234158(&DAT_0034a678, 43);
            else
                FUN_00234158(&DAT_0034a678, 30);
            break;
        default:
            FUN_00234158(&DAT_0034a678, 30);
            break;
        }
        break;
    }
}
