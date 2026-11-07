/* HSTMATCH module=hstp_trans_main.prx start=0x0005deac end=0x0005dec8 symbol=FUN_0005deac lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
char *FUN_0005deac(void *self, char **pp) { int n = *(int *)*pp; char *d; *pp += 4; d = *pp; *pp += n; return d; }
