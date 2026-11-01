/* HSTMATCH module=EBOOT.BIN start=0x000673c8 end=0x000673fc symbol=FUN_000673c8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern char DAT_00307190[];
extern int FUN_000457b4(int);

typedef struct Obj {
  char *vt;
  int f4;
  int f8;
  float fc;
  char f10;
} Obj;

Obj *FUN_000673c8(Obj *param_1, int param_2, int *param_3, float param_4)
{
  param_1->vt = DAT_00307190;
  param_1->f4 = param_2;
  param_1->f8 = param_3 ? param_3[1] : 0;
  param_1->fc = param_4;
  param_1->f10 = 0;
  return param_1;
}
