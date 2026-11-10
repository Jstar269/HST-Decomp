/* HSTMATCH module=EBOOT.BIN start=0x0011e790 end=0x0011ec54 symbol=FUN_0011e790 lang=c++ */
#pragma inline_depth(8)
extern "C" {
struct Reent_0011e790 { int pad[3]; void *err; };
Reent_0011e790 *FUN_0000fe3c(void);
int FUN_0000f3e8(void *stream, const char *format, ...);
void FUN_0000d518(void);
void *FUN_00000bcc(unsigned size);
extern char DAT_002c1370[]; /* "Memory allocation failure" */
}
typedef void *Block;
struct Buf_0011e790;
struct Iter_0011e790 {
    Block *ptr;
    Block *beg;
    Block *first;
    Block *end;
    Iter_0011e790(Buf_0011e790 *c, unsigned i);
    Iter_0011e790(const Iter_0011e790 &o) : ptr(o.ptr), beg(o.beg), first(o.first), end(o.end) {}
    Iter_0011e790 &operator+=(int n)
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
    Iter_0011e790 &operator-=(int n) { return *this += -n; }
    Iter_0011e790 operator-(int n) const { return Iter_0011e790(*this) -= n; }
};
struct Buf_0011e790 {
    unsigned cap_;
    unsigned size_;
    unsigned start_;
    Block *data_;
    unsigned capacity() const { return cap_ == 0 ? 0 : cap_ - 1; }
    unsigned size() const { return size_; }
    void swap(Buf_0011e790 &x)
    {
        if (this != &x) {
            unsigned t;
            Block *d;
            t = cap_; cap_ = x.cap_; x.cap_ = t;
            t = size_; size_ = x.size_; x.size_ = t;
            t = start_; start_ = x.start_; x.start_ = t;
            d = data_; data_ = x.data_; x.data_ = d;
        }
    }
    Iter_0011e790 begin() { return Iter_0011e790(this, start_); }
    Iter_0011e790 end() { return Iter_0011e790(this, cap_ != 0 ? (start_ + size_) % cap_ : 0); }
    Block &front() { return data_[start_]; }
    void pop_front()
    {
        --size_;
        if (++start_ == cap_)
            start_ = 0;
    }
    Block &back() { return data_[(start_ + size_ - 1) % cap_]; }
};
inline Iter_0011e790::Iter_0011e790(Buf_0011e790 *c, unsigned i)
    : ptr(c->data_ + i), beg(c->data_), first(c->data_ + c->start_), end(c->data_ + c->cap_) {}
