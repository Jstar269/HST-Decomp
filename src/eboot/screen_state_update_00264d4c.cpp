/* HSTMATCH module=EBOOT.BIN start=0x00264d4c end=0x00265d00 symbol=FUN_00264d4c lang=c++ rodata=0x002ca894 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/screen_mgr.h"
#pragma inline_depth(8)

extern char DAT_002c0e18[];
extern char DAT_002c0e1c[];
extern char DAT_002c0e20[];

struct Hdr { char pad[8]; int count; };
struct Row { const char **strs; int count; int pad; };
struct Bank { int pad0; int loaded; Hdr *hdr; Row *rows; };

/* String `idx` of group `group` in a text bank; fallback strings for an unloaded bank or an out-of-range group or index. */
static inline const char *Text(Bank *b, int group, int idx)
{
  if (b->loaded == 0) return DAT_002c0e20;
  if (group < 0 || group >= b->hdr->count) return DAT_002c0e1c;
  if (idx < 0 || idx >= b->rows[group].count) return DAT_002c0e18;
  return b->rows[group].strs[idx];
}

typedef ScreenPage Sprite;
static inline bool IsShown(const Sprite *s) { return (s->flags & 3) != 0; }

struct Widget {
  virtual void v0();
  virtual void SetActive(int on);
  char pad4[0x24 - 4];
  int lo;
  int hi;
  char pad2c[0x60 - 0x2c];
  float x;
  char pad64[0xf8 - 0x64];
  const char *text;
  char padfc[0x138 - 0xfc];
  int dirty;
};
struct Range { char pad[0x24]; int lo; int hi; };
struct Flag28 { char pad[0x28]; unsigned char on; };
struct Owner {
  virtual void v0();
  virtual void v1();
  virtual void Show(bool on);
  char pad4[0xdc - 4];
  int cursor;
  char pade0[0xe8 - 0xe0];
  unsigned char unke8;
  char pade9[0x120 - 0xe9];
  Flag28 *unk120;
};
struct Slot { char pad0[0x16]; unsigned char unk16; char pad17; unsigned char unk18; char pad19[0x120 - 0x19]; };
struct Ent { unsigned char ready; char pad[0x458 - 1]; };
struct Mgr {
  char pad0[0x18];
  char title[0x58 - 0x18];
  int unk58;
  char pad5c[0x74 - 0x5c];
  int dx;
  int dy;
  char pad7c[0x4d1 - 0x7c];
  unsigned char announced;
  char pad4d2[0x600 - 0x4d2];
  Slot slots[3];

};
struct Game { char pad0[0x3e]; char id3e[0x178 - 0x3e]; unsigned char host; char pad179[0x198 - 0x179]; int unk198; };
struct Speaker { virtual void v0(); virtual void v1(); virtual void Flush(); };
struct Float0c { char pad[0xc]; float v; };
struct Sub1860 { virtual void v0(); virtual void v1(); virtual void Update(); };

struct Self {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual int IsActive();
  char pad4[0x58 - 4];
  Owner *owner;
  char pad5c[0x640 - 0x5c];
  Mgr *mgr;
  char pad644[0x64c - 0x644];
  int frames;
  char pad650[0x658 - 0x650];
  Widget *w658;
  char pad65c[0x684 - 0x65c];
  Widget *panel;
  Widget *title;
  Widget *w68c;
  Widget *w690;
  Widget *w694;
  Widget *w698;
  Sprite *mark;
  char pad6a0[0x6ac - 0x6a0];
  Widget *tabs[4];
  unsigned int *tab_color;
  char pad6c0[0x6d8 - 0x6c0];
  Range *r6d8;
  Range *r6dc;
  Widget *w6e0;
  int base;
  char pad6e8[0x1850 - 0x6e8];
  int state;
  int pad1854;
  int timer;
  int pad185c;
  char sub1860[4];
  char pad1864[0x1e40 - 0x1864];
  unsigned char shown;
  char pad1e41[3];
  Range *scroll;
  int scroll_pos;
};

