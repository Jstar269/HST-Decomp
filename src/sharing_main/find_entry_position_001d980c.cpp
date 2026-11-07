/* HSTMATCH module=hstp_trans_main.prx start=0x001d980c end=0x001d993c symbol=FUN_001d980c lang=c++ */
/* Hand-written reconstruction: clone of the source of find_entry_position_001d993c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Copies the 80-byte entries of the global list (DAT_003e48b4+0xc04) into a local vector (FUN_001dcb1c = range init), finds the entry with the given id, stores its +0x40 vector to *pos and *pos + the +0x30 vector to *out (inline operator+ over FUN_00022440, rotated return copy), destroys the copy (FUN_00152db8 = vector dtor) and returns 1, else 0. Matching idioms: the derived iterator tag is passed as a temporary rnd_tag() (lb a3,44(sp) from a never-written slot; a named tag local sits at 48); the entry is read through a reference local `const Elem &e = *it;` (a direct `it->id == id` gives bnel s2,v0 in either operand order); the vector dtor is called explicitly with flag 0 as retail does (a local with a real dtor passes -1; 0 is what a base subobject gets); free operator!= written !(a == b). */
struct Vec4 {
    float x, y, z, w;
    Vec4() {}
} __attribute__((aligned(16)));

struct Elem {
    int id;
    char pad04[0x2c];
    Vec4 off;
    Vec4 pos;
};

struct in_tag {};
struct fwd_tag : in_tag {};
struct bid_tag : fwd_tag {};
struct rnd_tag : bid_tag {};

template <class T> struct It {
    T *p_;
    It(T *p) : p_(p) {}
    T &operator*() const { return *p_; }
    T *operator->() const { return p_; }
    It &operator++() { ++p_; return *this; }
};
template <class T> inline bool operator==(const It<T> &a, const It<T> &b) { return a.p_ == b.p_; }
template <class T> inline bool operator!=(const It<T> &a, const It<T> &b) { return !(a == b); }

struct List {
    Elem *data_;
    unsigned size_;
    unsigned cap_;
    List() : data_(0), size_(0), cap_(0) {}
    It<Elem> begin() const { return It<Elem>(data_); }
    It<Elem> end() { return It<Elem>(data_ + size_); }
};

struct Game {
    char pad[0xaf8];
    List list;
};

extern Game *DAT_003e48b4;

extern "C" {
void FUN_001dcb1c(List *, Elem *, Elem *, rnd_tag);
void FUN_00152db8(List *, short);
Vec4 *FUN_00022440(Vec4 *, const Vec4 *, const Vec4 *);
}

static inline Vec4 operator+(const Vec4 &a, const Vec4 &b)
{
    Vec4 r;
    FUN_00022440(&r, &a, &b);
    return r;
}

extern "C" int FUN_001d980c(void *self, int id, Vec4 *pos, Vec4 *out)
{
    List v;
    FUN_001dcb1c(&v, DAT_003e48b4->list.data_, DAT_003e48b4->list.data_ + DAT_003e48b4->list.size_, rnd_tag());
    for (It<Elem> it = v.begin(); it != v.end(); ++it) {
        const Elem &e = *it;
        if (e.id == id) {
            *pos = e.pos;
            *out = *pos + e.off;
            FUN_00152db8(&v, 0);
            return 1;
        }
    }
    FUN_00152db8(&v, 0);
    return 0;
}
