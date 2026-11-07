/* HSTMATCH module=hstp_trans_main.prx start=0x0026e5f8 end=0x0026e6d0 symbol=FUN_0026e5f8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern char DAT_000519d4, DAT_003b2c68, DAT_003eaad8;
extern void *DAT_003e1a08;
extern void FUN_001c20b8(...);
extern void FUN_001c0eb8(...);
extern void FUN_000006e8(...);
extern void FUN_0001ed28(...);
extern void FUN_00000a00(...);
}
struct Obj3 { virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); };
extern "C" Obj3 *FUN_0008f170(void *);
struct VC_0 { virtual ~VC_0(); };
struct Self { void *vtbl; char pad[0x3c]; VC_0 *m40; void *m44; void *m48; void *m4c; };
extern "C" Self *FUN_0026e5f8(Self *self, short flag)
{
  if (self != 0) {
    self->vtbl = &DAT_003b2c68;
    FUN_001c20b8(&DAT_003eaad8);
    if (self->m4c != 0) {
      FUN_000006e8(self->m4c, &DAT_000519d4);
      self->m4c = 0;
    }
    if (self->m40 != 0) {
      delete self->m40;
      self->m40 = 0;
    }
    FUN_001c0eb8(&DAT_003eaad8);
    FUN_0008f170(DAT_003e1a08)->s1();
    FUN_0001ed28(self, 0);
    if (0 < flag) {
      FUN_00000a00(self);
    }
  }
  return self;
}
