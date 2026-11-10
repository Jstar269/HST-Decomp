/* HSTMATCH module=EBOOT.BIN start=0x001c6608 end=0x001c67e8 symbol=FUN_001c6608 lang=c++ */
/* Hand-written reconstruction from the disassembly (lane wip, then the dropped call arguments FUN_001c6354(self) and FUN_002917ac(DAT_0034a81c) restored; matching idiom: the doubled p+0x61 test); exact match verified; names and types await human review. */

struct Disp_1c6608 {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

extern "C" {
extern int DAT_0034a81c;
extern char DAT_00311170[];
extern void FUN_001c6354(char *self);
extern int FUN_00050534(char *name);
extern void FUN_00051398(int id, float f);
extern void FUN_00051490(int id, int a, int b, int c, int d, int e, int f);
extern void FUN_00051894(void *tex, float u0, float v0, float x0, float y0,
                         float u1, float v1, float x1, float y1, float z,
                         unsigned color);
extern void FUN_0005185c(int id);
extern void FUN_000503d0(char *msg, float v);
extern void FUN_002917ac(int tex);
}

extern "C" void FUN_001c6608(char *self)
{
    char *p = *(char **)(self + 0x15c);
    int mode;
    int b;

    b = 0;
    b = *(int *)(p + 4) == 0xc;
    if (b)
        b = *(int *)(p + 8) < *(int *)(p + 0xfc);
    if (b) {
        FUN_001c6354(self);
        return;
    }

    if (*(unsigned char *)(p + 0x61) != 0) {
        if (*(unsigned char *)(p + 0x61) != 0) {
            if (*(unsigned char *)(p + 0x62) != 0) {
                FUN_000503d0(DAT_00311170, 3999.89990234375f);
                *(unsigned char *)(p + 0x62) = 0;
            } else {
                int tex = FUN_00050534(DAT_00311170);
                if (tex != 0) {
                    unsigned char col[4];
                    int i;
                    int packed;
                    float y;

                    FUN_00051398(tex, 0.0f);
                    col[0] = 128;
                    col[1] = 128;
                    col[2] = 128;
                    col[3] = 128;
                    y = 0.0f;
                    FUN_00051490(tex, 0, 0, 0x1e0, 0x110, 0, 0);
                    packed = *(int *)col;
                    for (i = 0; i < 4; i++) {
                        FUN_00051894((void *)tex, y, 0.0f, y, 0.0f,
                                     y + 120.0f, 272.0f, y + 120.0f, 272.0f,
                                     0.0f, packed);
                        y = y + 120.0f;
                    }
                    FUN_0005185c(tex);
                }
            }
        }
    }
    mode = *(int *)(p + 4);

    switch (mode) {
    case 2:
        if (*(unsigned char *)(p + 0x63) != 0)
            (*(Disp_1c6608 **)(p + 0x20))->v3();
        break;
    case 0xc:
        if (DAT_0034a81c != 0) {
            if (*(int *)(p + 8) >= *(int *)(p + 0xfc))
                FUN_002917ac(DAT_0034a81c);
        }
        break;
    }
}
