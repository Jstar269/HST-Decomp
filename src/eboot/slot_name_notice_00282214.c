/* HSTMATCH module=EBOOT.BIN start=0x00282214 end=0x00282518 symbol=FUN_00282214 lang=c99 */
/* Hand-written reconstruction from the disassembly; matching idiom: the id-reset loop walks an int pointer from the owner base with the 0x940 table offset added at each store (as in init_slots_00283660.c); exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/text_bank_lookup.h"

typedef struct SlotState {
  int ids[4];               /* 0x000 */
  int unk10;                /* 0x010 */
  u8 unk14;                 /* 0x014 */
  char pad15[3];
  char unk18[0x28];         /* 0x018 */
  char unk40[0xcc];         /* 0x040 */
  char name[0x28];          /* 0x10c */
  int link;                 /* 0x134 */
  char pad138[0x148 - 0x138];
  char unk148[0x10];        /* 0x148 */
  char unk158[8];           /* 0x158 */
  char unk160[0xcc];        /* 0x160 */
  u8 unk22c;                /* 0x22c */
  char pad22d[0x280 - 0x22d];
  char unk280[0xcc];        /* 0x280 */
  u8 unk34c;                /* 0x34c */
  char pad34d[0x458 - 0x34d];
} SlotState;

typedef struct Entry {
  char pad0[0xf8];
  char name[0x20];          /* 0x0f8 */
  char pad118[0x458 - 0x118];
} Entry;

typedef struct Owner {
  u8 unk0;
  u8 unk1;                  /* 0x001 */
  char pad2[0x90 - 2];
  u8 present[0x940 - 0x90]; /* 0x090 */
  SlotState slot[4];        /* 0x940 */
  char pad1aa0[0x1ab4 - 0x1aa0];
  Entry entries[4];         /* 0x1ab4 */
  char pad2c14[0x34d4 - 0x2c14];
  char text[0x400];         /* 0x34d4 */
  u8 busy;                  /* 0x38d4 */
} Owner;

typedef struct SlotView {
  char pad000[0x940];
  SlotState s;
} SlotView;

extern char *DAT_00338ac4;
extern char *DAT_003415a8;
extern void *DAT_003415ac;
extern int FUN_00014a40(const char *);
extern char *FUN_000149a8(char *, const char *);
extern int FUN_000143b0(char *, const char *, ...);
extern void FUN_002805fc(char *text, int b, int c, char *d);
extern int FUN_000bbba4(void *, void *);
extern int FUN_000bd8e0(void *, int);
extern int FUN_000bd8f8(void *, int);
extern void FUN_000bd704(void *, int);
extern TextBank *FUN_0018c9f4(int);
extern void FUN_00114168(void *, int, char *, int, int);
extern void FUN_0008d114(void *);
extern void FUN_0008d2ec(void *);
extern void *FUN_0008d0e4(void *, void *);
extern void *FUN_0008d0cc(void *, void *);

static inline u8 Check(Owner *self, int link)
{
  u8 r = 1;
  if (self->present[link] == 0 && FUN_000bbba4(DAT_00338ac4, DAT_00338ac4 + 0x3e) >= 0)
    r = 0;
  return r;
}

void FUN_00282214(Owner *self, int idx)
{
  char buf[24];
  int link;
  int show;

  if (self->busy != 0)
    return;
  link = self->slot[idx].link;
  if (link < 0) {
    char *nm = self->slot[idx].name;
    if (FUN_00014a40(nm) != 0)
      FUN_000149a8(buf, nm);
    else
      buf[0] = 0;
  } else {
    FUN_000149a8(buf, self->entries[link].name);
  }
  if (link >= 0) {
    show = Check(self, link);
  } else {
    show = FUN_000bbba4(DAT_00338ac4, DAT_00338ac4 + 0x3e) < 0;
  }
  if (show) {
    if (FUN_00014a40(buf) != 0) {
      int a = FUN_000bd8e0(DAT_00338ac4, idx);
      FUN_002805fc(self->text, a, FUN_000bd8f8(DAT_00338ac4, idx), buf);
    } else {
      FUN_002805fc(self->text, 1, 0, buf);
    }
    self->busy = 1;
  } else {
    if (self->unk1 != 0) {
      if (FUN_00014a40(buf) != 0) {
        int a = FUN_000bd8e0(DAT_00338ac4, idx);
        FUN_002805fc(self->text, a, FUN_000bd8f8(DAT_00338ac4, idx), buf);
      }
    } else {
      if (FUN_00014a40(buf) != 0) {
        FUN_000143b0(self->text, TextGet(FUN_0018c9f4(0x11), 16, 14), buf);
        FUN_00114168(DAT_003415ac, 6, self->text, 0, 0);
      }
    }
    self->busy = 0;
    if (self->unk1 != 0)
      self->busy = 1;
  }
  FUN_000bd704(DAT_00338ac4, idx);
  {
    u32 k = 0;
    int *q = (int *)((char *)self + idx * (int)sizeof(SlotState));
    do {
      k++;
      *(q + 0x940 / sizeof(int)) = -1;
      q++;
    } while (k < 4);
  }
  self->slot[idx].unk10 = -1;
  self->slot[idx].unk14 = 0;
  FUN_0008d114(self->slot[idx].unk18);
  FUN_0008d2ec(self->slot[idx].unk40);
  self->slot[idx].name[0] = 0;
  FUN_0008d2ec(self->slot[idx].unk160);
  FUN_0008d2ec(self->slot[idx].unk280);
  self->slot[idx].unk22c = 0;
  self->slot[idx].unk34c = 0;
  FUN_0008d0e4(self->slot[idx].unk148, DAT_003415a8 + 4);
  FUN_0008d0cc(self->slot[idx].unk158, DAT_003415a8 + 0x18);
}
