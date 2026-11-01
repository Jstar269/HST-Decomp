/* HSTMATCH module=hstp_trans_main.prx start=0x00043240 end=0x00043264 symbol=FUN_00043240 lang=c99 */
/* Clone of src/eboot/init_fields_0004315c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct Obj {
  char pad[0x40];
  float a;
  float b;
  short c;
  float d;
  float e;
  int f;
  int g;
  char h;
} Obj;

void FUN_00043240(Obj *self, short c, char h, float a, float b, float d, float e)
{
  self->a = a;
  self->b = b;
  self->c = c;
  self->d = d;
  self->e = e;
  self->f = 0;
  self->g = 0;
  self->h = h;
}
