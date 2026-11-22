/* HSTMATCH module=EBOOT.BIN start=0x002296a0 end=0x00229774 symbol=FUN_002296a0 lang=c99 */
typedef struct {
  unsigned int first;
  unsigned int second;
} Pair;

typedef struct {
  char pad[0x10];
  int map;
} Owner;

extern void FUN_00229774(Owner *self);
extern unsigned char *FUN_0004cad0(void *arg, int *size, int a, int b);
extern void FUN_00229af0(void *out, void *map, const Pair *item);
extern void FUN_000104e0(void *p);

int FUN_002296a0(Owner *self, void *arg)
{
  int size;
  unsigned char out[12];
  Pair item;
  unsigned char *buf;
  unsigned char *p;

  FUN_00229774(self);
  buf = FUN_0004cad0(arg, &size, 16, 1);
  if (buf == 0 || size <= 0)
    return 0;
  for (p = buf; p < buf + size; ) {
    unsigned short hi = *(unsigned short *)p;
    p = (unsigned char *)((unsigned short *)p + 1);
    unsigned short lo = *(unsigned short *)p;
    p = (unsigned char *)((unsigned short *)p + 1);
    unsigned int val = *(unsigned int *)p;
    item.first = ((unsigned int)hi << 16) | lo;
    item.second = val;
    p = (unsigned char *)((unsigned int *)p + 1);
    FUN_00229af0(out, &self->map, &item);
  }
  FUN_000104e0(buf);
  return 1;
}
// Improved to 53/53 words, 21 differing (down from 31): typed pointer increments
// ((unsigned short *)p + 1) x2 and ((unsigned int *)p + 1) reproduce retail's unfolded
// +2/+2/+4 addiu chain, `buf == 0 || size <= 0` reproduces retail's beqzl/bgtzl/b guard,
// and out[12] + Pair match the stack slots at sp+32/sp+24. Remaining 21 diffs are purely
// s-reg permutation (retail buf=s0, self=s1, p=s2; ours p=s0, buf=s1, self=s2) and
// scheduling addiu s2,v1,4 after the two sw stores. Tried C/C++, Pair temp vs local,
// reusing arg for buf, block-scoped p in do/while.
// improved by lane oc-hst-20261121 (unreviewed): 24 -> 21 differing words
