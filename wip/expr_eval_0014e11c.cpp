/* HSTMATCH module=EBOOT.BIN start=0x0014e11c end=0x0014e49c symbol=FUN_0014e11c lang=c++ helpers=FUN_0014c610@0x0014c610-0x0014c63c */

struct Token {
    virtual ~Token();
    virtual void v1();
    virtual void v2();
    virtual int Eval(int ctx);
    int kind;
};

struct NodeBase {
    struct Node *prev;
    struct Node *next;
};

struct Node : NodeBase {
    Token *value;
};

struct List {
    unsigned size;
    NodeBase end_;
};

struct It {
    NodeBase *node;
};

struct Runner {
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual const char *Name();
    int f4;
    List list;
};

struct IntNodeBase {
    struct IntNode *prev;
    struct IntNode *next;
};

struct IntNode : IntNodeBase {
    int value;
};

struct IntList {
    int size;
    IntNodeBase end_;
};

struct Ctx {
    int arg;
    char pad4[0x18 - 4];
    IntList stack;
};

struct IntIt {
    IntNodeBase *node;
};

extern unsigned char DAT_003402f0;
extern char DAT_002dc240[];
extern char DAT_002dc24c[];
extern char DAT_002dc31c[];
extern char DAT_002dc324[];
extern int DAT_002dc134[];

extern "C" {
static void FUN_0014c610(...) {}
void FUN_0014f43c(IntIt *, IntList *, IntIt *, int *);
void FUN_0014f6cc(List *, List *);
void FUN_0014f064(List *, Node *, NodeBase *);
void FUN_0014f6e4(List *);
void FUN_0014f540(It *, List *, It *, It *);
Token *FUN_0014ba88(Token *, Token *, Token *, int);
void FUN_0014f5e0(It *, List *, It *, Token **);

int FUN_0014e11c(Runner *self, float f, Ctx *ctx)
{
    List tmp;
    List out;
    Token *n;
    It eret;
    It efirst;
    It elast;
    It iret;
    It oret;
    It oend;
    int two;
    It ipos;
    int zero;
    int one;
    IntIt r1;
    IntIt p1;
    IntIt r2;
    IntIt p2;
    IntIt r3;
    IntIt p3;
    It d1ret;
    It d1first;
    It d1last;
    It d2ret;
    It d2first;
    It d2last;

    int v;
    if (ctx->stack.size > 0)
        v = ctx->stack.end_.prev->value;
    else
        v = 0;
    if (v == 1 || v == 2) {
        two = 2;
        p1.node = &ctx->stack.end_;
        FUN_0014f43c(&r1, &ctx->stack, &p1, &two);
        return 0;
    }
    if (DAT_003402f0 != 0) {
        if (DAT_003402f0 != 0) {
            FUN_0014c610(DAT_002dc240);
            for (int i = 0; i < ctx->stack.size + 1; i++)
                FUN_0014c610(DAT_002dc24c);
        }
        FUN_0014c610(self->Name());
    }
    FUN_0014f6cc(&tmp, &self->list);
    FUN_0014f064(&tmp, self->list.end_.next, &self->list.end_);
    FUN_0014f6e4(&out);
    while (tmp.size >= 2) {
        Node *m = tmp.end_.next;
        if (m != (Node *)&tmp.end_) {
            Node *i = m->next;
            while (i != (Node *)&tmp.end_) {
                if (DAT_002dc134[m->value->kind] < DAT_002dc134[i->value->kind])
                    m = i;
                i = i->next;
            }
        }
        efirst.node = m->prev;
        elast.node = m->next->next;
        Node *p = (Node *)efirst.node;
        Token *a = p->value;
        p = p->next;
        Token *op = p->value;
        p = p->next;
        Token *b = p->value;
        FUN_0014f540(&eret, &tmp, &efirst, &elast);
        n = FUN_0014ba88(a, op, b, ctx->arg);
        ipos = eret;
        FUN_0014f5e0(&iret, &tmp, &ipos, &n);
        oend.node = &out.end_;
        FUN_0014f5e0(&oret, &out, &oend, &n);
    }
    if (tmp.end_.next->value->Eval(ctx->arg) != 0) {
        zero = 0;
        p2.node = &ctx->stack.end_;
        FUN_0014f43c(&r2, &ctx->stack, &p2, &zero);
        if (DAT_003402f0 != 0)
            FUN_0014c610(DAT_002dc31c);
    } else {
        one = 1;
        p3.node = &ctx->stack.end_;
        FUN_0014f43c(&r3, &ctx->stack, &p3, &one);
        if (DAT_003402f0 != 0)
            FUN_0014c610(DAT_002dc324);
    }
    for (Node *q = out.end_.next; q != (Node *)&out.end_; q = q->next) {
        if (q->value)
            delete q->value;
    }
    d1last.node = &out.end_;
    d1first.node = out.end_.next;
    FUN_0014f540(&d1ret, &out, &d1first, &d1last);
    d2last.node = &tmp.end_;
    d2first.node = tmp.end_.next;
    FUN_0014f540(&d2ret, &tmp, &d2first, &d2last);
    return 0;
}
}
// NEAR 223 words (lane w4f2, 2026-11-10); 224/224 words, everything off by frame layout.
// Control flow and calls follow retail; the blocker is the frame: retail 224 bytes, saves s0-s5 (ctx in s5,
//   &tmp.end_ in s1 and &out.end_ in s0 hoisted for the loop), ours 160 bytes / s0-s3.
// Retail slot map (sp+): tmp 212 (size,end.prev,end.next = 12 bytes: the sentinel has no value field),
//   out 200, n 184, eret 172, efirst 168, elast 164, iret 160, oret 140, oend 136, two 132, ipos 120,
//   zero 116, one 112, push2 ret/pos 108/104, push0 100/96, push1 92/88, out-erase ret/first/last 84/80/76,
//   tmp-erase 72/68/64. Unexplained gaps: 188-199 (12), 176-183 (8), 144-159 (16), 124-131 (8), 32-63 (32)
//   -- most likely inline STL helpers (max_element/push_back/front/erase taking iterators and an empty
//   comparator or allocator by value) whose address-taken parameters keep frame slots. Next step: rebuild the list
//   as an inline class (begin()/end()/push_back/erase/max_element by value) instead of explicit locals.
// Also seen: `v == 1 || v == 2` is two value compares (xori/sltiu, bnez then beqzl) in retail; the main
//   loop has a duplicated size>=2 guard before the rotated body.
// near miss from lane w4f2 (unreviewed)
