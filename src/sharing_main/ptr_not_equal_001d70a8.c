/* HSTMATCH module=hstp_trans_main.prx start=0x001d70a8 end=0x001d70c0 symbol=FUN_001d70a8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_001d70a8(struct It *a, struct It *b) { return !(a->p == b->p); }
