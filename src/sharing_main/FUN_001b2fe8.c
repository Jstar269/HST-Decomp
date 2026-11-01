/* HSTMATCH module=hstp_trans_main.prx start=0x001b2fe8 end=0x001b30e0 symbol=FUN_001b2fe8 lang=c99 */
/* Clone of src/eboot/FUN_001a5a1c.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly and pseudocode; exact match verified; names and types await human review. */
void FUN_001b2fe8(int *pos, int *slot, int dir, int n, int wrap, int *tbl, int thr)
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
