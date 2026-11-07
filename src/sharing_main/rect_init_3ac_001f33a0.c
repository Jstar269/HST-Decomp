/* HSTMATCH module=hstp_trans_main.prx start=0x001f33a0 end=0x001f33bc symbol=FUN_001f33a0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Rect { char pad[8]; short x; short y; };
struct Obj { char pad[0x3ac]; struct Rect rect; };
extern void FUN_0022687c(void *rec, int x0, int y0, int x1, int y1, int steps);
void FUN_001f33a0(struct Obj *self) { FUN_0022687c(&self->rect, self->rect.x + 350, self->rect.y, self->rect.x, self->rect.y, 5); }
