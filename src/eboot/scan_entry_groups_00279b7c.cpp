/* HSTMATCH module=EBOOT.BIN start=0x00279b7c end=0x00279f64 symbol=FUN_00279b7c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Entry {
  char pad[0x60];
  signed char kind;
};

struct Pair {
  unsigned char a;
  unsigned char b;
};

struct Save {
  char pad[0x34c];
  Pair set1[640];
  unsigned char set2[640];
  unsigned char set3[640];
  unsigned char set4[640];
};

struct Obj96 {
  char data[96];
};

extern "C" {
extern void *DAT_00338b08;
extern char *DAT_003415a8;
extern Save *DAT_00341ccc;
int FUN_000d8bfc(void *, int);
Entry *FUN_000d8c24(void *, int, int);
void FUN_001bf884(void *, int, int);
void FUN_001bf948(void *, int, int);
void FUN_001bf980(void *, int, int);
void FUN_001bf9b8(void *, int, int);
void FUN_00293a34(Obj96 *);
void FUN_00293c6c(Obj96 *);
void FUN_00293ae0(Obj96 *, int);
}

inline unsigned char *Get1(int i)
{
  Save *s = DAT_00341ccc;
  if (i >= 0 && i < 640) return &s->set1[i].a;
  return 0;
}

inline unsigned char *Get2(int i)
{
  Save *s = DAT_00341ccc;
  if (i >= 0 && i < 640) return &s->set2[i];
  return 0;
}

inline unsigned char *Get3(int i)
{
  Save *s = DAT_00341ccc;
  if (i >= 0 && i < 640) return &s->set3[i];
  return 0;
}

inline unsigned char *Get4(int i)
{
  Save *s = DAT_00341ccc;
  if (i >= 0 && i < 640) return &s->set4[i];
  return 0;
}

extern "C" void FUN_00279b7c(int mode)
{
  int i;
  Obj96 o;

  for (i = 0; i < FUN_000d8bfc(DAT_00338b08, 5); i++) {
    Entry *e = FUN_000d8c24(DAT_00338b08, 5, i);
    if (e != 0) {
      unsigned char *p = Get1(i);
      if (e->kind >= 2 && e->kind == mode && *p < 2u) {
        if (mode == 2)
          FUN_001bf884(DAT_003415a8 + 0x80, i, 6);
        else
          FUN_001bf884(DAT_003415a8 + 0x80, i, 2);
      }
    }
  }
  for (i = 0; i < FUN_000d8bfc(DAT_00338b08, 1); i++) {
    Entry *e = FUN_000d8c24(DAT_00338b08, 1, i);
    if (e != 0) {
      unsigned char *p = Get2(i);
      if (e->kind >= 2 && e->kind == mode && *p < 2u) {
        if (mode == 2)
          FUN_001bf948(DAT_003415a8 + 0x80, i, 6);
        else
          FUN_001bf948(DAT_003415a8 + 0x80, i, 2);
      }
    }
  }
  for (i = 0; i < FUN_000d8bfc(DAT_00338b08, 2); i++) {
    Entry *e = FUN_000d8c24(DAT_00338b08, 2, i);
    if (e != 0) {
      unsigned char *p = Get3(i);
      if (e->kind >= 2 && e->kind == mode && *p < 2u) {
        if (mode == 2)
          FUN_001bf980(DAT_003415a8 + 0x80, i, 6);
        else
          FUN_001bf980(DAT_003415a8 + 0x80, i, 2);
      }
    }
  }
  for (i = 0; i < FUN_000d8bfc(DAT_00338b08, 3); i++) {
    Entry *e = FUN_000d8c24(DAT_00338b08, 3, i);
    if (e != 0) {
      unsigned char *p = Get4(i);
      if (e->kind >= 2 && e->kind == mode && *p < 2u) {
        if (mode == 2)
          FUN_001bf9b8(DAT_003415a8 + 0x80, i, 6);
        else
          FUN_001bf9b8(DAT_003415a8 + 0x80, i, 2);
      }
    }
  }
  FUN_00293a34(&o);
  FUN_00293c6c(&o);
  FUN_00293ae0(&o, -1);
}
