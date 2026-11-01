/* HSTMATCH module=hstp_trans_main.prx start=0x0029899c end=0x00298b14 symbol=FUN_0029899c lang=c++ */
/* Clone of src/eboot/advance_sub_002872a0.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Node {
    int pad;
    int sub;
};

struct Sub {
    char pad0[4];
    Node *f4;
    char pad1[0xc];
    int *f14;
};

struct Obj {
    char pad0[4];
    void *f4;
    char pad1[4];
    Sub *p12;
    char pad2[0x2c];
    int f3c;
    char pad3[0x14];
    unsigned char f54;
    char pad4[1];
    unsigned char f56;
    unsigned char f57;
    unsigned char f58;
    char pad5[1];
    unsigned char f5a;
};

struct StackObj {
    char data[36];
};

extern "C" int FUN_00298b14(void);
extern "C" void FUN_001587e8(Sub *);
extern "C" void FUN_00157be4(StackObj *, void *, int, Obj *);
extern "C" void FUN_001583fc(Sub *, StackObj *);
extern "C" void FUN_00158308(StackObj *, int);
extern "C" unsigned char FUN_001584a0(float, Sub *, int);
extern "C" unsigned char DAT_003e9734;

static inline int is_nonempty(Sub *s) {
    Node *n = s->f4;
    int ok = 0;
    if (n != 0) {
        int e = (n != 0) ^ 1;
        int b;
        if (e == 0) b = (s->f14 == &n->sub) ^ 1; else b = e ^ 1;
        if (b) ok = 1;
    }
    return ok;
}

extern "C" void FUN_0029899c(Obj *o)
{
    if (o->f57 && !o->f5a && o->f54 && o->f3c >= 0 && FUN_00298b14() == 0) {
        if (o->f56) {
            if (is_nonempty(o->p12)) {
                FUN_001587e8(o->p12);
                o->f56 = 0;
            }
        }
        if (!is_nonempty(o->p12)) {
            if (o->f4 != 0) {
                StackObj st;
                FUN_00157be4(&st, o->f4, o->f3c, o);
                FUN_001583fc(o->p12, &st);
                FUN_00158308(&st, -1);
            }
        }
        unsigned char old = DAT_003e9734;
        DAT_003e9734 = 0;
        o->f58 = 1;
        FUN_001584a0(1.0f, o->p12, 0);
        o->f58 = 0;
        DAT_003e9734 = old;
    }
}
