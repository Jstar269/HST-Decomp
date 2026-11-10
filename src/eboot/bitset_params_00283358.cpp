/* HSTMATCH module=EBOOT.BIN start=0x00283358 end=0x00283660 symbol=FUN_00283358 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern unsigned char DAT_003417b8;
extern unsigned char DAT_003417b9;
extern unsigned char DAT_003417ba;
extern unsigned char DAT_003417bb;
extern unsigned char DAT_003417bc;
extern unsigned char DAT_003417bd;
extern unsigned char DAT_003417c1;
extern unsigned char DAT_003417c2;
extern int DAT_003417c4;
extern unsigned char DAT_003417c8;
extern unsigned char DAT_003417cc;
extern unsigned char DAT_00341c50;
extern unsigned char DAT_00341c51;
extern unsigned char DAT_00341c52;
extern char DAT_00341c54[];
extern short DAT_00341c78;
extern short DAT_00341cbc;
extern short DAT_00341cbe;
extern int DAT_002caab8[];
extern int DAT_002caac4[];
extern char DAT_002c6a78[];
void FUN_0000d518(void);
char *FUN_0000fe3c(void);
void *FUN_000114c0(void *, int, int);
void FUN_002803d8(...);
}

struct Bits {
    unsigned bits;
    Bits(unsigned v) : bits(v) {}
    unsigned char count() const
    {
        unsigned x = bits;
        x = x - ((x >> 1) & 0x55555555);
        x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
        x = (x + (x >> 4)) & 0x0f0f0f0f;
        x = x + (x >> 8);
        x = x + (x >> 16);
        return (unsigned char)x;
    }
    bool test(unsigned pos) const
    {
        if (pos >= 32) {
            FUN_002803d8(*(void **)(FUN_0000fe3c() + 0xc), DAT_002c6a78);
            FUN_0000d518();
        }
        return (bits & (1 << pos)) != 0;
    }
};

struct Param {
    char pad0[0x10];
    int f10;
    char pad14[0x58 - 0x14];
    int f58;
    int f5c;
    int f60;
    int f64;
    int f68;
    int f6c;
    unsigned f70;
    char pad74[0x4d4 - 0x74];
    int f4d4;
    int f4d8;
};

struct Tag {};

static inline void SetPair(int a, int b)
{
    DAT_00341c50 = a;
    DAT_00341c51 = b;
}

extern "C" void FUN_00283358(Param *p)
{
    int i;

    DAT_003417b8 = 0;
    DAT_003417b9 = 1;
    DAT_003417ba = 2;
    DAT_003417bb = 0;
    DAT_003417c1 = 0;
    DAT_003417c2 = 0;
    DAT_003417c8 = 0;
    DAT_003417b8 = p->f5c;
    DAT_003417b9 = DAT_002caab8[p->f64];
    DAT_003417ba = DAT_002caac4[p->f68];
    DAT_003417bb = p->f6c;
    Bits b(p->f70);
    switch (b.count()) {
    case 0:
        DAT_003417c1 = 0;
        DAT_003417c2 = 0;
        break;
    case 1:
        for (i = 0; i < 21; i++) {
            if (b.test(i)) {
                DAT_003417c1 = i + 1;
                break;
            }
        }
        DAT_003417c2 = 0;
        break;
    case 2:
        for (i = 0; i < 21; i++) {
            if (b.test(i)) {
                DAT_003417c1 = ++i;
                break;
            }
        }
        for (; i < 21; i++) {
            if (b.test(i)) {
                DAT_003417c2 = i + 1;
                break;
            }
        }
        break;
    }
    DAT_003417cc = p->f60;
    DAT_00341c50 = 0;
    DAT_00341c51 = 0;
    FUN_000114c0(DAT_00341c54, 0, 0x6c);
    DAT_00341cbc = -1;
    DAT_00341cbe = -1;
    DAT_00341c78 = -1;
    DAT_003417c4 = p->f10;
    DAT_003417bd = p->f58;
    SetPair(p->f4d4, p->f4d8);
    DAT_00341c52 = 0;
    DAT_003417bc = 0;
}
