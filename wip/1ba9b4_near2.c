/* HSTMATCH module=EBOOT.BIN start=0x001ba9b4 end=0x001baac0 symbol=FUN_001ba9b4 lang=c99 */
extern int FUN_001feec4(void);
extern unsigned char FUN_001baf80(int p);
extern void FUN_002060f8(int a0, int a1, void *a2, int a3, int a4, int a5, int a6);

void FUN_001ba9b4(char *self, int idx, int **arr)
{
  char *e = (char *)*arr + idx * 48;
  char *o;
  unsigned char r;
  int first, el, b0, b1, b2, is2;

  if (*(unsigned char *)e == 0) return;
  o = (char *)FUN_001feec4();
  first = *(short *)(o + 64);
  {
  char buf[24] = {0};
  *(int *)(buf + 0) = first;
  *(int *)(buf + 4) = *(short *)(o + 66);
  *(int *)(buf + 8) = *(short *)(o + 68);
  *(int *)(buf + 12) = *(short *)(o + 70);
  *(int *)(buf + 16) = *(short *)(o + 72);
  *(int *)(buf + 20) = *(short *)(o + 74);
  r = FUN_001baf80(*(int *)(*(int *)(*(int *)(self + 4) + idx * 4) + 4));
  el = *(int *)(*(int *)(self + 4) + idx * 4);
  is2 = *(unsigned char *)(o + 0xa6) == 2;
  b0 = *(unsigned char *)(o + 0xde);
  b1 = *(unsigned char *)(o + 0xdc);
  b2 = *(unsigned char *)(o + 0xdd);
  FUN_002060f8(el, r, buf, b0, b1, b2, is2);
  *(char *)(self + idx + 0x10) = 1;
  *(char *)(self + idx + 0x14) = 0;
  }
}
// dsn6: 67/67 words, 14 differ, first +0xb0. Fixed is2 to `o->a6 == 2` (was `(x^2)==0`) removing the extra `move t2,v0` (68->67 words). Residual: retail schedules the a6 load 3rd (after el) and the de/dc/dd loads after el; ours loads de/dc before the el pointer math and a6 last. Tail colouring: retail `addu v1,s2,s3; li a0,1; sb a0,16(v1)` vs ours `addu a0,s3,s2; li v1,1; sb v1,16(a0)`. Ruled out: v3/v5/v6 source reorderings of el/is2/b0..b2, declaration reorder, fully-inline args (v4, 26 differ) -> scheduler/colour floor.
// near miss from lane dsn6 (unreviewed)
