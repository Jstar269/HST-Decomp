/* HSTMATCH module=EBOOT.BIN start=0x002b4060 end=0x002b40b0 symbol=FUN_002b4060 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Rec {
    int w00;
    char a04[32];
    char a24[32];
    char c44, c45, c46;
    unsigned char b47, b48;
    char c49, c4a, c4b;
    int w4c, w50;
    char a54[16];
};

struct It {
    Rec *p;
    It() {}
    It(Rec *q) : p(q) {}
};

extern "C" Rec *FUN_002b4194(Rec *, const Rec *);
extern "C" Rec *FUN_002b40b0(Rec *, const Rec *);

extern "C" void FUN_002b4060(It a, It b)
{
    Rec tmp;
    FUN_002b4194(&tmp, a.p);
    FUN_002b40b0(a.p, b.p);
    FUN_002b40b0(b.p, &tmp);
}
