/* HSTMATCH module=hstp_trans_main.prx start=0x000452c8 end=0x00045674 symbol=FUN_000452c8 lang=c++ */
/* Hand-written reconstruction from the disassembly (structure from src/sharing_main/quicksort_ptr_001c6d0c.cpp, Entry/Swap from selection_sort_entries_00045790.cpp); exact match verified; names and types await human review. */
struct Entry {
  unsigned short key;
  unsigned short val;
  Entry &operator=(const Entry &o)
  {
    key = o.key;
    val = o.val;
    return *this;
  }
};

inline void Swap(Entry &a, Entry &b)
{
  Entry t = a;
  a = b;
  b = t;
}

struct Less {
  bool operator()(const Entry &a, const Entry &b) const { return a.key < b.key; }
};
#define LT(a, b) comp(*(a), *(b))

extern int DAT_00375900;
extern "C" void FUN_00045674(Entry *a, Entry *b, Entry *c, Less comp);
extern "C" void FUN_00045790(Entry *first, Entry *last, Less comp);

inline void Bump()
{
  DAT_00375900 = DAT_00375900 + 1;
  if (DAT_00375900 >= 5)
    DAT_00375900 = -4;
}

extern "C" void FUN_000452c8(Entry *first, Entry *last, Less comp)
{
  for (;;) {
    int n = last - first;
    if (n <= 1)
      return;
    if (n < 21) {
      FUN_00045790(first, last, comp);
      return;
    }
    int q1 = n / 4;
    Entry *m1 = first + (q1 + DAT_00375900 % 5);
    Bump();
    int q2 = (n * 3) / 4;
    Entry *m2 = first + (q2 + DAT_00375900 % 5);
    Bump();
    Entry *lm = last - 1;
    FUN_00045674(m1, m2, lm, comp);
    Entry *i = first;
    Entry *j = lm;
    while (LT(i, lm))
      ++i;
    do {
      --j;
    } while (i != j && !LT(j, lm));
    if (i < j) {
      Swap(*i, *j);
      ++i;
      for (;;) {
        while (LT(i, lm))
          ++i;
        do {
          --j;
        } while (!LT(j, lm));
        if (!(i < j))
          break;
        Swap(*i, *j);
        ++i;
      }
    }
    if (i == first) {
      Swap(*i, *lm);
      ++i;
      Entry *j2 = last - 1;
      if (!LT(first, &last[-1])) {
        while (i != last && !LT(first, i))
          ++i;
        if (i < j2)
          Swap(*i, *j2);
      }
      if (i < j2) {
        for (;;) {
          while (!LT(first, i))
            ++i;
          do {
            --j2;
          } while (LT(first, j2));
          if (!(i < j2))
            break;
          Swap(*i, *j2);
          ++i;
        }
      }
      first = i;
    } else {
      if (i - first < last - i) {
        FUN_000452c8(first, i, comp);
        first = i;
      } else {
        FUN_000452c8(i, last, comp);
        last = i;
      }
    }
  }
}
