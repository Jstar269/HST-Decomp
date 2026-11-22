/* HSTMATCH module=EBOOT.BIN start=0x00287de4 end=0x00287ff0 symbol=FUN_00287de4 lang=c++ */
struct Elem {
    float a, b, c, d;
};
struct False {};
struct bidir_tag {};
struct ra_tag : bidir_tag {};
struct It {
    Elem *p;
    It() {}
    It(Elem *q) : p(q) {}
    It operator-(int n) const { return It(p - n); }
};
struct Vec;
struct Alloc {
    void construct(Elem *p, const Elem &x)
    {
        if (p != 0)
            *p = x;
    }
};
extern "C" {
void FUN_0012ca08(Vec *v, const Elem *x, False);
void FUN_00289330(It first, It middle, It last, ra_tag);
void FUN_000e88e4(Vec *v, int flag);
}
struct Vec {
    Elem *data;
    unsigned size;
    unsigned cap;
    void push_back(const Elem &x)
    {
        if (size < cap) {
            Alloc().construct(data + size, x);
            ++size;
        } else {
            FUN_0012ca08(this, &x, False());
        }
    }
    It begin() { It r; r.p = data; return r; }
    It end() { It r; r.p = data + size; return r; }
};
extern "C" void FUN_001cb464(Vec *v, void *g, int key);

inline void rotate(It first, It middle, It last)
{
    if (first.p == middle.p || middle.p == last.p)
        return;
    FUN_00289330(first, middle, last, ra_tag());
}
inline const int &max(const int &a, const int &b) { return a < b ? b : a; }

struct Cmd {
    int code;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
};
struct LNode;
struct LIt {
    LNode *n;
    LIt(LNode *p) : n(p) {}
    LIt(const LIt &o) : n(o.n) {}
};
struct CmdList {
    int pad;
    LIt end() { return LIt((LNode *)((char *)this + 4)); }
    void push_back(const Cmd &c);
};
extern "C" LIt FUN_00289544(CmdList *l, LIt pos, const Cmd &c);
inline void CmdList::push_back(const Cmd &c) { FUN_00289544(this, end(), c); }

struct Obj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual Elem *Item();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void Select(Elem *e, int n, int f);
    char pad[0x3c];
    int sel;
    char pad2[0x20];
    CmdList cmds;
};

extern void *DAT_0034a114;
extern int DAT_002fea14;

extern "C" void FUN_00287de4(Obj *self, int key, int hi, int defer)
{
    if (defer < 1) {
        Vec v;
        FUN_001cb464(&v, DAT_0034a114, key);
        int cur = self->sel;
        int lo = cur;
        int count = v.size;
        if (hi < 0)
            hi = count - 1;
        if (!(cur < count && hi < count)) {
            FUN_000e88e4(&v, 0);
            return;
        }
        {
            if (hi < cur) {
                lo = hi;
                hi = cur;
            }
            if (lo < 0) {
                v.push_back(*self->Item());
                It m(v.data + v.size - 1); rotate(v.begin(), m, v.end());
            }
            int n = hi - lo + 1;
            lo = max(lo, DAT_002fea14);
            if (v.size != 0)
                self->Select(&v.data[lo], n, 0);
            self->sel = hi;
            FUN_000e88e4(&v, 0);
        }
    } else {
        Cmd c;
        c.code = 0x2f;
        c.a = defer;
        c.b = key;
        c.c = hi;
        self->cmds.push_back(c);
    }
}
// NEAR MISS (ol1 lane): 131/131 words, 13 differ.
// Differs: (1) rotate block slots: ours begin 64, extra temp 60, end 56, first 44, last 40, tag 36;
//   retail begin 64, end 60, first 56, (gap 52 = middle param?), last 48, tag 44, gap 40.
//   The middle (end()-1) must reach rotate with no caller-level temp; ruled out: by-value middle
//   via member/free operator- (const& or by value), It(end().p-1), named It e=end(), t(*this)-=n forms.
// (2) Select(&v.data[lo]) loads data before sll lo (retail sll first, regs v0/v1 swapped); &v[lo], &*(begin()+lo) same.
// (3) else-branch insert ret/it slots 4 lower (consequence of (1)).
// Fixed already: Alloc().construct temp gives the 16-byte frame; Cmd is 28 bytes; cur/lo split; const& middle.
// near miss from lane ol1 (unreviewed)
// lane api-hstnear2-20261121 (13 -> 11): rotate by value `(It first, It middle, It last)` with a NAMED middle
// `It m(v.data + v.size - 1); rotate(v.begin(), m, v.end());` keeps end() at 60 like retail, but the named m (56) + by-value copy
// shift first/last/tag 8 lower (ours 48/40/36 vs retail 56/48/44). By-value with a temporary (`It(v.end().p-1)`, `v.end()-1`,
// friend/member operator- spellings) gives 136 words (copy stored through memory). Extra `False` tag params on rotate: no change.
// Left: the 3 slot groups (11 words) and Select's `sll v0,v1,4 / lw v1,100(sp) / addu a1,v1,v0` operand order (not retried).
// improved by lane api-hstnear2-20261121 (unreviewed): 13 -> 11 differing words
