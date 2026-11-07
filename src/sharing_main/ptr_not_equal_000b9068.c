/* HSTMATCH module=hstp_trans_main.prx start=0x000b9068 end=0x000b9080 symbol=FUN_000b9068 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_000b9068(struct It *a, struct It *b) { return !(a->p == b->p); }
