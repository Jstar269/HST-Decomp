/* HSTMATCH module=EBOOT.BIN start=0x00102958 end=0x00102a88 symbol=FUN_00102958 lang=c99 */
extern int DAT_0033b45c;
extern char DAT_002d8a28[];
extern int DAT_0033b394;
extern int FUN_00014a40(int);
extern void FUN_000149a8(void *, int);
extern void FUN_00014788(void *, char *);
extern int FUN_00014934(void *, int *);

void FUN_00102958(int param_1)
{
  int *p;
  int id;
  int n;
  unsigned int k;
  int *e;
  unsigned char buf[260];

  p = *(int **)(param_1 + 4);
  if (p != p + *(int *)(param_1 + 8)) {
    do {
      id = *p;
      if (DAT_0033b45c == 0) {
        n = *(int *)(param_1 + 8);
        goto next;
      }
      if (FUN_00014a40(id + 0x59) == 0) {
        n = *(int *)(param_1 + 8);
        goto next;
      }
      FUN_000149a8(buf, id + 0x59);
      FUN_00014788(buf, DAT_002d8a28);
      k = 0;
      e = (int *)0x33a6e0;
      do {
        if (FUN_00014934(buf, e) == 0) {
          *(unsigned char *)(k * 0x41 + 0x33a6e0) = 0;
          *(int *)((char *)&DAT_0033b394 + k * 4) = 0;
          DAT_0033b45c = DAT_0033b45c - 1;
          break;
        }
        k = k + 1;
        e = (int *)((char *)e + 0x41);
      } while (k < 0x32);
      n = *(int *)(param_1 + 8);
    next:
      p = p + 1;
    } while (p != (int *)(*(int *)(param_1 + 4) + n * 4));
  }
}
// Ruling: PROGRESS.md VERDICTS (0x00102958); experiment: PROGRESS.md VERDICTS
// near miss from lane dsn8 (unreviewed)
