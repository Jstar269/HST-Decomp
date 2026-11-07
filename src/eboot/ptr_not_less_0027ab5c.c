/* HSTMATCH module=EBOOT.BIN start=0x0027ab5c end=0x0027ab70 symbol=FUN_0027ab5c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_0027ab5c(struct It *a, struct It *b) { return a->p >= b->p; }
