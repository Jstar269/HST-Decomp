/* HSTMATCH module=hstp_trans_main.prx start=0x0028e6bc end=0x0028ecb0 symbol=FUN_0028e6bc lang=c++ */
/* Clone of src/eboot/camera_script_load_0027d34c.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
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
extern CameraEntry *DAT_003f3b14;
extern char DAT_003a7458[];
extern char DAT_003a7490[];
extern char DAT_003a7494[];
extern char DAT_003a74a4[];

void FUN_00065884(void *);
int FUN_000143b0(char *, const char *, ...);
unsigned char FUN_00065ab8(void *, const char *, int, const void *);
void FUN_000658cc(void *, int);
int FUN_00065fa4(void *);
char *FUN_000661a8(void *, int, int);
int FUN_00014d64(const char *, const char *);
float FUN_000660f0(void *, int, int);
int FUN_00066038(void *, int, int);
int FUN_0028ecb0(void *, const char *);
int FUN_0028edec(void *, const char *);
int FUN_0028ed24(void *, const char *);
void FUN_0028edf4(void *, const char *);
int FUN_0028edfc(void *, const char *);
}

extern "C" int FUN_0028e6bc(void *param_1, const char *param_2) {
    char parser[16];
    char path[256];

    FUN_00065884(parser);
    FUN_000143b0(path, DAT_003a7458, param_2);
    if (FUN_00065ab8(parser, path, 1, DAT_003a7490) == 0) {
        FUN_000658cc(parser, -1);
        return 0;
    }

    for (int i = 0; i < 128; i++) {
        DAT_003f3b14[i].f0 = 0.0f;
        DAT_003f3b14[i].f4 = 0.0f;
        DAT_003f3b14[i].f8 = 0.0f;
        DAT_003f3b14[i].fc = 0.0f;
        DAT_003f3b14[i].f10 = 0;
        DAT_003f3b14[i].f14 = 0.0f;
        DAT_003f3b14[i].f18 = 0.0f;
        DAT_003f3b14[i].f1c = 0.0f;
        DAT_003f3b14[i].f20 = 0;
        DAT_003f3b14[i].f24 = 0;
        DAT_003f3b14[i].f28 = 0;
        DAT_003f3b14[i].f2c = 0.0f;
        DAT_003f3b14[i].f30 = 0;
        DAT_003f3b14[i].f34[0] = 0;
        DAT_003f3b14[i].f34[1] = 0;
        DAT_003f3b14[i].f3c = 1;
    }

    int slot = 0;
    int row = 0;
    int count = FUN_00065fa4(parser);
    if (count > 0) {
        do {
            int col = 0;
            char *str = FUN_000661a8(parser, row, 0);
            if (FUN_00014d64(str, DAT_003a7494) != 0) {
                row++;
                slot = 0;
            } else if (FUN_00014d64(FUN_000661a8(parser, row, 0), DAT_003a74a4) != 0) {
                row++;
                slot = 64;
            }

            col++;
            DAT_003f3b14[slot].f0 = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f4 = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f8 = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].fc = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f10 = FUN_0028ecb0(param_1, FUN_000661a8(parser, row, col++));
            DAT_003f3b14[slot].f14 = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f18 = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f1c = FUN_000660f0(parser, row, col++);
            DAT_003f3b14[slot].f20 = FUN_0028ecb0(param_1, FUN_000661a8(parser, row, col++));
            FUN_0028ecb0(param_1, FUN_000661a8(parser, row, col++));
            FUN_0028edec(param_1, FUN_000661a8(parser, row, col++));
            DAT_003f3b14[slot].f24 = FUN_0028ed24(param_1, FUN_000661a8(parser, row, col++));
            DAT_003f3b14[slot].f28 = FUN_0028edec(param_1, FUN_000661a8(parser, row, col++));
            DAT_003f3b14[slot].f2c = FUN_000660f0(parser, row, col++);
            FUN_0028edf4(param_1, FUN_000661a8(parser, row, col++));
            DAT_003f3b14[slot].f30 = FUN_00066038(parser, row, col++);

            for (int k = 0; k < 2; k++) {
                if ((int)FUN_000661a8(parser, row, col) > 0) {
                    DAT_003f3b14[slot].f34[k] = FUN_0028edfc(param_1, FUN_000661a8(parser, row, col++));
                } else {
                    DAT_003f3b14[slot].f34[k] = FUN_00066038(parser, row, col++);
                }
            }

            slot++;
            row++;
            count = FUN_00065fa4(parser);
        } while (row < count);
    }

    FUN_000658cc(parser, -1);

    for (int i = 0; i < 128; i++) {
        int cnt = DAT_003f3b14[i].f3c;
        if (cnt > 0) {
            DAT_003f3b14[i].f3c = 1;
            for (int j = i + 1; j < 128; j++) {
                if (DAT_003f3b14[i].f30 == DAT_003f3b14[j].f30) {
                    DAT_003f3b14[i].f3c++;
                    DAT_003f3b14[j].f3c = 0;
                }
            }
        } else {
            DAT_003f3b14[i].f3c = 0;
        }
    }

    return 1;
}
