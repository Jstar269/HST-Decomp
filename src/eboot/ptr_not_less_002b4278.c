/* HSTMATCH module=EBOOT.BIN start=0x002b4278 end=0x002b428c symbol=FUN_002b4278 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_002b4278(struct It *a, struct It *b) { return a->p >= b->p; }
