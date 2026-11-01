/* HSTMATCH module=EBOOT.BIN start=0x0006e93c end=0x0006e978 symbol=FUN_0006e93c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_0006e93c(unsigned int *param_1, void *param_2)
{
  unsigned int i = 0;
  if (param_1[0] != 0) {
    char *p = (char *)param_1[1];
    do {
      if (p == (char *)param_2) {
        *(unsigned char *)(param_1[2] + i) = 0;
        return;
      }
      i++;
      p += 0x70;
    } while (i < param_1[0]);
  }
}
