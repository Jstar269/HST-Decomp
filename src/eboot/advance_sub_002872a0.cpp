/* HSTMATCH module=EBOOT.BIN start=0x002872a0 end=0x00287418 symbol=FUN_002872a0 lang=c++ */
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

extern "C" int FUN_00287418(void);
extern "C" void FUN_0014cd00(Sub *);
extern "C" void FUN_0014c214(StackObj *, void *, int, Obj *);
extern "C" void FUN_0014c914(Sub *, StackObj *);
extern "C" void FUN_0014c820(StackObj *, int);
extern "C" unsigned char FUN_0014c9b8(float, Sub *, int);
extern "C" unsigned char DAT_003403f4;

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

extern "C" void FUN_002872a0(Obj *o)
{
    if (o->f57 && !o->f5a && o->f54 && o->f3c >= 0 && FUN_00287418() == 0) {
        if (o->f56) {
            if (is_nonempty(o->p12)) {
                FUN_0014cd00(o->p12);
                o->f56 = 0;
            }
        }
        if (!is_nonempty(o->p12)) {
            if (o->f4 != 0) {
                StackObj st;
                FUN_0014c214(&st, o->f4, o->f3c, o);
                FUN_0014c914(o->p12, &st);
                FUN_0014c820(&st, -1);
            }
        }
        unsigned char old = DAT_003403f4;
        DAT_003403f4 = 0;
        o->f58 = 1;
        FUN_0014c9b8(1.0f, o->p12, 0);
        o->f58 = 0;
        DAT_003403f4 = old;
    }
}
