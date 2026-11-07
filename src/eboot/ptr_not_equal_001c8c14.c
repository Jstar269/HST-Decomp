/* HSTMATCH module=EBOOT.BIN start=0x001c8c14 end=0x001c8c2c symbol=FUN_001c8c14 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_001c8c14(struct It *a, struct It *b) { return !(a->p == b->p); }
