/* HSTMATCH module=EBOOT.BIN start=0x0005a650 end=0x0005a670 symbol=FUN_0005a650 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Ring { char pad[0x50]; int count; int pos; int pad1; int pending; };
void FUN_0005a650(struct Ring *r) { r->pos = (r->pos + 1) % r->count; r->pending = 0; }
