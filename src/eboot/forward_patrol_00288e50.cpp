/* HSTMATCH module=EBOOT.BIN start=0x00288e50 end=0x00288f2c symbol=FUN_00288e50 lang=c++ */
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
    char pad2[0x20];
    int f30;
    int f34;
    int f38;
    int f3c;
};

struct StackObj {
    char data[36];
};

extern "C" void FUN_0014c214(StackObj *, void *, int, Obj *);
extern "C" void FUN_0014c914(Sub *, StackObj *);
extern "C" void FUN_0014c820(StackObj *, int);
extern "C" int FUN_0014ce38(Sub *, int);
extern "C" void FUN_00286acc(...);
extern char DAT_002feb30[];

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

extern "C" void FUN_00288e50(Obj *o, int n)
{
    if (n > 0) {
        if (!is_nonempty(o->p12)) {
            if (o->f4 != 0) {
                StackObj st;
                FUN_0014c214(&st, o->f4, o->f3c, o);
                FUN_0014c914(o->p12, &st);
                FUN_0014c820(&st, -1);
            }
        }
        if (FUN_0014ce38(o->p12, n) == 0) {
            FUN_00286acc(DAT_002feb30, o->f30, o->f34, o->f38, n);
        }
    }
}
