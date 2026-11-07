/* HSTMATCH module=EBOOT.BIN start=0x0006ddb4 end=0x0006de18 symbol=FUN_0006ddb4 lang=c99 */
/* Hand-written reconstruction: clone of the source of set_vec_in_cur_buf_0019f4f0.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Clone of src/eboot/set_vec_in_cur_buf_00192ce0.c (hstclone.py tag c43): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Vec3 { float x, y, z; };
struct Entry { char pad[4]; struct Vec3 v; char pad2[12]; };
struct Obj { char pad[0x54]; int cur; int pad2[3]; struct Entry *bufs[2]; };

void FUN_0006ddb4(struct Obj *p, int n, struct Vec3 *v)
{
  p->bufs[p->cur][n].v.x = v->x;
  p->bufs[p->cur][n].v.y = v->y;
  p->bufs[p->cur][n].v.z = v->z;
}
