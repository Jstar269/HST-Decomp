/* HSTMATCH module=EBOOT.BIN start=0x0023b43c end=0x0023b5c0 symbol=FUN_0023b43c lang=c99 */
extern unsigned char *FUN_0016bc30(int v);
extern unsigned char FUN_00238cc0(void *a, void *b);

void FUN_0023b43c(int *first, int *last)
{
  int *min, *q;
  int *end = last;
  int *limit = last - 1;
  if (((first == last) ^ 1) && first < limit) {
    for (; first < limit; first++) {
      min = first;
      if ((first == last) ^ 1) {
        q = first + 1;
        while ((q == end) ^ 1) {
          unsigned char *pq = FUN_0016bc30(*q);
          unsigned char *pm = FUN_0016bc30(*min);
          int a = (unsigned)(pq[0] - 4) < 2u;
          int b = (unsigned)(pm[0] - 4) < 2u;
          int c;
          if (!a || !b) {
            if (!a || b) {
              if (!a) { if (b) c = 1; else c = 0; }
              else c = 0;
            } else c = 1;
          } else c = FUN_00238cc0(pq + 4, pm + 4) & 0xff;
          if (c != 0) min = q;
          q++;
        }
      }
      if ((min == first) ^ 1) {
        int t = *min;
        *min = *first;
        *first = t;
      }
    }
  }
}
// dsn6 best: 90/97 words, 96 differ, first +0x0. Reconstructed comparator faithfully from Ghidra branch structure (if (!a||!b){...}) and used `(x==y)^1` guards, taking 62 -> 90 words. Residual: retail frame -80 saves s0-s6 (7 regs) vs ours -32 s0-s5 (6); retail keeps TWO copies of `last` (s6 outer `i!=last`, s0 inner `q!=end`) which we CSE to one, plus 48 bytes of unused local frame. Ruled out: plain != (bne/bne, 76 words), while vs for, char/unsigned char/int c.
// near miss from lane dsn6 (unreviewed)
