/* HSTMATCH module=hstp_trans_main.prx start=0x00125b30 end=0x00125c58 symbol=FUN_00125b30 lang=c99 */
/* Clone of src/eboot/inpane_point_ctor_0011a95c.c (hstclone.py tag c145607): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the Ghidra pseudocode (lane sn3 near miss wip/inpane_point_ctor_00125b30.c, finished by lane pn4: the format argument global is `extern char *const`, which lets its load schedule into the prologue saves like retail); exact match verified; loop colours need declaration order `int i; char **names; int *p;` and block-scoped `int j; int *tab; int *q;`; names and types await human review. */
extern char DAT_00380c3c[];
extern char DAT_002d16cc[];
extern char *const DAT_0038108c;
extern char *DAT_00380ca4[];
extern int DAT_00380ed8[];
extern void FUN_000143b0();
extern void *FUN_00000bcc(int);
extern void *FUN_0010873c(void *);
extern void FUN_00109a44();
extern void FUN_00108e6c(float, void *);
extern void FUN_001090f4(void *, int);
extern void *FUN_00109304(void *, char *, int);
extern int FUN_00108124();

int *FUN_00125b30(int *param_1)
{
  char buf[256];
  void *mem;
  int i;
  char **names;
  int *p;

  FUN_000143b0(buf, DAT_00380c3c, DAT_002d16cc, DAT_0038108c);
  mem = FUN_00000bcc(0x54);
  if (mem != 0) {
    mem = FUN_0010873c(mem);
  }
  *(void **)param_1 = mem;
  FUN_00109a44(mem, buf, 0);
  FUN_00108e6c(3099.0f, *(void **)param_1);
  FUN_001090f4(*(void **)param_1, -4096);
  i = 0;
  names = DAT_00380ca4;
  p = param_1;
  do {
    p[1] = (int)FUN_00109304(*(void **)param_1, *names, 0);
    i++;
    names++;
    p++;
  } while (i < 7);
  {
    int j;
    int *tab;
    int *q;
    j = 0;
    tab = DAT_00380ed8;
    q = param_1;
    do {
      int r = FUN_00108124(param_1[tab[0] + 1], tab[1]);
      q[8] = r;
      j++;
      q++;
      tab[2] = *(int *)(r + 0x24);
      tab += 3;
    } while (j < 35);
  }
  return param_1;
}
