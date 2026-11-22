/* HSTMATCH module=EBOOT.BIN start=0x0014c274 end=0x0014c344 symbol=FUN_0014c274 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct input_iterator_tag {};
struct forward_iterator_tag : input_iterator_tag {};
struct bidirectional_iterator_tag : forward_iterator_tag {};
struct random_access_iterator_tag : bidirectional_iterator_tag {};

struct Rec {
  char *key;
  int value;
};

struct Iter {
  typedef random_access_iterator_tag iterator_category;
  Rec *p;
  Iter(Rec *q) : p(q) {}
  Iter &operator++() { ++p; return *this; }
  Rec &operator*() const { return *p; }
  int operator-(const Iter &o) const { return p - o.p; }
};

inline bool operator==(const Iter &a, const Iter &b) { return a.p == b.p; }
inline bool operator!=(const Iter &a, const Iter &b) { return a.p != b.p; }

struct Cont {
  Rec *data;
  int count;
  Iter begin() { return Iter(data); }
  Iter end() { return Iter(data + count); }
};

extern "C" int FUN_00014934(const char *a, const char *b);

struct Pred {
  const char *key;
  Pred(const char *k) : key(k) {}
  bool operator()(const Rec &r) const { return FUN_00014934(r.key, key) == 0; }
};

template <class It, class P> inline It find_if(It first, It last, P pred)
{
  while (first != last && !pred(*first))
    ++first;
  return first;
}

template <class It> inline int __distance(It first, It last, random_access_iterator_tag)
{
  return last - first;
}

template <class It> inline int distance(It first, It last)
{
  return __distance(first, last, typename It::iterator_category());
}

extern "C" int FUN_0014c274(Cont *c, const char *key)
{
  Iter it = find_if(c->begin(), c->end(), Pred(key));
  if (it == c->end())
    return -1;
  return distance(c->begin(), it);
}
