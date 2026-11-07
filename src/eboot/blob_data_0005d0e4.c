/* HSTMATCH module=EBOOT.BIN start=0x0005d0e4 end=0x0005d100 symbol=FUN_0005d0e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
char *FUN_0005d0e4(void *self, char **pp) { int n = *(int *)*pp; char *d; *pp += 4; d = *pp; *pp += n; return d; }
