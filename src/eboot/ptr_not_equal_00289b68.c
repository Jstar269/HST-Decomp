/* HSTMATCH module=EBOOT.BIN start=0x00289b68 end=0x00289b80 symbol=FUN_00289b68 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_00289b68(struct It *a, struct It *b) { return !(a->p == b->p); }
