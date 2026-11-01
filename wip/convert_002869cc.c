/* HSTMATCH module=EBOOT.BIN start=0x002869cc end=0x00286acc symbol=FUN_002869cc lang=c99 */
extern char DAT_002fe9d0[];
extern void FUN_00286e08(int);
extern void FUN_0014c100(void *, int, int, int);
extern void FUN_0014c914(int, void *);
extern void FUN_0014c820(void *, int);
extern char FUN_00286af8(int, int);
extern void FUN_00286acc(char *, int);

char FUN_002869cc(int param_1, int param_2, int param_3)
{
  unsigned char buf[36];
  char cVar2;
  int h;
  int v;
  int bVar4;

  if (*(int *)(param_1 + 4) == 0) {
    cVar2 = 0;
  }
  else {
    h = *(int *)(*(int *)(param_1 + 8) + 4);
    bVar4 = (h != 0) && (*(int *)(*(int *)(param_1 + 8) + 0x14) != h + 4);
    v = *(int *)(param_1 + 4);
    if (bVar4) {
      FUN_00286e08(param_1);
      v = *(int *)(param_1 + 4);
    }
    FUN_0014c100(buf, v, param_2, param_1);
    FUN_0014c914(*(int *)(param_1 + 8), buf);
    FUN_0014c820(buf, -1);
    cVar2 = FUN_00286af8(param_1, param_3);
    if (cVar2 == 0 && *(int *)(*(int *)(param_1 + 8) + 4) == 0) {
      FUN_00286acc(DAT_002fe9d0, param_2);
    }
  }
  return cVar2;
}
// Ruling: PROGRESS.md VERDICTS (0x002869cc); experiment: try/hsttry/convert_002869cc-43328
// near miss from lane dsn8 (unreviewed)
