/* HSTMATCH module=hstp_trans_main.prx start=0x002bd670 end=0x002bd694 symbol=FUN_002bd670 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Key { long long v; };
int FUN_002bd670(const struct Key *a, const struct Key *b) { return a->v == b->v; }
