/* HSTMATCH module=EBOOT.BIN start=0x001414e4 end=0x001415e8 symbol=FUN_001414e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma opt_strength_reduction off
extern void FUN_000ce8e8(void *p);
extern void FUN_00058694(void *p, void *src);
extern void FUN_00067790(void *a, void *b);
extern char DAT_00307564[];
extern char DAT_0030753C[];

void FUN_001414e4(char *self, void *arg)
{
  char b[112];
  char *p;
  unsigned i;
  int off;
  char *pb;

  i = 0;
  if (*(int *)(self + 0x7a8) == 0) return;
  pb = b;
  for (; i < *(int *)(self + 0x7a8); i++) {
    off = i * 80;
    p = (char *)(*(int *)(self + 0x7a4) + off);
    FUN_000ce8e8(pb);
    *(char **)pb = DAT_00307564;
    FUN_00058694(pb + 16, p);
    *(int *)(pb + 80) = 0;
    *(int *)(pb + 84) = 0x3f800000;
    *(int *)(pb + 88) = 0x453b8000;
    *(int *)(pb + 92) = 0;
    *(char *)(pb + 96) = 0;
    *(int *)(b + 80) = *(int *)((char *)*(int *)(self + 0x7a4) + off + 0x40);
    *(int *)(b + 84) = 0x3f800000;
    *(int *)(b + 88) = 0x455ac000;
    *(char *)(b + 96) = 0;
    FUN_00067790(arg, b);
    *(char **)b = DAT_0030753C;
  }
}