extern "C" {
extern ScreenMgr *DAT_003415ac;
extern Game *DAT_00338ac4;
extern int DAT_003386c8;
extern Float0c *DAT_00331b84;
extern unsigned char DAT_003417be;
extern char DAT_0034179c[];
Bank *FUN_0018c9f4(int);
void FUN_00103128(Self *);
int FUN_001155c0(ScreenMgr *);
void FUN_00103628(ScreenMgr *, int);
void FUN_0028144c(Mgr *);
int FUN_00264550(Self *);
void FUN_00258630(Self *, bool);
void FUN_00258a68(Self *);
void FUN_0010d4a8(Widget *);
void FUN_000ffb3c(void *, int);
void FUN_0025967c(Self *);
int FUN_00264c0c(Self *);
void FUN_0010b6d4(Owner *, int, int);
int FUN_000bd92c(Game *, int);
void FUN_000bcc88(Game *);
void FUN_00114178(ScreenMgr *, int, const char *, int);
void FUN_00102c54(Widget *);
void FUN_00263f8c(Self *, int);
void FUN_001b1c44(void *, int);
void FUN_00285e40(Mgr *);
int FUN_000bbba4(Game *, void *);
Speaker *FUN_0008d994(int);
void FUN_00110a78(Speaker *, const char *, int);
extern "C" float fabsf(float);
}

static inline bool IsHost()
{
  bool b = true;
  if (DAT_00338ac4 != 0 && DAT_00338ac4->host == 0) b = false;
  return b;
}

static inline void Say(const char *s)
{
  FUN_00110a78(FUN_0008d994(DAT_003386c8), s, 0);
}

static inline float Approach(float cur, float target)
{
  float d = target - cur;
  if (fabsf(d) >= 0.005f) cur += 0.5f * d;
  return cur;
}

static inline void SetRange(Range *r, int v)
{
  r->lo = v;
  r->hi = v;
}

static inline void Announce(Self *self)
{
  if (self->shown == 0) {
    self->shown = 1;
    unsigned char &once = self->mgr->announced;
    if (once == 0) {
      once = 1;
      switch (self->state) {
      case 7:
      case 8:
      case 9:
        break;
      default:
        FUN_00114178(DAT_003415ac, 2, Text(FUN_0018c9f4(0x11), 16, 13), 1);
        break;
      }
    }
  }
}

