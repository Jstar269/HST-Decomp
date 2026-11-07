/* HSTMATCH module=EBOOT.BIN start=0x002ade48 end=0x002ade6c symbol=FUN_002ade48 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Key { long long v; };
int FUN_002ade48(const struct Key *a, const struct Key *b) { return a->v == b->v; }
