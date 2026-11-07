/* HSTMATCH module=EBOOT.BIN start=0x000b5d78 end=0x000b5d90 symbol=FUN_000b5d78 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_000b5d78(struct It *a, struct It *b) { return !(a->p == b->p); }
