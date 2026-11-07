/* HSTMATCH module=hstp_trans_main.prx start=0x00274b9c end=0x00274e08 symbol=FUN_00274b9c lang=c++ */
/* Hand-written reconstruction: clone of the source of status_update_00263d20.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (Ghidra pseudocode ram-0x00263d20.c as a guide), from the lane fm4 near miss: the Obj0 vcall at +0x658 takes the dropped int argument 1, which shares its register (a1) with the stored literal 1; exact match verified; names and types await human review. */
#include "hst/screen_obj.h"

struct Obj {
  virtual void v0();
  virtual void v1(int);
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
};

struct Obj0 {
  virtual void v0();
  virtual void v1(int);
};

/* Message-table entry returned by FUN_001991d4: loaded flag, a count record and a string-list record. */
struct TextCount {
  char pad[8];
  int total;           /* 0x8 */
};
struct TextLines {
  char pad[0xc0];
  char **lines;        /* 0xc0 */
  int count;           /* 0xc4 */
};
struct TextEntry {
  int unk0;
  int loaded;          /* 0x4, zero when the entry is missing */
  TextCount *counts;   /* 0x8 */
  TextLines *list;     /* 0xc */
};

struct EnvFlags {
  char pad[0x178];
  unsigned char unk178;
};

enum { TEXT_ENTRY_ID = 0x11 }; /* entry looked up in the message table */

extern "C" {
extern char *DAT_003f3af4;
extern EnvFlags *DAT_003e1e04;
extern void *DAT_003ea8ec;
extern char DAT_002d10d4[];
extern char DAT_002d10d0[];
extern char DAT_002d10cc[];

void FUN_001118ac(void *, int, int);
TextEntry *FUN_001991d4(int);
void FUN_0011e940(void *, int, void *, int, int);
void FUN_00105470(void *, int);
void FUN_00108f54(void *, int);

/* Refresh the screen's widgets after a state change and show the message (a fixed text-table line, or a notice on first use). */
void FUN_00274b9c(ScreenObj *self)
{
  ((Obj *)self->part)->v1(self->unk1850 == 0);
  FUN_001118ac(self->part, 0, (self->item->unk0e & 3) == 0);
  ScreenPart *part = self->part;
  part->unkd8 = 1;
  part->unk114 = 0;
  ((Obj0 *)self->unk658)->v1(1);
  ((Obj *)self->unk680)->v1(1);
  int unk74 = self->panel->unk74;
  int unk78 = self->panel->unk78;
  if (unk74 != 0 || unk78 != 0) {
    FUN_00105470((void *)self->unk6e0, 0);
    FUN_00105470((void *)self->unk6d8, 0);
    FUN_00105470((void *)self->unk6dc, 0);
  }
  ((Obj *)DAT_003f3af4)->v7();
  void *msg;
  if (self->unk1850 == 0) {
    TextEntry *entry = FUN_001991d4(TEXT_ENTRY_ID);
    if (entry->loaded == 0) {
      msg = DAT_002d10d4;
    } else if (entry->counts->total < 0x11) {
      msg = DAT_002d10d0;
    } else if (entry->list->count < 2) {
      msg = DAT_002d10cc;
    } else {
      msg = entry->list->lines[1];
    }
    FUN_0011e940(DAT_003ea8ec, 0, msg, 0, 0);
  } else {
    bool ok = true;
    if (DAT_003e1e04 != 0 && DAT_003e1e04->unk178 == 0) {
      ok = false;
    }
    if (ok && self->unk1854 == 0 && self->panel->unk4d0 == 0) {
      self->panel->unk4d0 = 1;
      TextEntry *entry = FUN_001991d4(TEXT_ENTRY_ID);
      if (entry->loaded == 0) {
        msg = DAT_002d10d4;
      } else if (entry->counts->total < 0x11) {
        msg = DAT_002d10d0;
      } else if (entry->list->count < 0xc) {
        msg = DAT_002d10cc;
      } else {
        msg = entry->list->lines[11];
      }
      FUN_0011e940(DAT_003ea8ec, 2, msg, 0, 0);
    } else {
      FUN_00108f54(DAT_003ea8ec, 1);
    }
  }
  ((Obj *)self->unk6a0)->v1(0);
  FUN_00105470((void *)self->unk6a4, 0);
}
}
