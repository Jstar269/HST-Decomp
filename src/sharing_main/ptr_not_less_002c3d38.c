/* HSTMATCH module=hstp_trans_main.prx start=0x002c3d38 end=0x002c3d4c symbol=FUN_002c3d38 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct It { char *p; };
int FUN_002c3d38(struct It *a, struct It *b) { return a->p >= b->p; }
