/* HSTMATCH module=hstp_trans_main.prx start=0x0025d660 end=0x0025d7c0 symbol=FUN_0025d660 lang=c++ */
/* Clone of src/eboot/FUN_0024d458.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" void FUN_0005ffd8(void *p, int n);

struct GlobalData1 {
    char pad[0x1d8];
    float f1d8;
};

struct GlobalData2 {
    char pad[0x8c];
    int f8c;
};

extern "C" int FUN_0013d2b0(GlobalData2 *p);

struct TargetObj {
    char pad[0x3c];
    int f3c;
    char pad50[0x50 - 0x40];
    int f50;
    char padB8[0xb8 - 0x54];
    float fb8;
    char pad249[0x249 - 0xbc];
    unsigned char b249;
    char pad24c[0x24c - 0x24a];
    float f24c;
    float f250;
    unsigned char b254;

    void Reset() {
        if (f3c == 2 && b249 != 0) {
            FUN_0005ffd8(&f50, 0);
            b254 = 0;
        }
    }
};

extern "C" GlobalData1 *DAT_003da704;
extern "C" GlobalData2 *DAT_003e95c4;

extern "C" void FUN_0025d660(TargetObj *self)
{
    if (DAT_003e95c4->f8c == 0 || FUN_0013d2b0(DAT_003e95c4) != 0) {
        if (DAT_003da704->f1d8 > self->f24c) {
            if (self->fb8 > self->f24c) {
                self->b254 = 1;
            } else {
                self->Reset();
            }
        } else if (DAT_003da704->f1d8 < self->f250) {
            if (self->fb8 < self->f250) {
                self->b254 = 1;
            } else {
                self->Reset();
            }
        } else {
            self->Reset();
        }
    } else {
        self->Reset();
    }
}
