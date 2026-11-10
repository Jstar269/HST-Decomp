/* HSTMATCH module=hstp_trans_main.prx start=0x002c3b20 end=0x002c3b70 symbol=FUN_002c3b20 lang=c++ */
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

extern "C" Rec *FUN_002c3c54(Rec *, const Rec *);
extern "C" Rec *FUN_002c3b70(Rec *, const Rec *);

extern "C" void FUN_002c3b20(It a, It b)
{
    Rec tmp;
    FUN_002c3c54(&tmp, a.p);
    FUN_002c3b70(a.p, b.p);
    FUN_002c3b70(b.p, &tmp);
}
