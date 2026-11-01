/* HSTMATCH module=EBOOT.BIN start=0x0029bd0c end=0x0029bf68 symbol=FUN_0029bd0c lang=c99 */
extern int FUN_0010a5b0(int, int *, int);
extern void FUN_00060224(int, int);
extern void FUN_0001b880(void);
extern void FUN_0029a584(char *, char *);
extern int FUN_002b7790(char *, int, int);
extern int FUN_002b77a0(int, int, int, int, int);
extern int FUN_002b7958(int);
extern int FUN_002b7cf8(void);
extern int FUN_002b7720(int, int, int);
extern int FUN_002b7738(int);

int FUN_0029bd0c(int *p)
{
  int local;
  int ret;
  int h;
  int v;

  local = 0;
  ret = 9;
  if (FUN_0010a5b0(p[4], &local, 2) >= 0 && local == 0) {
    return 0;
  }
  p[2] = ret;
  switch (ret) {
  case 4:
    FUN_00060224(*(int *)0x00311000, 1);
    FUN_0001b880();
    FUN_0029a584((char *)0x2cb714, (char *)0x2cb6dc);
    do {
      h = FUN_002b7790((char *)0x2cb6a4, 1, 0);
      if (h >= 0) break;
      FUN_0001b880();
    } while (1);
    do {
      v = FUN_002b77a0(h, 1, 0, 0, 2);
      if (v >= 0) break;
      FUN_0001b880();
    } while (1);
    p[8] = v;
    FUN_002b77a0(h, 1, 0, 0, 0);
    FUN_0029a584((char *)0x2cb68c, (char *)0);
    do {
      FUN_002b7958(0xf4240);
      if ((FUN_002b7cf8() & 0x20) != 0) break;
    } while (1);
    do {
      if (FUN_002b7720(h, p[7], p[8]) == p[8]) {
        FUN_002b7738(h);
        if (FUN_002b7738(h) >= 0) break;
      }
      FUN_002b7958(0xf4240);
    } while (1);
    FUN_00060224(*(int *)0x00311000, 0);
    break;
  case 3:
    p[1] = 0;
    break;
  }
  return 0;
}
// Ruling: PROGRESS.md VERDICTS (0x0029bd0c); experiment: try/hsttry/queue_put_0029bd0c-11132
// near miss from lane dsn8 (unreviewed)
