/* HSTMATCH module=EBOOT.BIN start=0x002ab5ac end=0x002ab724 symbol=FUN_002ab5ac lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Str {
    char d[12];
};

struct Rep {
    int a, b, c;
};

extern "C" {
void FUN_0001f06c(void *p);
void FUN_002abb70(void *p, int n);
void *FUN_00000bcc(unsigned int sz);
void FUN_00198df0(void *p);
void FUN_00198eb4(void *p);
void FUN_000031f0(Str *dst, const char *src);
void FUN_00003e04(Str *dst, const Str *src);
void FUN_00190880(Rep *dst, const Rep *src);
void FUN_000462a0(Rep *p, int n);
void FUN_00004138(Str *p, int n);
void FUN_0001f21c(void *a, void *b);

extern void *DAT_00309efc;
extern void *DAT_00309194;
extern const char DAT_00300a90[];
extern const char DAT_002cc2b0[];
}

struct StrPair {
    Str s;
    Rep r;
    StrPair(const char *lit)
    {
        FUN_000031f0(&s, lit);
        int *p = &r.a;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    ~StrPair()
    {
        FUN_000462a0(&r, -1);
        FUN_00004138(&s, 0);
    }
    StrPair &operator=(const StrPair &o)
    {
        FUN_00003e04(&s, &o.s);
        FUN_00190880(&r, &o.r);
        return *this;
    }
};

struct SubSubObj {
    char pad[0x1c];
    const void *f1c;
    char pad2[0x24 - 0x20];
    int f24;
    int *f28;
    char pad3[0x30 - 0x2c];
    int f30;
};

struct SubClass {
    void *vtable;
    char pad[0x38];
    int f3c;
    char pad70[0x70 - 0x40];
    SubSubObj *f70;
    char pad88[0x88 - 0x74];
};

struct TargetObj {
    void *vtable;
    char pad3c[0x3c - 4];
    int f3c;
    int f40;
    int f44;
    int f48;
    int f4c;
    int f50;
    int f54;
    int f58;
    int f5c;
    int f60;
    int f64;
    int f68;
    int f6c;
    int f70;
    StrPair pair74;
    SubClass *f8c;
    int f90;
};

extern "C" TargetObj *FUN_002ab5ac(TargetObj *self)
{
    SubClass *s1;

    FUN_0001f06c(self);
    self->vtable = &DAT_00309efc;
    self->f3c = 0;
    self->f40 = 0;
    self->f44 = 0;
    self->f48 = 0;
    self->f4c = 5;
    self->f54 = 0;
    self->f5c = 0;
    self->f60 = 0;
    self->f64 = 0;
    self->f68 = 0;
    self->f6c = 0;
    self->f70 = 0;
    *(int *)((char *)self + 116) = 0;
    *(int *)((char *)self + 120) = 0;
    *(int *)((char *)self + 124) = 0;
    *(int *)((char *)self + 128) = 0;
    *(int *)((char *)self + 132) = 0;
    *(int *)((char *)self + 136) = 0;
    self->f8c = 0;

    FUN_002abb70(self, 0);
    FUN_002abb70(self, 1);

    if ((s1 = (SubClass *)FUN_00000bcc(136)) != 0) {
        FUN_0001f06c(s1);
        s1->vtable = &DAT_00309194;
        FUN_00198df0(&s1->f3c);
        FUN_00198eb4(s1);
    }
    self->f8c = s1;

    self->pair74 = StrPair(DAT_00300a90);

    self->f8c->f70->f1c = &DAT_002cc2b0;
    self->f8c->f70->f24 = 74;
    self->f8c->f70->f28 = &self->f90;
    self->f8c->f70->f30 = 24;

    FUN_0001f21c(self, self->f8c);

    return self;
}
