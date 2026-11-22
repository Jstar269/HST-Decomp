/* HSTMATCH module=EBOOT.BIN start=0x001dd0a4 end=0x001dd1c0 symbol=FUN_001dd0a4 lang=c++ */
struct Entry {
    int idx;
    int arg;
    int out;
};
extern "C" char DAT_002eba1c[];
extern "C" char DAT_00309460[];
extern "C" float DAT_002eb90c;
extern "C" int DAT_002eb930[3];
extern "C" struct Entry DAT_002eb998[11];
extern "C" int DAT_00338c20;
struct Menu {
    int vt;
    char pad[0x50];
    int tex[3];
    int obj[11];
    int last;
};
extern "C" void FUN_00102e38(Menu *);
extern "C" void FUN_00104118(Menu *, char *, int);
extern "C" void FUN_00103540(float, void *);
extern "C" int FUN_001039d8(Menu *, int, int);
extern "C" int FUN_001026b8(int, int);
extern "C" void FUN_000ffc3c(int, int);
extern "C" int FUN_000fb874(int);

#pragma opt_strength_reduction off
extern "C" Menu *FUN_001dd0a4(Menu *self)
{
    FUN_00102e38(self);
    self->vt = (int)DAT_00309460;
    FUN_00104118(self, DAT_002eba1c, 0);
    FUN_00103540(DAT_002eb90c, self);
    {
    int i;
    const int *src = DAT_002eb930;
    int off;
    for (i = 0, off = 0; i < 3; i++, off += 4) {
        *(int *)((char *)self + off + 0x54) = FUN_001039d8(self, *src, 0);
        src++;
    }
    }
    {
    int i;
    Entry *e = DAT_002eb998;
    int off;
    for (i = 0, off = 0; i < 11; i++, off += 4) {
        self->obj[i] = FUN_001026b8(self->tex[e->idx], e->arg);
        FUN_000ffc3c(self->obj[i], 0xffffff00);
        e->out = *(int *)(self->obj[i] + 0x24);
        e++;
    }
    }
    self->last = FUN_000fb874(DAT_00338c20);
    return self;
}
// NEAR (70/71 words, 35 differ): 4 saved regs and loop 1 (separate byte-offset IV s0, source pointer s1, counter s2) match
// retail with #pragma opt_strength_reduction off and explicit `off += 4`. Loop 2 differs: MWCC CSEs self+i*4+0x60 into a
// saved pointer (addiu s0,v0,96) where retail recomputes `addu v0,s3,s0; lw 96(v0)` after the call and stores via
// `addu v1,s3,s0; sw 96(v1)` with off as an IV. Ruled out: off-IV with (char*)/(int) casts (CSE'd pointer, 5 saved regs
// with an int* self), slot pointer local, ((int*)(self+off))[0x18] spellings.
// improved by lane api-hst-20261121 (unreviewed): 60 -> 35 differing words
