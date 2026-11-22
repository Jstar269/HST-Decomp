/* HSTMATCH module=EBOOT.BIN start=0x00250f68 end=0x0025112c symbol=FUN_00250f68 lang=c99 */
typedef struct Row { int a, b, c; } Row;
typedef struct Obj {
  char pad[0xac];
  void *f_ac;
  void *f_b0[3];
  void *f_bc;
} Obj;
extern char DAT_002fc4e0[];
extern int DAT_002fc518;
extern double FUN_0000c07c(double, double);
extern double FUN_00022178(int);
extern int FUN_00022254(double);
extern void FUN_000ffb3c(void *, int);

void FUN_00250f68(Obj *self, int param_2, int param_3)
{
  int i;
  int d;
  int g;
  int *ac;
  char *tp;
  if (param_3 == 0) {
    FUN_000ffb3c(self->f_bc, 1);
    FUN_000ffb3c(self->f_ac, 1);
    for (i = 0; i < 3; i++) {
      FUN_000ffb3c(self->f_b0[i], 1);
    }
  }
  else if (param_2 >= 0) {
    FUN_000ffb3c(self->f_bc, 0);
    FUN_000ffb3c(self->f_ac, 1);
    g = DAT_002fc518;
    tp = DAT_002fc4e0;
    i = 0;
    ac = (int *)self->f_ac;
    ac[9] = g;
    ac[10] = g;
    for (; i < 3; i++, tp += 12) {
      d = FUN_00022254(FUN_0000c07c(10.0, FUN_00022178(2 - i)));
      FUN_000ffb3c(self->f_b0[i], param_2 < d && i != 2);
      {
        int v = *(int *)(tp + 0x44) + (param_2 / d) % 10;
        int *o = (int *)self->f_b0[i];
        o[9] = v;
        o[10] = v;
      }
    }
  }
  else {
    FUN_000ffb3c(self->f_bc, 0);
    FUN_000ffb3c(self->f_ac, 1);
    g = DAT_002fc518;
    i = 0;
    tp = DAT_002fc4e0;
    ac = (int *)self->f_ac;
    ac[9] = g;
    ac[10] = g;
    for (; i < 3; i++, tp += 12) {
      FUN_000ffb3c(self->f_b0[i], 0);
      {
        int v = *(int *)(tp + 0x44) + 10;
        int *o = (int *)self->f_b0[i];
        o[9] = v;
        o[10] = v;
      }
    }
  }
}

/* near miss api-hstm-20261121: 113/113 words, 9 differ (was 74): only the s0/s1 colouring of the general (param_2 >= 0) loop is swapped
   (ours i=s0, table pointer s1; retail i=s1, pointer s0); the param_2 < 0 loop and everything else match.
   Found: the cases are `p3==0`, `param_2 >= 0` (general, laid out first), else negative; the double chain is
   FUN_00022254(FUN_0000c07c(10.0, FUN_00022178(2 - i))) with double prototypes; each store pair uses one local (v, o = f_b0[i]);
   the table pointer must be a user char* walked `tp += 12` (without it the negative loop is swapped too).
   Ruled out for the swap: all declaration orders of i,d,g,ac,tp, init statement orders, increment order, do-while. */
// improved by lane api-hstm-20261121 (unreviewed): 74 -> 9 differing words
