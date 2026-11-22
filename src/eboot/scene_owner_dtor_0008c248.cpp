/* HSTMATCH module=EBOOT.BIN start=0x0008c248 end=0x0008c434 symbol=FUN_0008c248 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/draft_0008c248.c); exact match verified; matching idiom: the null test of each inlined member destructor is written four times (nested inlined destructor layers); names and types await human review. */
extern "C" {
extern char DAT_003070bc;
extern int DAT_00310fc4;
extern int DAT_0034a7f0;
extern int DAT_0034a820;
extern void FUN_00000788(...);
extern void FUN_00000a00(...);
extern void FUN_000429a0(...);
extern void FUN_0005ba00(...);
extern void FUN_00064a08(...);
extern void FUN_00066338(...);
extern void FUN_00102958(...);
extern void FUN_00194110(...);
extern void FUN_001b8c54(...);
extern void FUN_001fe0f4(...);
extern void FUN_00280114(...);
extern void FUN_00291cdc(...);
extern void FUN_00056cb0(...);
}
struct VC_0 { virtual ~VC_0(); };
struct VC_1 { virtual ~VC_1(); };
struct VC_2 { virtual ~VC_2(); };
struct VC_3 { virtual ~VC_3(); };
extern VC_0 *DAT_003386c0;
extern VC_1 *DAT_003415d4;

struct Leaf { int a[4]; };
struct Pair {
  void *vptr;
  int pad04[3];
  Leaf l10;
  Leaf l20;
};
struct Inner { int *p; };
struct Head { char pad[4]; Inner **q; };
struct Obj {
  VC_2 *f0;
  char pad04[0xc];
  char arr[4][0x1b0];
  char m6d0[0x990 - 0x6d0];
  Pair m990;
  char pad9b0[0x9cc - 0x9b0 - 0x10];
  int *f9cc;
};

extern "C" Obj *FUN_0008c248(Obj *self, short flag)
{
  if (self != 0) {
    if (DAT_003386c0 != 0) {
      delete DAT_003386c0;
      DAT_003386c0 = 0;
    }
    if (DAT_003415d4 != 0) {
      delete DAT_003415d4;
      DAT_003415d4 = 0;
    }
    if (DAT_0034a7f0 != 0) {
      FUN_00280114(DAT_0034a7f0, 1);
      DAT_0034a7f0 = 0;
    }
    FUN_00194110();
    FUN_00102958(*(*(int ***)((char *)self->f0 + 4)));
    if (self->f0 != 0) {
      delete self->f0;
      self->f0 = 0;
    }
    if (self->f9cc != 0) {
      FUN_001b8c54(self->f9cc, 1);
      self->f9cc = 0;
    }
    FUN_001fe0f4(0);
    FUN_000429a0(DAT_00310fc4);
    if (DAT_0034a820 != 0) {
      FUN_00291cdc(DAT_0034a820);
    }
    if (&self->m990 != 0) {
      self->m990.vptr = &DAT_003070bc;
      if (&self->m990.l20 != 0 && &self->m990.l20 != 0 && &self->m990.l20 != 0 && &self->m990.l20 != 0) {
        FUN_00066338(&self->m990.l20, 0);
      }
      if (&self->m990.l10 != 0 && &self->m990.l10 != 0 && &self->m990.l10 != 0 && &self->m990.l10 != 0) {
        FUN_00066338(&self->m990.l10, 0);
      }
      FUN_00064a08(&self->m990, 0);
    }
    FUN_0005ba00(self->arr[4], -1);
    FUN_00000788((char *)self + 0x10, FUN_00056cb0, 0x1b0, 4);
    if (flag > 0) FUN_00000a00(self);
  }
  return self;
}
