/* HSTMATCH module=EBOOT.BIN start=0x000b6894 end=0x000b6990 symbol=FUN_000b6894 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct input_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

struct Rec {
  float f[20];
} __attribute__((aligned(16)));

struct It {
  typedef random_access_iterator_tag iterator_category;
  Rec *p;
  It() {}
  It(Rec *q) : p(q) {}
  Rec &operator*() const { return *p; }
  It &operator++() { ++p; return *this; }
  It &operator+=(int n) { p += n; return *this; }
  int operator-(const It &o) const { return p - o.p; }
};

struct Comp {
  bool operator()(Rec a, Rec b) const { return a.f[14] < b.f[14]; }
};

template <class Iter> inline int __distance(Iter first, Iter last, random_access_iterator_tag)
{
  return last - first;
}

template <class Iter> inline int distance(Iter first, Iter last)
{
  return __distance(first, last, typename Iter::iterator_category());
}

template <class Iter> inline void __advance(Iter &it, int n, random_access_iterator_tag)
{
  it += n;
}

template <class Iter> inline void advance(Iter &it, int n)
{
  __advance(it, n, typename Iter::iterator_category());
}

extern "C" It FUN_000b6894(It first, It last, const Rec &value)
{
  Comp comp;
  int len = distance(first, last);
  while (len > 0) {
    It i = first;
    int len2 = len / 2;
    advance(i, len2);
    if (!comp(value, *i)) {
      first = ++i;
      len -= len2 + 1;
    } else {
      len = len2;
    }
  }
  return first;
}
