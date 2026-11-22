/* HSTMATCH module=EBOOT.BIN start=0x0027f04c end=0x0027f3cc symbol=FUN_0027f04c lang=c++ */
/* Hand-written reconstruction from the disassembly (near miss, parked in wip/); names and types await human review. */
#pragma inline_depth(8)
inline void *operator new(unsigned, void *p) { return p; }

template <class T> inline const T &Min(const T &a, const T &b) { return b < a ? b : a; }

struct Tag {};

class C_0027e6d4 {
public:
  virtual ~C_0027e6d4();
  short s4, s6, s8, sa, sc, se, s10;
  char a[10];
  char b[9];
  unsigned char c;
  short d;
};

struct Vec {
  C_0027e6d4 *data;
  unsigned size;
  unsigned cap;
};

extern "C" unsigned FUN_0027f814(Vec *, unsigned);
extern "C" void FUN_0027f8f8(Vec *, unsigned, unsigned);

static inline void Fill(C_0027e6d4 *p, unsigned m, const C_0027e6d4 &x, Tag, Tag)
{
  for (; m != 0; --m, ++p) {
    *p = x;
  }
}

static inline void AppendN(Vec *v, unsigned n, const C_0027e6d4 &x, Tag)
{
  C_0027e6d4 *p = v->data + v->size;
  for (; n != 0; --n, ++p) {
    new (p) C_0027e6d4(x);
    ++v->size;
  }
}

static inline void DestroyBack(C_0027e6d4 *e, int cnt, Tag)
{
  for (; cnt != 0; --cnt) {
    --e;
    e->~C_0027e6d4();
  }
}

extern "C" void FUN_0027f04c(Vec *v, unsigned n, const C_0027e6d4 *x)
{
  unsigned c;
  if (v->cap < n) {
    c = FUN_0027f814(v, n - v->cap);
  } else {
    Fill(v->data, Min(n, v->size), *x, Tag(), Tag());
    if (n < v->size) {
      int cnt = v->size - n;
      C_0027e6d4 *e = v->data + v->size;
      v->size -= cnt;
      DestroyBack(e, cnt, Tag());
      goto done;
    }
    AppendN(v, n - v->size, *x, Tag());
    goto done;
  }
  FUN_0027f8f8(v, n, c);
  AppendN(v, n, *x, Tag());
done:;
}

// api-hste1 NEAR 2 differ: the returns replaced by `goto done` (tail: grow call + append after the else block, as in vec_fill16_000d565c.cpp) remove all 4 dead words;
// only the two loads before `n < size` are swapped (ours lw v1,24(sp); lw a0,4(s1); retail lw a0,4(s1); lw v1,24(sp)).
// Tried: v->size > n, local size copy (before the test or before Min), !(n >= size), arm swap with size <= n / n >= size (81 differ).
// improved by lane api-hste1-20261121 (unreviewed): 210 -> 2 differing words
