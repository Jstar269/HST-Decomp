/* HSTMATCH module=EBOOT.BIN start=0x0027dfac end=0x0027e0e8 symbol=FUN_0027dfac lang=c++ */
struct Tag { char c; };
struct False {};
struct Elem { char pad[100]; unsigned char f100, f101, f102, f103; };
struct Vec { Elem *begin; int n; int cap; };
extern "C" void FUN_0027e4c0(Vec *v, int a, Tag t);
extern "C" void FUN_00000a00(void *p);

extern "C" void FUN_0027dfac(Vec *self, int n, False)
{
  Vec tmp;
  Elem *p, *end, *d;
  int i;
  int oldcap, oldn;
  Elem *oldbegin;

  tmp.begin = 0;
  tmp.n = 0;
  tmp.cap = 0;
  FUN_0027e4c0(&tmp, n, Tag());
  p = self->begin;
  end = self->begin + self->n;
  d = tmp.begin + tmp.n;
  while (p < end) {
    if (d != 0) {
      char *s = (char *)p;
      char *q = (char *)d;
      i = 50;
      do {
        q[0] = s[0];
        q[1] = s[1];
        s += 2;
        q += 2;
      } while (--i != 0);
      d->f100 = p->f100;
      d->f101 = p->f101;
      d->f102 = p->f102;
      d->f103 = p->f103;
    }
    p++;
    d++;
    tmp.n = tmp.n + 1;
  }
  oldcap = self->cap;
  self->cap = tmp.cap;
  tmp.cap = oldcap;
  oldbegin = self->begin;
  self->begin = tmp.begin;
  tmp.begin = oldbegin;
  oldn = self->n;
  self->n = tmp.n;
  tmp.n = 0;
  if (tmp.begin != 0)
    FUN_00000a00(tmp.begin);
}
// NEAR 60 differ (improved from 68): adding a trailing unused empty by-value `False` parameter fixed the
// frame (-48, retail's) and matched the whole prologue + first block byte-for-byte; `struct Tag {char c;}` gives
// retail's `seb a2,v0`. Remaining: outer-loop pointer colouring (retail p=a0, end=v1, d=t3; ours p=v1, end=a3,
// d=t2) and the char-copy inner-loop colouring; every following word shifts. Ruled out (all 60 differ, +0x2c):
// end-declared-first, for-loop head, cached begin local, index-form d computation.
// improved by lane dsn6 (unreviewed): 68 -> 60 differing words
