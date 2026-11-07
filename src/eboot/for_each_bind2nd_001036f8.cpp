/* HSTMATCH module=EBOOT.BIN start=0x001036f8 end=0x001037c8 symbol=FUN_001036f8 lang=c++ data=0x002d8c48 */
/* Hand-written reconstruction: clone of the source of for_each_bind2nd_00103628.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* for_each(v.begin(), v.end(), bind2nd(mem_fun(&Elem::method), flag)) over a vector<Elem *> member at +4
   {data, size, cap}; method is virtual slot 1 taking a bool. The binder (12-byte MWCC pointer-to-member in
   .data + the bool) is copied by value into for_each and called through __ptmf_scall (0x100c) with the bool
   in a1. Same shape as for_each_mem_fun_00103068. Names provisional. */
class Elem_00103628 {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02(bool on);
};

template <class T> struct It_00103628 {
    T *p_;
    It_00103628(T *p) : p_(p) {}
    T &operator*() const { return *p_; }
    It_00103628 &operator++() { ++p_; return *this; }
};
template <class T> inline bool operator==(const It_00103628<T> &a, const It_00103628<T> &b) { return a.p_ == b.p_; }
template <class T> inline bool operator!=(const It_00103628<T> &a, const It_00103628<T> &b) { return !(a == b); }

template <class R, class T, class A> struct MemFun1_00103628 {
    typedef T *first_argument_type;
    typedef A second_argument_type;
    typedef R result_type;
    R (T::*pm_)(A);
    explicit MemFun1_00103628(R (T::*pm)(A)) : pm_(pm) {}
    R operator()(T *p, A a) const { return (p->*pm_)(a); }
};
template <class R, class T, class A> inline MemFun1_00103628<R, T, A> mem_fun_00103628(R (T::*pm)(A)) { return MemFun1_00103628<R, T, A>(pm); }

template <class Op> struct Binder2nd_00103628 {
    Op op_;
    typename Op::second_argument_type value_;
    Binder2nd_00103628(const Op &op, const typename Op::second_argument_type &v) : op_(op), value_(v) {}
    typename Op::result_type operator()(const typename Op::first_argument_type &x) const { return op_(x, value_); }
};
template <class Op, class T> inline Binder2nd_00103628<Op> bind2nd_00103628(const Op &op, const T &x)
{
    return Binder2nd_00103628<Op>(op, typename Op::second_argument_type(x));
}

template <class I, class F> inline F for_each_00103628(I first, I last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

template <class T> struct Vec_00103628 {
    T *data_;
    unsigned size_;
    unsigned cap_;
    It_00103628<T> begin() { return It_00103628<T>(data_); }
    It_00103628<T> end() { return It_00103628<T>(data_ + size_); }
};

struct Owner_00103628 {
    int head_;
    Vec_00103628<Elem_00103628 *> list_;
};

extern "C" void FUN_001036f8(Owner_00103628 *self, bool on)
{
    for_each_00103628(self->list_.begin(), self->list_.end(), bind2nd_00103628(mem_fun_00103628(&Elem_00103628::v02), on));
}
