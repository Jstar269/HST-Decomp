/* HSTMATCH module=EBOOT.BIN start=0x00101ce8 end=0x00101db8 symbol=FUN_00101ce8 lang=c++ data=0x002d8b04 */
/* Hand-written reconstruction: clone of the source of for_each_bind2nd_00107754.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction: clone of the source of for_each_bind2nd_00101c18.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* for_each(v.begin(), v.end(), bind2nd(mem_fun(&Elem::SetFlag), flag)) over a vector<Elem *> member at +0x10
   {data, size, cap}; the method is the NON-virtual member 0xffb8c (sets/clears bit 0 of the halfword at +14),
   so the 12-byte pointer-to-member in .data {0, -1, func} relocates against a mangled member name.
   Names provisional. */
class Elem_001054c0 {
public:
    void f_000ffb8c(bool on);
};

template <class T> struct It_00101c18 {
    T *p_;
    It_00101c18(T *p) : p_(p) {}
    T &operator*() const { return *p_; }
    It_00101c18 &operator++() { ++p_; return *this; }
};
template <class T> inline bool operator==(const It_00101c18<T> &a, const It_00101c18<T> &b) { return a.p_ == b.p_; }
template <class T> inline bool operator!=(const It_00101c18<T> &a, const It_00101c18<T> &b) { return !(a == b); }

template <class R, class T, class A> struct MemFun1_00101c18 {
    typedef T *first_argument_type;
    typedef A second_argument_type;
    typedef R result_type;
    R (T::*pm_)(A);
    explicit MemFun1_00101c18(R (T::*pm)(A)) : pm_(pm) {}
    R operator()(T *p, A a) const { return (p->*pm_)(a); }
};
template <class R, class T, class A> inline MemFun1_00101c18<R, T, A> mem_fun_00101c18(R (T::*pm)(A)) { return MemFun1_00101c18<R, T, A>(pm); }

template <class Op> struct Binder2nd_00101c18 {
    Op op_;
    typename Op::second_argument_type value_;
    Binder2nd_00101c18(const Op &op, const typename Op::second_argument_type &v) : op_(op), value_(v) {}
    typename Op::result_type operator()(const typename Op::first_argument_type &x) const { return op_(x, value_); }
};
template <class Op, class T> inline Binder2nd_00101c18<Op> bind2nd_00101c18(const Op &op, const T &x)
{
    return Binder2nd_00101c18<Op>(op, typename Op::second_argument_type(x));
}

template <class I, class F> inline F for_each_00101c18(I first, I last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

template <class T> struct Vec_00101c18 {
    T *data_;
    unsigned size_;
    unsigned cap_;
    It_00101c18<T> begin() { return It_00101c18<T>(data_); }
    It_00101c18<T> end() { return It_00101c18<T>(data_ + size_); }
};

struct Owner_00101c18 {
    int pad_[4];
    Vec_00101c18<Elem_001054c0 *> list_;
};

extern "C" void FUN_00101ce8(Owner_00101c18 *self, bool on)
{
    for_each_00101c18(self->list_.begin(), self->list_.end(), bind2nd_00101c18(mem_fun_00101c18(&Elem_001054c0::f_000ffb8c), on));
}
// EXACT (hstmatch). PTMF {0, -1, 0xffb8c} at 0x2d8b04 names the non-virtual member 0xffb8c; the mangled member
// symbol f_000ffb8c__13Elem_001054c0Fb is in symbols/EBOOT.BIN.txt.
