/* HSTMATCH module=EBOOT.BIN start=0x00109a20 end=0x00109ca8 symbol=FUN_00109a20 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Elem { char pad[0x20]; float key; };
struct Buf { int cap; int size; Elem **data; };

struct Out {
    Elem **pos;
    Buf *buf;
    Out() {}
    Out(Elem **p, Buf *b) : pos(p), buf(b) {}
    Out &operator=(Elem *const &x) {
        if (pos - buf->data < buf->size) {
            *pos = x;
        } else {
            if (pos != 0) *pos = x;
            ++buf->size;
        }
        return *this;
    }
    Out operator*() { return *this; }
    Out &operator++() { ++pos; return *this; }
};

struct It {
    Elem **p;
    It() {}
    It(Elem **q) : p(q) {}
    Elem *const &operator*() const { return *p; }
    It &operator++() { ++p; return *this; }
};
inline bool operator==(const It &a, const It &b) { return a.p == b.p; }
inline bool operator!=(const It &a, const It &b) { return !(a == b); }

struct Greater {
    bool operator()(const Elem *a, const Elem *b) const { return a->key > b->key; }
};

template <class In, class O>
inline O copy_(In first, In last, O result)
{
    for (; first != last; ++first, ++result)
        *result = *first;
    return result;
}

extern "C" Out FUN_00109a20(It first1, It last1, It first2, It last2, Out result, Greater comp)
{
    for (; first1 != last1 && first2 != last2; ++result) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1;
        }
    }
    if (first1 == last1)
        return copy_(first2, last2, result);
    return copy_(first1, last1, result);
}
