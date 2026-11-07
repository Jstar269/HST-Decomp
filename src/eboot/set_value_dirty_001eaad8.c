/* HSTMATCH module=EBOOT.BIN start=0x001eaad8 end=0x001eaaf4 symbol=FUN_001eaad8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x418]; unsigned char dirty; char pad1[3]; int values[1]; };
struct Obj { struct Sub *sub; };
void FUN_001eaad8(struct Obj *self, int i, int v) { struct Sub *s = self->sub; s->dirty = 1; s->values[i] = v; }
