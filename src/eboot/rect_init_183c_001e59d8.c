/* HSTMATCH module=EBOOT.BIN start=0x001e59d8 end=0x001e59f4 symbol=FUN_001e59d8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Rect { char pad[8]; short x; short y; };
struct Obj { char pad[0x183c]; struct Rect rect; };
extern void FUN_002177d4(void *rec, int x0, int y0, int x1, int y1, int steps);
void FUN_001e59d8(struct Obj *self) { FUN_002177d4(&self->rect, self->rect.x + 350, self->rect.y, self->rect.x, self->rect.y, 5); }
