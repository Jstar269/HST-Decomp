/* HSTMATCH module=hstp_trans_main.prx start=0x001a63f8 end=0x001a6500 symbol=FUN_001a63f8 lang=c99 */
/* Clone of src/eboot/FUN_00199904.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int DAT_003b247c;
extern int DAT_003b7ebc;
extern int DAT_00389148[6];
extern int DAT_00389160;
extern int DAT_003eaad8;
extern void FUN_0001ec44(void *self);
extern void FUN_0001c684(void *a, int b, void *c);
extern void *FUN_00000bcc(int size);
extern int FUN_00051908(void *p, int a, int b);
extern void FUN_001c0cf8(void *p);
extern void FUN_001a6248(void *p, int a);
extern void FUN_0001c73c(void *p);

int FUN_001a63f8(int this_)
{
  int *o = (int *)this_;
  int *s0;
  int i;
  int *tab;

  FUN_0001ec44(o);
  o[0] = (int)&DAT_003b247c;
  o[15] = 0;
  FUN_0001c684(&DAT_003b7ebc, 1, &DAT_00389160);
  if ((s0 = (int *)FUN_00000bcc(44)) != 0) {
    s0[6] = 0;
    s0[7] = 0;
    s0[8] = 0;
    *(char *)((char *)s0 + 36) = 0;
    s0[10] = -1;
    for (i = 0, tab = DAT_00389148; i < 6; i++) {
      void *p = FUN_00000bcc(72);
      if (p != 0) {
        p = (void *)FUN_00051908(p, *tab, 0);
      }
      s0[i] = (int)p;
      tab++;
    }
    FUN_001c0cf8(&DAT_003eaad8);
    FUN_001a6248(s0, 0);
  }
  o[15] = (int)s0;
  FUN_0001c73c(&DAT_003b7ebc);
  return this_;
}
