/* HSTMATCH module=EBOOT.BIN start=0x0022af6c end=0x0022aff4 symbol=FUN_0022af6c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef struct S { char pad[0x3d00]; unsigned char tab[1][20]; } S;
extern S *DAT_003415a8;
extern int FUN_0022a9a8(int, int);
int FUN_0022af6c(int a, int b)
{
  int v;
  if (FUN_0022a9a8(a, b) == 1) {
    int i = b + 10;
    v = DAT_003415a8->tab[a][i];
  } else
    v = DAT_003415a8->tab[a][b];
  return v == 0;
}
