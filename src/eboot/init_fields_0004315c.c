/* HSTMATCH module=EBOOT.BIN start=0x0004315c end=0x00043180 symbol=FUN_0004315c lang=c99 */
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

void FUN_0004315c(Obj *self, short c, char h, float a, float b, float d, float e)
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
