/* HSTMATCH module=EBOOT.BIN start=0x001e1964 end=0x001e1c40 symbol=FUN_001e1964 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* steer=pragma:opt_lifetimes (off, scoped to FUN_001e1964 by #pragma push/pop; found by the 7-decomp pragma sweep; no natural source matched) */
/* Hand-written reconstruction from the disassembly; EXACT with #pragma opt_lifetimes off for this function; names and types await human review. */
struct Entry { char pad[24]; short v; char pad2[0x120 - 26]; };
struct Game { char pad[0x3cc]; int f3cc; int f3d0; };
struct Tmp { int a, b; };
struct Rect { short pad0, pad2; short s4, s6, s8, s10; };

class Widget {
public:
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual void m6();
    virtual void m7();
    virtual void Set(int, int, int, int, int);
};

extern Entry DAT_003417b8[];
extern char DAT_002ebcf4[];
extern char DAT_002ebd24[];
extern char DAT_002ebd28[];
extern char DAT_002ebd38[];
extern char DAT_002ebd3c[];
extern char DAT_002ebd44[];
extern char DAT_002ebd54[];
extern char DAT_002ebd58[];
extern char DAT_002ebb74[];
extern char DAT_002c7068[];

extern "C" {
Game *FUN_001ee200();
void *FUN_00000bcc(int);
void *FUN_000649c0(void *);
void FUN_00064bf4(void *, void *, int, void *);
void *FUN_0006517c(void *, int, int);
void FUN_00014934(void *, void *);
void *FUN_00014d64(void *, void *);
void FUN_000143b0(Tmp *, void *, int);
void FUN_000111c0(void *, Tmp *, int);
void FUN_001041dc(void *, void *, void *, int);
void FUN_00103540(void *, float);
void FUN_001037c8(void *, int);
void FUN_00216e90(void *, void *, void *, int);
void FUN_00216f14(void *, void *, void *, int);
void FUN_00217778(void *, void *);
void FUN_00217614(void *, void *);

#pragma push
#pragma opt_lifetimes off
void FUN_001e1964(char *o)
{
    void *t;
    void *u;
    void *p;
    int a = 0;
    int b = 0;
    if (FUN_001ee200()->f3cc != -1) {
        a = ((Entry *)((char *)DAT_003417b8 + FUN_001ee200()->f3cc * 0x120))->v;
    }
    if (FUN_001ee200()->f3d0 != -1) {
        b = ((Entry *)((char *)DAT_003417b8 + FUN_001ee200()->f3d0 * 0x120))->v;
    }
    p = FUN_00000bcc(16);
    if (p != 0) {
        p = FUN_000649c0(p);
    }
    FUN_00064bf4(p, DAT_002ebcf4, 1, DAT_002ebd24);
    t = FUN_0006517c(p, 2, 0);
    FUN_00014934(t, DAT_002ebd28);
    t = FUN_00014d64(t, DAT_002ebd38);
    Tmp s1;
    FUN_000143b0(&s1, DAT_002ebd3c, a);
    FUN_000111c0(t, &s1, 3);
    u = FUN_0006517c(p, 3, 0);
    FUN_00014934(u, DAT_002ebd44);
    u = FUN_00014d64(u, DAT_002ebd54);
    Tmp s2;
    FUN_000143b0(&s2, DAT_002ebd3c, b);
    FUN_000111c0(u, &s2, 3);
    FUN_001041dc(*(void **)(o + 0x1f4), p, DAT_002ebd58, 0);
    FUN_00103540(*(void **)(o + 0x1f4), 2900.0f);
    FUN_001037c8(*(void **)(o + 0x1f4), -256);
    FUN_00216e90(o + 0x1f8, *(void **)(o + 0x1f4), DAT_002ebb74, 5);
    FUN_00216f14(o + 0x20c, o + 0x1f8, DAT_002c7068, 20);
    *(int *)(o + 0x39c) = (*(Rect **)(o + 0x360))->s4;
    *(int *)(o + 0x3a0) = (*(Rect **)(o + 0x360))->s6;
    *(int *)(o + 0x3a4) = (*(Rect **)(o + 0x360))->s8;
    *(int *)(o + 0x3a8) = (*(Rect **)(o + 0x360))->s10;
    Rect *r = *(Rect **)(o + 0x360);
    (*(Widget **)(o + 0x204))->Set(0, r->s4, r->s6, r->s8, r->s10);
    FUN_00217778(o + 0x3ac, *(void **)(o + 0x1f4));
    FUN_00217614(o + 0x43c, *(void **)(o + 0x2ac));
    FUN_00217614(o + 0x4c8, *(void **)(o + 0x284));
    FUN_00217614(o + 0x554, *(void **)(o + 0x2c0));
    FUN_00217614(o + 0x5e0, *(void **)(o + 0x2d4));
    FUN_00217614(o + 0x66c, *(void **)(o + 0x270));
    FUN_00217614(o + 0x6f8, *(void **)(o + 0x20c));
    FUN_00217614(o + 0x784, *(void **)(o + 0x220));
}
#pragma pop
}

// sn1 NEAR (183/183 words, 42 differ, every difference is the saved-register naming of two values): retail has self=s2, p=s3 (the new+ctor result), t=s4,
// a=s1, b=s0; ours (with `void *t; void *p;` declared first) gives self=s3, p=s2 (t=s4, a=s1, b=s0 match). With p declared where it is first assigned p lands in s0
// (8 more words), a local copy `char *o = arg;` of the parameter does not change anything. The table read is
// `((Entry *)((char *)DAT_003417b8 + idx * 0x120))->v` (indexing `DAT_003417b8[idx].v` folds the +24 into the symbol address: `addiu v0,v0,6096; lh 0(v0)`, retail keeps
// `addiu v0,v0,6072 ... lh 24(v0)`). The 8-byte string-ish temporaries are declared s1 then s2 (first = sp+40).
// sn1 second pass (183/183 words, 13 differ): separate locals for the two Widget chains (`void *t; void *u; void *p;` declared in that order, block 2 uses `u`) give self=s2 (retail), a=s1, b=s0; the only
// remaining difference is p/t: ours p=s4, t=s3, retail p=s3, t=s4. With `p` declared before `t`/`u` (or a single `t`) self drops back to s3 (42 differ). 24 declaration/split permutations tried (grid), only these two states exist.
// near miss from lane sn1 (unreviewed)
