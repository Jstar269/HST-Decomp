/* HSTMATCH module=hstp_trans_main.prx start=0x0029b470 end=0x0029b490 symbol=FUN_0029b470 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Vec3 { union { int v[3]; struct { int x, y, z; }; }; };
struct Vec3 *FUN_0029b470(struct Vec3 *self) { self->x = 0; self->y = 0; self->z = 0; self->y = self->v[1] - self->y; return self; }
