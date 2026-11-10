/* HSTMATCH module=EBOOT.BIN start=0x00191194 end=0x001915f4 symbol=FUN_00191194 lang=c++ */
#pragma inline_depth(8)
extern "C" {
struct Reent_00191194 { int pad[3]; void *err; };
Reent_00191194 *FUN_0000fe3c(void);
int FUN_0000f3e8(void *stream, const char *format, ...);
void FUN_0000d518(void);
void *FUN_00000bcc(unsigned size);
extern char DAT_002c27a4[]; /* "Memory allocation failure" */
}
typedef void *Block;
struct Buf_00191194;
struct Iter_00191194 {
    Block *ptr;
    Block *beg;
    Block *first;
    Block *end;
    Iter_00191194(Buf_00191194 *c, unsigned i);
    Iter_00191194(const Iter_00191194 &o) : ptr(o.ptr), beg(o.beg), first(o.first), end(o.end) {}
    Iter_00191194 &operator+=(int n)
    {
        int cap = end - beg;
        n += ptr - beg;
        if (n < 0)
            n += cap;
        else if (n >= cap)
            n -= cap;
        ptr = beg + n;
        return *this;
    }
    Iter_00191194 &operator-=(int n) { return *this += -n; }
    Iter_00191194 operator-(int n) const { return Iter_00191194(*this) -= n; }
};
struct Buf_00191194 {
    unsigned cap_;
    unsigned size_;
    unsigned start_;
    Block *data_;
    unsigned capacity() const { return cap_ == 0 ? 0 : cap_ - 1; }
    unsigned size() const { return size_; }
    Iter_00191194 begin() { return Iter_00191194(this, start_); }
    Iter_00191194 end() { return Iter_00191194(this, cap_ != 0 ? (start_ + size_) % cap_ : 0); }
    Block &front() { return data_[start_]; }
    void pop_front()
    {
        --size_;
        if (++start_ == cap_)
            start_ = 0;
    }
    Block &back() { return data_[(start_ + size_ - 1) % cap_]; }
};
inline Iter_00191194::Iter_00191194(Buf_00191194 *c, unsigned i)
    : ptr(c->data_ + i), beg(c->data_), first(c->data_ + c->start_), end(c->data_ + c->cap_) {}