extern "C" void FUN_00264d4c(Self *self)
{
  if (self->IsActive() == 0) return;
  self->mgr->slots[0].unk16 = 1;
  self->mgr->slots[1].unk16 = 1;
  self->mgr->slots[2].unk16 = 1;
  self->mgr->slots[0].unk18 = 1;
  self->mgr->slots[1].unk18 = 1;
  self->mgr->slots[2].unk18 = 1;
  FUN_00103128(self);
  self->scroll_pos = (self->scroll_pos + 1) % 32;
  self->scroll->lo = self->scroll->hi + self->scroll_pos;
  if (!IsShown(*DAT_003415ac->pageList->pages) && FUN_001155c0(DAT_003415ac)) {
    FUN_00103628(DAT_003415ac, 1);
  }
  self->owner->Show(!IsShown(*DAT_003415ac->pageList->pages));
  FUN_0028144c(self->mgr);
  if (FUN_00264550(self) != 0) return;
  FUN_00258630(self, self->state == 4 || self->state == 3);
  FUN_00258a68(self);
  Widget *t = self->title;
  t->text = self->mgr->title;
  t->dirty = 1;
  FUN_0010d4a8(t);
  switch (self->state) {
  case 0:
  case 1:
  case 2:
  case 7:
  case 8:
  case 9:
    FUN_000ffb3c(self->w68c, 0);
    FUN_000ffb3c(self->w690, 0);
    FUN_000ffb3c(self->w694, 1);
    FUN_000ffb3c(self->w698, 1);
    self->title->SetActive(0);
    break;
  case 4:
    FUN_000ffb3c(self->w68c, 1);
    FUN_000ffb3c(self->w690, 1);
    FUN_000ffb3c(self->w694, 0);
    FUN_000ffb3c(self->w698, 1);
    self->title->SetActive(1);
    break;
  case 3:
    FUN_000ffb3c(self->w68c, 1);
    FUN_000ffb3c(self->w690, 1);
    FUN_000ffb3c(self->w694, 1);
    FUN_000ffb3c(self->w698, 0);
    self->title->SetActive(1);
    break;
  case 5:
  case 6:
    break;
  }
  if (self->state == 2) {
    for (unsigned i = 0; i < 4; i++) {
      FUN_000ffb3c(self->tabs[i], i != self->owner->cursor);
    }
    if (IsHost() && self->mgr->unk58 != 1) {
      *self->tab_color = 0x80808080;
    } else {
      *self->tab_color = 0x40808080;
      FUN_000ffb3c(self->tabs[1], 1);
      self->owner->unk120->on = 1;
    }
  } else {
    self->w658->SetActive(1);
  }
  Mgr *m = self->mgr;
  int dx = m->dx;
  int dy = m->dy;
  SetRange(self->r6d8, self->base + dx);
  SetRange(self->r6dc, self->base + dy);
  if (self->state != 4) {
    bool none = dx == 0 && dy == 0;
    FUN_000ffb3c(self->w6e0, none);
    FUN_000ffb3c(self->r6d8, none);
    FUN_000ffb3c(self->r6dc, none);
  }
  self->owner->unke8 = 0;
  ((Sub1860 *)self->sub1860)->Update();
  FUN_0025967c(self);
  if (IsHost()) {
    if (FUN_00264c0c(self)) {
      if (IsShown(self->mark)) {
        FUN_000ffb3c(self->mark, 0);
        if ((unsigned)self->state < 2) FUN_0010b6d4(self->owner, 0, 1);
        Announce(self);
      }
    } else {
      if (!IsShown(self->mark)) {
        FUN_000ffb3c(self->mark, 1);
        if ((unsigned)self->state < 2) FUN_0010b6d4(self->owner, 0, 0);
      }
      self->shown = 0;
    }
  } else {
    if (DAT_00338ac4 != 0 && FUN_000bd92c(DAT_00338ac4, DAT_00338ac4->unk198) == 0) {
      FUN_000bcc88(DAT_00338ac4);
    }
    if (FUN_00264c0c(self)) {
      Announce(self);
    } else {
      self->shown = 0;
    }
  }
  if (self->state == 7) {
    self->panel->SetActive(0);
    self->panel->x = Approach(self->panel->x, 0.0f);
    FUN_00102c54(self->panel);
    if (self->timer >= 22) FUN_00263f8c(self, 8);
    if (self->timer == 3) FUN_001b1c44(DAT_0034179c, 21);
  } else if (self->state == 8) {
    self->panel->SetActive(0);
    self->panel->x = Approach(self->panel->x, 480.0f);
    FUN_00102c54(self->panel);
    if (self->timer >= 90) FUN_00263f8c(self, 9);
  } else if (self->state == 9) {
    if (DAT_00331b84->v == 0.0f) ((unsigned char *)self->mgr)[0x2c02] = 1;
    if (self->timer >= 20) FUN_00285e40(self->mgr);
  } else if (self->state == 1 || self->state == 2 || self->state == 3 || self->state == 5) {
    self->panel->SetActive(1);
    Mgr *mm = self->mgr;
    if (((Ent *)((char *)mm + 0x954))[FUN_000bbba4(DAT_00338ac4, DAT_00338ac4->id3e)].ready) {
      FUN_00263f8c(self, 7);
    }
  } else {
    self->panel->SetActive(1);
  }
  if (self->state == 1) {
    if (self->timer >= 90) {
      if (FUN_00264c0c(self)) {
        Say(Text(FUN_0018c9f4(0x12), 46, 2));
      } else {
        Say(Text(FUN_0018c9f4(0x12), 46, 1));
      }
    } else {
      if (self->owner->cursor == 0) {
        Say(Text(FUN_0018c9f4(0x12), 47, 0));
      } else {
        Say(Text(FUN_0018c9f4(0x12), 48, 0));
      }
    }
    FUN_0008d994(DAT_003386c8)->Flush();
  } else if (self->state == 2) {
    switch (self->owner->cursor) {
    case 0:
      if (IsHost() && DAT_003417be == 0) {
        Say(Text(FUN_0018c9f4(0x12), 47, 1));
      } else {
        Say(Text(FUN_0018c9f4(0x12), 47, 5));
      }
      break;
    case 1:
      if (IsHost()) {
        Say(Text(FUN_0018c9f4(0x12), 47, 2));
      } else {
        Say(Text(FUN_0018c9f4(0x12), 47, 6));
      }
      break;
    case 2:
      Say(Text(FUN_0018c9f4(0x12), 47, 3));
      break;
    case 3:
      Say(Text(FUN_0018c9f4(0x12), 47, 4));
      break;
    }
    FUN_0008d994(DAT_003386c8)->Flush();
  } else if (self->state == 4) {
    Say(Text(FUN_0018c9f4(0x12), 47, 2));
    FUN_0008d994(DAT_003386c8)->Flush();
  } else if (self->state == 5) {
    FUN_0008d994(DAT_003386c8)->Flush();
  }
  self->timer++;
  self->frames++;
}
