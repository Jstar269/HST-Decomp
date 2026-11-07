/* HSTMATCH module=hstp_trans_main.prx start=0x00107efc end=0x001080cc symbol=FUN_00107efc lang=c++ data=0x0037f904 */
/* Hand-written reconstruction: clone of the source of for_each_bind2nd_rev_00102490.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* for_each(rbegin(), rend(), bind2nd(mem_fun(&Elem::f), flag)) over a vector<Elem*> at +0x10 when flag != 0,
   for_each(begin(), end(), ...) otherwise; the NON-virtual member 0x102660 (PTMF {0, -1, func} per instantiation,
   so symbols/EBOOT.BIN.txt carries f_00102660__13Elem_00102490Fb). Matching idiom: RevIt has an unwritten
   leading word (retail copies 8-byte reverse iterators, only the second word holds the pointer), and for_each
   advances its by-value parameter directly. */
class Elem_00102490 {
public:
    void f_00102660(bool on);
};

template <class T> struct It_00102490 {
    T *p_;
    It_00102490(T *p) : p_(p) {}
    T &operator*() const { return *p_; }
    It_00102490 &operator++() { ++p_; return *this; }
    It_00102490 &operator--() { --p_; return *this; }
};
template <class T> inline bool operator==(const It_00102490<T> &a, const It_00102490<T> &b) { return a.p_ == b.p_; }
template <class T> inline bool operator!=(const It_00102490<T> &a, const It_00102490<T> &b) { return !(a == b); }

template <class T> struct RevIt_00102490 {
    int unused_;
    It_00102490<T> current_;
    explicit RevIt_00102490(It_00102490<T> i) : current_(i) {}
    T &operator*() const { return current_.p_[-1]; }
    RevIt_00102490 &operator++() { --current_; return *this; }
};
template <class T> inline bool operator==(const RevIt_00102490<T> &a, const RevIt_00102490<T> &b) { return a.current_ == b.current_; }
template <class T> inline bool operator!=(const RevIt_00102490<T> &a, const RevIt_00102490<T> &b) { return !(a == b); }

template <class R, class T, class A> struct MemFun1_00102490 {
    typedef T *first_argument_type;
    typedef A second_argument_type;
    typedef R result_type;
    R (T::*pm_)(A);
    explicit MemFun1_00102490(R (T::*pm)(A)) : pm_(pm) {}
    R operator()(T *p, A a) const { return (p->*pm_)(a); }
};
template <class R, class T, class A> inline MemFun1_00102490<R, T, A> mem_fun_00102490(R (T::*pm)(A)) { return MemFun1_00102490<R, T, A>(pm); }

template <class Op> struct Binder2nd_00102490 {
    Op op_;
    typename Op::second_argument_type value_;
    Binder2nd_00102490(const Op &op, const typename Op::second_argument_type &v) : op_(op), value_(v) {}
    typename Op::result_type operator()(const typename Op::first_argument_type &x) const { return op_(x, value_); }
};
template <class Op, class T> inline Binder2nd_00102490<Op> bind2nd_00102490(const Op &op, const T &x)
{
    return Binder2nd_00102490<Op>(op, typename Op::second_argument_type(x));
}

template <class I, class F> inline F for_each_00102490(I first, I last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}

template <class T> struct Vec_00102490 {
    T *data_;
    unsigned size_;
    unsigned cap_;
    It_00102490<T> begin() { return It_00102490<T>(data_); }
    It_00102490<T> end() { return It_00102490<T>(data_ + size_); }
    RevIt_00102490<T> rbegin() { return RevIt_00102490<T>(It_00102490<T>(data_ + size_)); }
    RevIt_00102490<T> rend() { return RevIt_00102490<T>(It_00102490<T>(data_)); }
};

struct Owner_00102490 {
    int pad_[4];
    Vec_00102490<Elem_00102490 *> list_;
};

extern "C" void FUN_00107efc(Owner_00102490 *self, bool flag)
{
    if (flag)
        for_each_00102490(self->list_.rbegin(), self->list_.rend(),
                          bind2nd_00102490(mem_fun_00102490(&Elem_00102490::f_00102660), flag));
    else
        for_each_00102490(self->list_.begin(), self->list_.end(),
                          bind2nd_00102490(mem_fun_00102490(&Elem_00102490::f_00102660), flag));
}
