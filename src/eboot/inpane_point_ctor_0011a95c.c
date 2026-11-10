/* HSTMATCH module=EBOOT.BIN start=0x0011a95c end=0x0011aa84 symbol=FUN_0011a95c lang=c99 */
/* Hand-written reconstruction from the Ghidra pseudocode (lane sn3 near miss wip/inpane_point_ctor_0011a95c.c, finished by lane pn4: the format argument global is `extern char *const`, which lets its load schedule into the prologue saves like retail); exact match verified; loop colours need declaration order `int i; char **names; int *p;` and block-scoped `int j; int *tab; int *q;`; names and types await human review. */
extern char DAT_002d9c5c[];
extern char DAT_002c12ec[];
extern char *const DAT_002da0ac;
extern char *DAT_002d9cc4[];
extern int DAT_002d9ef8[];
extern void FUN_000143b0();
extern void *FUN_00000bcc(int);
extern void *FUN_00102e38(void *);
extern void FUN_00104118();
extern void FUN_00103540(float, void *);
extern void FUN_001037c8(void *, int);
extern void *FUN_001039d8(void *, char *, int);
extern int FUN_001026b8();

int *FUN_0011a95c(int *param_1)
{
  char buf[256];
  void *mem;
  int i;
  char **names;
  int *p;

  FUN_000143b0(buf, DAT_002d9c5c, DAT_002c12ec, DAT_002da0ac);
  mem = FUN_00000bcc(0x54);
  if (mem != 0) {
    mem = FUN_00102e38(mem);
  }
  *(void **)param_1 = mem;
  FUN_00104118(mem, buf, 0);
  FUN_00103540(3099.0f, *(void **)param_1);
  FUN_001037c8(*(void **)param_1, -4096);
  i = 0;
  names = DAT_002d9cc4;
  p = param_1;
  do {
    p[1] = (int)FUN_001039d8(*(void **)param_1, *names, 0);
    i++;
    names++;
    p++;
  } while (i < 7);
  {
    int j;
    int *tab;
    int *q;
    j = 0;
    tab = DAT_002d9ef8;
    q = param_1;
    do {
      int r = FUN_001026b8(param_1[tab[0] + 1], tab[1]);
      q[8] = r;
      j++;
      q++;
      tab[2] = *(int *)(r + 0x24);
      tab += 3;
    } while (j < 35);
  }
  return param_1;
}
