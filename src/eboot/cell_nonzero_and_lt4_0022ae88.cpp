/* HSTMATCH module=EBOOT.BIN start=0x0022ae88 end=0x0022af6c symbol=FUN_0022ae88 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct S { char pad[0x3d00]; unsigned char tab[1][20]; };
extern "C" {
extern S *DAT_003415a8;
int FUN_0022a9a8(int, int);
}

static inline int GetCell(int a, int b)
{
  if (FUN_0022a9a8(a, b) == 1) {
    int i = b + 10;
    return DAT_003415a8->tab[a][i];
  }
  return DAT_003415a8->tab[a][b];
}

static inline bool Ge1(int a, int b)
{
  return GetCell(a, b) >= 1;
}

extern "C" bool FUN_0022ae88(int a, int b)
{
  return Ge1(a, b) && GetCell(a, b) < 4;
}
