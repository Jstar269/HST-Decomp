/* HSTMATCH module=EBOOT.BIN start=0x0022abdc end=0x0022ac78 symbol=FUN_0022abdc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Matching idiom: block-scope index locals `int i = param_2 + 15;` on a typed 2D byte table (stride 20) keep the +15/+5 out of the 0x3d00 displacement (as in flag_lookup_0022af6c.c). */
typedef struct S { char pad[0x3d00]; unsigned char tab[1][20]; } S;
extern S *DAT_003415a8;
extern int FUN_0022a9a8(int, int);

unsigned char FUN_0022abdc(int param_1, int param_2, int *param_3)
{
  int v;
  unsigned char r;

  v = FUN_0022a9a8(param_1, param_2);
  if (param_3 != 0) {
    *param_3 = v;
  }
  if (v == 1) {
    int i = param_2 + 15;
    r = DAT_003415a8->tab[param_1][i];
  }
  else {
    int j = param_2 + 5;
    r = DAT_003415a8->tab[param_1][j];
  }
  return r;
}
