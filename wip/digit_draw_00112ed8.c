/* HSTMATCH module=EBOOT.BIN start=0x00112ed8 end=0x00113194 symbol=FUN_00112ed8 lang=c99 */
extern float fabsf(float x);
extern float FUN_0000bd2c(float x);
extern void FUN_000ffb3c(int a, int b);
extern void FUN_000ffca4(int a);
extern int DAT_002d9360;

void FUN_00112ed8(float param_1, int *param_2, int param_3)
{
  short sa;
  short sb;
  short t;
  int base;
  int cnt;
  int idx;
  int elem;
  int off;
  int i;
  int digit;
  int max;
  int limit;
  char flag;
  float val;

  val = FUN_0000bd2c(fabsf(param_1) + 0.001f);
  cnt = param_2[2];
  digit = (int)val;
  base = *(int *)(*param_2 + 0x10);
  idx = param_2[1] + param_3 * cnt;
  elem = *(int *)(base + idx * 4);
  sa = *(short *)(elem + 4);
  sb = *(short *)(elem + 6);
  if (cnt < 9) {
    limit = *(int *)(0x2c114c + cnt * 4) - 1;
  } else {
    limit = 999999999;
  }
  max = DAT_002d9360;
  if (DAT_002d9360 <= digit && digit <= limit) max = digit;
  flag = *(char *)((int)param_2 + param_3 + 0x4c);
  i = 0;
  if (0 < cnt) {
    do {
      if (i == cnt - 1) flag = 0;
      off = (cnt - i) * 4;
      digit = (max % *(int *)(0x2c114c + off)) /
              *(int *)(0x2c1148 + (cnt - i) * 4);
      idx = (idx + i) * 4;
      FUN_000ffb3c(*(int *)(base + idx), digit == 0 && flag != 0);
      max = param_2[param_3 + 4];
      elem = *(int *)(*(int *)(*param_2 + 0x10) + idx);
      *(int *)(elem + 0x24) = max + digit;
      *(int *)(elem + 0x28) = max + digit;
      FUN_000ffca4(*(int *)(*(int *)(*param_2 + 0x10) + idx));
      elem = *(int *)(*(int *)(*param_2 + 0x10) + idx);
      *(short *)(elem + 4) = *(short *)(elem + 4) + sa;
      *(short *)(elem + 8) = *(short *)(elem + 8) + sa;
      elem = *(int *)(*(int *)(*param_2 + 0x10) + idx);
      t = *(short *)(elem + 6);
      *(short *)(elem + 6) = sb;
      *(short *)(elem + 10) = *(short *)(elem + 10) + (sb - t);
      elem = *(int *)(*(int *)(*param_2 + 0x10) + idx);
      *(short *)(elem + 8) = *(short *)(elem + 8) + (short)*(char *)((int)param_2 + param_3 + 0x5b);
      elem = *(int *)(*(int *)(*param_2 + 0x10) + idx);
      *(short *)(elem + 10) = *(short *)(elem + 10) + (short)*(char *)((int)param_2 + param_3 + 0x6a);
      base = *(int *)(*param_2 + 0x10);
      sa = *(short *)(*(int *)(base + idx) + 8);
      if (digit != 0) flag = 0;
      i = i + 1;
    } while (i < cnt);
  }
}

/* NEAR 171/175 words, 154 differ, first +0x0. Frame 64 vs retail 80 -- but the save sequence itself is
   already identical (ra + s8..s0 all in retail's slots, and the 0.001f `lui/ori` materialises in the
   same gap), so the 16 missing bytes are pure LOCAL AREA: retail spills 16 bytes the draft never needs.
   Body is 4 words short. Structure decoded: val=bd2c(fabsf(p1)+0.001f); cnt=p2[2]; digit=(int)val;
   base=*(*p2+0x10); idx=p2[1]+p3*cnt; elem=base[idx]; sa=+4, sb=+6 shorts; limit = cnt<9 ?
   *(int*)(0x2c114c+cnt*4)-1 : 999999999; max = clamp(digit, DAT_002d9360, limit); flag=*(char*)
   (p2+p3+0x4c); for i<cnt: (i==cnt-1) flag=0; digit=(max % *(int*)(0x2c114c+(cnt-i)*4)) /
   *(int*)(0x2c1148+(cnt-i)*4); FUN_000ffb3c(base[idx+i], digit==0 && flag); pos=p2[p3+4];
   *(elem+0x24)=*(elem+0x28)=pos+digit; FUN_000ffca4(elem); shorts +4/+8 += sa; +6 = sb and +10 += sb-t;
   +8 += (char)(p2+p3+0x5b); +10 += (char)(p2+p3+0x6a); sa = elem+8; if (digit) flag=0.
   NEXT: the 16-byte local area is the only prologue difference -- retail keeps `pos` (p2[p3+4]) and the
   final `sa` in frame slots across the two calls, so promote both to named locals declared before the
   loop and read them back after FUN_000ffca4 (that is what forces the spills); the 4 missing words
   should follow from the same live ranges. */
// near miss from lane dsn8 (unreviewed)
