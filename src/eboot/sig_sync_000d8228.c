/* HSTMATCH module=EBOOT.BIN start=0x000d8228 end=0x000d8334 symbol=FUN_000d8228 lang=c99 */
/* Ghidra pseudocode (hstgen.py tag a1) corrected by hand: restructured as do/while loops, packed signature as an int, rebuilt table lookup; exact match verified; names and types await human review. */

struct Obj {
  short src[6];
  char pad[0x67 - 0x0c];
  unsigned char key;
  char pad2[0xb8 - 0x68];
  int sig;
  short cur[6];
};

extern char DAT_002d688c[];
void FUN_000d8160(struct Obj *, int *);

void FUN_000d8228(struct Obj *self)
{
  int local[5];
  int changed;
  unsigned i;
  short *p;

  changed = self->sig != self->key;
  if (!changed) {
    i = 0;
    p = self->src;
    do {
      if (p[0x5e] != p[0]) {
        changed = 1;
        break;
      }
      i++;
      p++;
    } while (i < 6);
  }
  if (changed) {
    i = 0;
    p = self->src;
    do {
      i++;
      p[0x5e] = p[0];
      p++;
    } while (i < 6);
    self->sig = -1;
    {
      unsigned k = 0;
      int *q = (int *)self;
      do {
        k++;
        q[0x29] = 0;
        q++;
      } while (k < 5);
    }
    self->sig = self->key;
    FUN_000d8160(self, local);
    {
      unsigned j = 0;
      int *l = local;
      unsigned off = 0;
      float *o = (float *)self;
      do {
        j++;
        o[0x29] = *(float *)((int)(DAT_002d688c + *l * 0x14) + off);
        l++;
        off += 4;
        o++;
      } while (j < 5);
    }
  }
}
