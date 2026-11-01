/* HSTMATCH module=EBOOT.BIN start=0x0006ac74 end=0x0006acb0 symbol=FUN_0006ac74 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_0006ac74(unsigned int *param_1, void *param_2)
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
      p += 0x1b0;
    } while (i < param_1[0]);
  }
}
