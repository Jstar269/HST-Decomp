/* HSTMATCH module=EBOOT.BIN start=0x00114178 end=0x00114190 symbol=FUN_00114178 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x90a]; unsigned char flag; };
struct Obj { char pad[0x54]; struct Sub *sub; };
extern void FUN_00114190(struct Obj *self, int a, int b, int c, int d);
void FUN_00114178(struct Obj *self, int a, int b, int d) { self->sub->flag = 1; FUN_00114190(self, a, b, 0, d); }
