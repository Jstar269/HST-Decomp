/* HSTMATCH module=EBOOT.BIN start=0x0027d34c end=0x0027d940 symbol=FUN_0027d34c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct CameraEntry {
    float f0;
    float f4;
    float f8;
    float fc;
    int f10;
    float f14;
    float f18;
    float f1c;
    int f20;
    int f24;
    int f28;
    float f2c;
    int f30;
    int f34[2];
    int f3c;
};

extern "C" {
extern CameraEntry *DAT_0034a7e4;
extern char DAT_002fe6d8[];
extern char DAT_002fe710[];
extern char DAT_002fe714[];
extern char DAT_002fe724[];

void FUN_000649c0(void *);
int FUN_000143b0(char *, const char *, ...);
unsigned char FUN_00064bf4(void *, const char *, int, const void *);
void FUN_00064a08(void *, int);
int FUN_000650e0(void *);
char *FUN_0006517c(void *, int, int);
int FUN_00014d64(const char *, const char *);
float FUN_00065140(void *, int, int);
int FUN_00065104(void *, int, int);
int FUN_0027d940(void *, const char *);
int FUN_0027da7c(void *, const char *);
int FUN_0027d9b4(void *, const char *);
void FUN_0027da84(void *, const char *);
int FUN_0027da8c(void *, const char *);
}

extern "C" int FUN_0027d34c(void *param_1, const char *param_2) {
    char parser[16];
    char path[256];

    FUN_000649c0(parser);
    FUN_000143b0(path, DAT_002fe6d8, param_2);
    if (FUN_00064bf4(parser, path, 1, DAT_002fe710) == 0) {
        FUN_00064a08(parser, -1);
        return 0;
    }

    for (int i = 0; i < 128; i++) {
        DAT_0034a7e4[i].f0 = 0.0f;
        DAT_0034a7e4[i].f4 = 0.0f;
        DAT_0034a7e4[i].f8 = 0.0f;
        DAT_0034a7e4[i].fc = 0.0f;
        DAT_0034a7e4[i].f10 = 0;
        DAT_0034a7e4[i].f14 = 0.0f;
        DAT_0034a7e4[i].f18 = 0.0f;
        DAT_0034a7e4[i].f1c = 0.0f;
        DAT_0034a7e4[i].f20 = 0;
        DAT_0034a7e4[i].f24 = 0;
        DAT_0034a7e4[i].f28 = 0;
        DAT_0034a7e4[i].f2c = 0.0f;
        DAT_0034a7e4[i].f30 = 0;
        DAT_0034a7e4[i].f34[0] = 0;
        DAT_0034a7e4[i].f34[1] = 0;
        DAT_0034a7e4[i].f3c = 1;
    }

    int slot = 0;
    int row = 0;
    int count = FUN_000650e0(parser);
    if (count > 0) {
        do {
            int col = 0;
            char *str = FUN_0006517c(parser, row, 0);
            if (FUN_00014d64(str, DAT_002fe714) != 0) {
                row++;
                slot = 0;
            } else if (FUN_00014d64(FUN_0006517c(parser, row, 0), DAT_002fe724) != 0) {
                row++;
                slot = 64;
            }

            col++;
            DAT_0034a7e4[slot].f0 = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f4 = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f8 = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].fc = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f10 = FUN_0027d940(param_1, FUN_0006517c(parser, row, col++));
            DAT_0034a7e4[slot].f14 = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f18 = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f1c = FUN_00065140(parser, row, col++);
            DAT_0034a7e4[slot].f20 = FUN_0027d940(param_1, FUN_0006517c(parser, row, col++));
            FUN_0027d940(param_1, FUN_0006517c(parser, row, col++));
            FUN_0027da7c(param_1, FUN_0006517c(parser, row, col++));
            DAT_0034a7e4[slot].f24 = FUN_0027d9b4(param_1, FUN_0006517c(parser, row, col++));
            DAT_0034a7e4[slot].f28 = FUN_0027da7c(param_1, FUN_0006517c(parser, row, col++));
            DAT_0034a7e4[slot].f2c = FUN_00065140(parser, row, col++);
            FUN_0027da84(param_1, FUN_0006517c(parser, row, col++));
            DAT_0034a7e4[slot].f30 = FUN_00065104(parser, row, col++);

            for (int k = 0; k < 2; k++) {
                if ((int)FUN_0006517c(parser, row, col) > 0) {
                    DAT_0034a7e4[slot].f34[k] = FUN_0027da8c(param_1, FUN_0006517c(parser, row, col++));
                } else {
                    DAT_0034a7e4[slot].f34[k] = FUN_00065104(parser, row, col++);
                }
            }

            slot++;
            row++;
            count = FUN_000650e0(parser);
        } while (row < count);
    }

    FUN_00064a08(parser, -1);

    for (int i = 0; i < 128; i++) {
        int cnt = DAT_0034a7e4[i].f3c;
        if (cnt > 0) {
            DAT_0034a7e4[i].f3c = 1;
            for (int j = i + 1; j < 128; j++) {
                if (DAT_0034a7e4[i].f30 == DAT_0034a7e4[j].f30) {
                    DAT_0034a7e4[i].f3c++;
                    DAT_0034a7e4[j].f3c = 0;
                }
            }
        } else {
            DAT_0034a7e4[i].f3c = 0;
        }
    }

    return 1;
}
