/* HSTMATCH module=EBOOT.BIN start=0x00199904 end=0x00199a0c symbol=FUN_00199904 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int DAT_003091b0;
extern int DAT_0030ebf4;
extern int DAT_002e1944[6];
extern int DAT_002e195c;
extern int DAT_0034179c;
extern void FUN_0001f06c(void *self);
extern void FUN_0001cad0(void *a, int b, void *c);
extern void *FUN_00000bcc(int size);
extern int FUN_00050ff4(void *p, int a, int b);
extern void FUN_001b2bc8(void *p);
extern void FUN_00199754(void *p, int a);
extern void FUN_0001cb88(void *p);

int FUN_00199904(int this_)
{
  int *o = (int *)this_;
  int *s0;
  int i;
  int *tab;

  FUN_0001f06c(o);
  o[0] = (int)&DAT_003091b0;
  o[15] = 0;
  FUN_0001cad0(&DAT_0030ebf4, 1, &DAT_002e195c);
  if ((s0 = (int *)FUN_00000bcc(44)) != 0) {
    s0[6] = 0;
    s0[7] = 0;
    s0[8] = 0;
    *(char *)((char *)s0 + 36) = 0;
    s0[10] = -1;
    for (i = 0, tab = DAT_002e1944; i < 6; i++) {
      void *p = FUN_00000bcc(72);
      if (p != 0) {
        p = (void *)FUN_00050ff4(p, *tab, 0);
      }
      s0[i] = (int)p;
      tab++;
    }
    FUN_001b2bc8(&DAT_0034179c);
    FUN_00199754(s0, 0);
  }
  o[15] = (int)s0;
  FUN_0001cb88(&DAT_0030ebf4);
  return this_;
}
