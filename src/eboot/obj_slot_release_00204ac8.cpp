/* HSTMATCH module=EBOOT.BIN start=0x00204ac8 end=0x00204c80 symbol=FUN_00204ac8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; matching idiom: the member-destructor null test is written twice (two nested inlined destructor layers); names and types await human review. */
extern "C" {
extern void *DAT_0034b340[];
extern void FUN_00000a00(...);
extern void FUN_00091c48(...);
extern void FUN_00092ff8(...);
extern void FUN_000b42f8(...);
extern void FUN_00205428(...);
}
struct VC_0 { virtual ~VC_0(); };
struct VC_1 { virtual ~VC_1(); };
struct VC_2 { virtual ~VC_2(); };
struct Slot { char pad0[4]; int idx; unsigned char b8; unsigned char b9; char pad[2]; int m0c[6]; };
struct Wnd { char pad[0x50]; int f50; };

extern "C" Slot *FUN_00204ac8(Slot *self, short flag)
{
  if (self != 0) {
    if (self->b8 != 0) {
      if (self->b9 != 0) {
        VC_2 *p = (VC_2 *)DAT_0034b340[self->idx];
        if (p != 0) {
          delete p;
          DAT_0034b340[self->idx] = 0;
        }
      } else {
        Wnd *w = (Wnd *)DAT_0034b340[self->idx];
        if (w != 0) {
          if (w->f50 != 0) {
            FUN_00091c48(w, 0);
            FUN_00092ff8(DAT_0034b340[self->idx]);
          } else {
            if (w != 0) {
              delete (VC_1 *)w;
              DAT_0034b340[self->idx] = 0;
            }
          }
        }
      }
    } else {
      VC_0 *p = (VC_0 *)DAT_0034b340[self->idx];
      if (p != 0) {
        delete p;
        DAT_0034b340[self->idx] = 0;
      }
    }
    FUN_00205428(self);
    if (&self->m0c != 0 && &self->m0c != 0) {
      FUN_000b42f8(&self->m0c, 0);
    }
    if (flag > 0) FUN_00000a00(self);
  }
  return self;
}