struct IterMem_0011e790 { Block *w[4]; };
struct Tag_0011e790 {};
struct Deque_0011e790 {
    Buf_0011e790 buf_;
    unsigned start_;
    unsigned size_;
};
template <class T> inline const T &max_0011e790(const T &a, const T &b) { return a < b ? b : a; }
struct Alloc_0011e790 {
    Block allocate()
    {
        Block p = FUN_00000bcc(504);
        if (p == 0) {
            FUN_0000f3e8(FUN_0000fe3c()->err, DAT_002c1370);
            FUN_0000d518();
        }
        return p;
    }
};
extern "C" {
void FUN_0011d894(Buf_0011e790 *self, const Block &x);                       /* push_back */
void FUN_0011dad8(Buf_0011e790 *self, unsigned n);                           /* reserve */
void FUN_0011d7e4(Buf_0011e790 *self);                                       /* clear */
Deque_0011e790 *FUN_0011d190(Deque_0011e790 *self, short flag);              /* ~Deque */
Alloc_0011e790 *FUN_00118f74(Alloc_0011e790 *ret, Deque_0011e790 *self);    /* get_allocator */
Buf_0011e790 *FUN_00118f5c(Buf_0011e790 *self, Alloc_0011e790 *a);          /* Buf(alloc) */
Buf_0011e790 *FUN_00118de0(Buf_0011e790 *self, Deque_0011e790 *owner, Buf_0011e790 *x); /* Buf(x) */
Buf_0011e790 *FUN_00118494(Buf_0011e790 *self, short flag);                  /* ~Buf */
IterMem_0011e790 *FUN_0011ecdc(IterMem_0011e790 *dst, const Iter_0011e790 *src);   /* Iter(const Iter &) */
void FUN_0011ec54(Buf_0011e790 *self, IterMem_0011e790 *pos, IterMem_0011e790 *first, IterMem_0011e790 *last, Tag_0011e790); /* insert(pos, first, last) */
}
inline void InitTmp_0011e790(Deque_0011e790 *tmp, Deque_0011e790 *self)
{
    Alloc_0011e790 al;
    Buf_0011e790 tb0;
    FUN_00118f5c(&tb0, FUN_00118f74(&al, self));
    FUN_00118de0(&tmp->buf_, self, &tb0);
    FUN_00118494(&tb0, -1);
}
inline void CallInsert_0011e790(Buf_0011e790 *t, Iter_0011e790 &tbeg, Iter_0011e790 &sbeg, Iter_0011e790 &send)
{
    IterMem_0011e790 c1, c2, c3;
    FUN_0011ec54(t, FUN_0011ecdc(&c1, &tbeg), FUN_0011ecdc(&c2, &sbeg), FUN_0011ecdc(&c3, &send), Tag_0011e790());
}
inline void CallInsertV2_0011e790(Buf_0011e790 *t, Iter_0011e790 &a, Iter_0011e790 &b, Iter_0011e790 &c) { CallInsert_0011e790(t, a, b, c); }
inline void CopyBlocks_0011e790(Buf_0011e790 *t, Buf_0011e790 *src)
{
    if (src->size() != 0) {
        Iter_0011e790 tbeg = t->begin();
        Iter_0011e790 sbeg = src->begin();
        Iter_0011e790 send = src->end() - 1;
        CallInsertV2_0011e790(t, tbeg, sbeg, send);
    }
}
inline void InitTmpV2_0011e790(Deque_0011e790 *tmp, Deque_0011e790 *self) { InitTmp_0011e790(tmp, self); }
inline void InitTmpV3_0011e790(Deque_0011e790 *tmp, Deque_0011e790 *self) { InitTmpV2_0011e790(tmp, self); }
extern "C" void FUN_0011e790(Deque_0011e790 *self, unsigned n)
{
    Deque_0011e790 tmp;
    int blocks = n / 42 + (n % 42 != 0);
    for (; ((blocks != 0) ^ 1) == 0 && self->start_ >= 42; --blocks) {
        Block b = self->buf_.front();
        self->buf_.pop_front();
        self->buf_.back() = b;
        FUN_0011d894(&self->buf_, Block());
        self->start_ -= 42;
    }
    Alloc_0011e790 a;
    unsigned sz = self->buf_.size_;
    unsigned total = blocks + sz;
    if (total <= self->buf_.capacity()) {
        if (blocks == 0)
            return;
        do {
            Block *p = &self->buf_.back();
            *p = a.allocate();
            FUN_0011d894(&self->buf_, Block());
        } while (--blocks != 0);
        return;
    }
    InitTmpV3_0011e790(&tmp, self);
    tmp.start_ = 0;
    tmp.size_ = 0;
    Buf_0011e790 *t = &tmp.buf_;
    FUN_0011dad8(t, max_0011e790(self->buf_.capacity() << 1, blocks + self->buf_.size_ + 1));
    FUN_0011d894(t, Block());
    if (blocks != 0) {
        do {
            Block *p = &t->back();
            *p = a.allocate();
            FUN_0011d894(t, Block());
        } while (--blocks != 0);
    }
    CopyBlocks_0011e790(t, &self->buf_);
    self->buf_.swap(*t);
    FUN_0011d7e4(t);
    FUN_0011d190(&tmp, -1);
}
// NEAR MISS 6 words (opus1 2026-11-09): EBOOT copy of the Sharing twin wip/deque_grow_back_00129964.cpp (retail bodies are
// identical apart from addresses; callees mapped 128a68->11d894, 124148->118f74, 124130->118f5c, 123fb4->118de0,
// 123668->118494, 128cac->11dad8, 129eb0->11ecdc, 129e28->11ec54, 1289b8->11d7e4, 128364->11d190, d530->d518,
// DAT_002d1750->DAT_002c1370). Same 6 colour words as the twin (size v1 / cap_ a1 / sum a0 / capacity v1 in retail).
// Reversing the capacity test polarity (`if (total > capacity()) { grow } else { fast }`) relays the body (234-241).
// near miss from lane opus1 (unreviewed)
