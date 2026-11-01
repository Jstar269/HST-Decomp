/* HSTMATCH module=hstp_trans_main.prx start=0x000700fc end=0x00070138 symbol=FUN_000700fc lang=c99 */
/* Clone of src/eboot/scan_entry_0006e93c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

void FUN_000700fc(unsigned int *param_1, void *param_2)
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
