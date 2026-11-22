/* HSTMATCH module=EBOOT.BIN start=0x000fbaf4 end=0x000fbbf8 symbol=FUN_000fbaf4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct {
  char pad[0x3c];
  void *f3c;
  void *slots[6];
} Obj;

extern void *FUN_00000bcc(unsigned int size);
extern void *FUN_00050fc0(void *mem);
extern void FUN_000511c4(void *obj, char *name, int flag);
extern void FUN_00051398(void *obj, float value);
extern void *FUN_00102e38(void *obj);
extern void FUN_00104118(void *obj, char *name, int flag);
extern void FUN_00103540(void *obj, float value);
extern void FUN_001037c8(void *obj, int mode);
extern void FUN_00103628(void *obj, int flag);
extern unsigned char DAT_003386b8;
extern char DAT_002d8324[];
extern char *const DAT_002c0d5c[];

void FUN_000fbaf4(Obj *self)
{
  if (DAT_003386b8 != 0) {
    int i = 0;
    Obj *p = self;
    char *const *src = DAT_002c0d5c;
    do {
      void *obj = FUN_00000bcc(72);
      if (obj != 0)
        obj = FUN_00050fc0(obj);
      p->slots[0] = obj;
      FUN_000511c4(obj, *src, 0);
      FUN_00051398(p->slots[0], 100000.0f);
      i++;
      p = (Obj *)((char *)p + 4);
      src++;
    } while (i < 6);
    void *q = FUN_00000bcc(84);
    if (q != 0)
      q = FUN_00102e38(q);
    self->f3c = q;
    FUN_00104118(q, DAT_002d8324, 0);
    FUN_00103540(self->f3c, 100000.0f);
    FUN_001037c8(self->f3c, -1);
    FUN_00103628(self->f3c, 1);
  }
}
