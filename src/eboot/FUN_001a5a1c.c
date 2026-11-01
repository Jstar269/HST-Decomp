/* HSTMATCH module=EBOOT.BIN start=0x001a5a1c end=0x001a5b14 symbol=FUN_001a5a1c lang=c99 */
/* Hand-written reconstruction from the disassembly and pseudocode; exact match verified; names and types await human review. */
void FUN_001a5a1c(int *pos, int *slot, int dir, int n, int wrap, int *tbl, int thr)
{
  int base, i, idx;
  if (dir < 0) {
    i = 1;
    if (n <= 1) return;
    base = *pos;
    do {
      idx = base - i;
      if (idx < 0) {
        if (wrap == 0) return;
        idx += n;
      }
      if (thr < *(int *)(*(int *)(*tbl + *slot * 12) + idx * 4)) {
        *pos = idx;
        return;
      }
      i++;
    } while (i < n);
  } else {
    i = 1;
    if (n <= 1) return;
    idx = *pos + 1;
    do {
      base = idx;
      if (idx >= n) {
        if (wrap == 0) return;
        base = idx - n;
      }
      if (thr < *(int *)(*(int *)(*tbl + *slot * 12) + base * 4)) {
        *pos = base;
        return;
      }
      i++;
      idx++;
    } while (i < n);
  }
}