struct IterMem_00191194 { Block *w[4]; };
struct Tag_00191194 {};
struct Deque_00191194 {
    Buf_00191194 buf_;
    unsigned start_;
    unsigned size_;
};
template <class T> inline const T &max_00191194(const T &a, const T &b) { return a < b ? b : a; }
struct Alloc_00191194 {
    Block allocate()
    {
        Block p = FUN_00000bcc(512);
        if (p == 0) {
            FUN_0000f3e8(FUN_0000fe3c()->err, DAT_002c27a4);
            FUN_0000d518();
        }
        return p;
    }
};
extern "C" {
void FUN_001925f0(Buf_00191194 *self, const Block &x);                       /* push_back */
void FUN_00192834(Buf_00191194 *self, unsigned n);                           /* reserve */
void FUN_00190f50(Buf_00191194 *self);                                       /* clear */
void FUN_001925a0(Buf_00191194 *a, Buf_00191194 *b);                         /* swap */
Deque_00191194 *FUN_00190ba0(Deque_00191194 *self, short flag);              /* ~Deque */
Alloc_00191194 *FUN_00191838(Alloc_00191194 *ret, Deque_00191194 *self);    /* get_allocator */
Buf_00191194 *FUN_00191820(Buf_00191194 *self, Alloc_00191194 *a);          /* Buf(alloc) */
Buf_00191194 *FUN_001916a4(Buf_00191194 *self, Deque_00191194 *owner, Buf_00191194 *x); /* Buf(x) */
Buf_00191194 *FUN_00190400(Buf_00191194 *self, short flag);                  /* ~Buf */
IterMem_00191194 *FUN_0019167c(IterMem_00191194 *dst, const Iter_00191194 *src);   /* Iter(const Iter &) */
void FUN_001915f4(Buf_00191194 *self, IterMem_00191194 *pos, IterMem_00191194 *first, IterMem_00191194 *last, Tag_00191194); /* insert(pos, first, last) */
}
inline void InitTmp_00191194(Deque_00191194 *tmp, Deque_00191194 *self)
{
    Alloc_00191194 al;
    Buf_00191194 tb0;
    FUN_00191820(&tb0, FUN_00191838(&al, self));
    FUN_001916a4(&tmp->buf_, self, &tb0);
    FUN_00190400(&tb0, -1);
}
inline void CallInsert_00191194(Buf_00191194 *t, Iter_00191194 &tbeg, Iter_00191194 &sbeg, Iter_00191194 &send)
{
    IterMem_00191194 c1, c2, c3;
    FUN_001915f4(t, FUN_0019167c(&c1, &tbeg), FUN_0019167c(&c2, &sbeg), FUN_0019167c(&c3, &send), Tag_00191194());
}
inline void CallInsertV2_00191194(Buf_00191194 *t, Iter_00191194 &a, Iter_00191194 &b, Iter_00191194 &c) { CallInsert_00191194(t, a, b, c); }
inline void CopyBlocks_00191194(Buf_00191194 *t, Buf_00191194 *src)
{
    if (src->size() != 0) {
        Iter_00191194 tbeg = t->begin();
        Iter_00191194 sbeg = src->begin();
        Iter_00191194 send = src->end() - 1;
        CallInsertV2_00191194(t, tbeg, sbeg, send);
    }
}
inline void InitTmpV2_00191194(Deque_00191194 *tmp, Deque_00191194 *self) { InitTmp_00191194(tmp, self); }
inline void InitTmpV3_00191194(Deque_00191194 *tmp, Deque_00191194 *self) { InitTmpV2_00191194(tmp, self); }
extern "C" void FUN_00191194(Deque_00191194 *self, unsigned n)
{
    Deque_00191194 tmp;
    int blocks = (n >> 7) + ((n & 0x7f) != 0);
    for (; ((blocks != 0) ^ 1) == 0 && self->start_ >= 128; --blocks) {
        Block b = self->buf_.front();
        self->buf_.pop_front();
        self->buf_.back() = b;
        FUN_001925f0(&self->buf_, Block());
        self->start_ -= 128;
    }
    Alloc_00191194 a;
    unsigned sz = self->buf_.size_;
    unsigned total = blocks + sz;
    if (total <= self->buf_.capacity()) {
        if (blocks == 0)
            return;
        do {
            Block *p = &self->buf_.back();
            *p = a.allocate();
            FUN_001925f0(&self->buf_, Block());
        } while (--blocks != 0);
        return;
    }
    InitTmpV3_00191194(&tmp, self);
    tmp.start_ = 0;
    tmp.size_ = 0;
    Buf_00191194 *t = &tmp.buf_;
    FUN_00192834(t, max_00191194(self->buf_.capacity() << 1, blocks + self->buf_.size_ + 1));
    FUN_001925f0(t, Block());
    if (blocks != 0) {
        do {
            Block *p = &t->back();
            *p = a.allocate();
            FUN_001925f0(t, Block());
        } while (--blocks != 0);
    }
    CopyBlocks_00191194(t, &self->buf_);
    FUN_001925a0(&self->buf_, t);
    FUN_00190f50(t);
    FUN_00190ba0(&tmp, -1);
}
// NEAR MISS 6 words (opus1 2026-11-09): EBOOT copy of the Sharing twin wip/deque_grow_back_0019d9a4.cpp (bodies identical
// apart from addresses; callees 19cc10->190400, 19d3b0->190ba0, 19d760->190f50, 19de04->1915f4, 19de8c->19167c,
// 19deb4->1916a4, 19e030->191820, 19e048->191838, 19edb0->1925a0, 19ee00->1925f0, 19f044->192834, d530->d518,
// DAT_002d2ca4->DAT_002c27a4). Same 6 colour words as wip/deque_grow_back_0011e790.cpp.
// Measured: with the test written inline (`size()+blocks <= capacity()`, `capacity() >= ...`) MWCC evaluates capacity()
// FIRST and colours cap_ v1 / result a0 / size a1 / sum v1 (9-10): the retail colours are exactly that pattern applied
// with the size/sum chain evaluated first and NOT coalesced in place, which no named-local form gives (sz/total in
// place a0, cap/result in place v1). Also tried: capacity() as `cap_ ? cap_-1 : 0` (10), if/return (6), r-local (220),
// `!=` ternary (10); const/register locals (6); assignment inside the test (226); `!(cap < sum)` / `!(sum > cap)` (226);
// cap named after total (6/8).
// near miss from lane opus1 (unreviewed)
