/* HSTMATCH module=EBOOT.BIN start=0x001f2f08 end=0x001f3144 symbol=FUN_001f2f08 lang=c++ */
extern "C" {
extern char DAT_0030ab9c;
extern char DAT_0030ebf4;
extern char *DAT_003386b0;
extern char *DAT_003415ac;
extern void FUN_00000a00(...);
extern void FUN_000104e0(...);
extern void FUN_0001bfb4(...);
extern void FUN_0001cb88(...);
extern int FUN_0001cbfc(...);
extern int FUN_0001cc54(...);
extern void FUN_00092ff8(...);
extern void FUN_001b5758(...);
extern void FUN_001ee0f8(...);
extern void FUN_001f4bbc(...);
extern void FUN_001fb44c(...);
extern void FUN_001fb7cc(...);
}
struct VC_0 { virtual ~VC_0(); };
struct VC_1 { virtual ~VC_1(); };
struct Alloc {};
struct Vec { int *data; unsigned size; void shrink(int n, Alloc) { size -= n; } };

struct Inner { char pad[0x60]; float f60; float f64; };
struct Holder { char pad[4]; Inner **pp; };
struct Cam { char pad[0x74]; float f74; float f78; };
struct Sub { char pad[0x90]; int *f90; };
struct Misc { char pad[0xa8]; int fa8; };

struct Scene {
  char pad000[0x39c];
  void *f39c;
  int a[4];
  int b[4];
  int f3c0;
  char pad3c4[0x510 - 0x3c4];
  int *f510;
  char pad514[0x584 - 0x514];
  Sub *f584;
  char pad588[0x1368 - 0x588];
  void *f1368;
  char pad136c[0x8184 - 0x136c];
  Vec vec8184;
};

extern "C" void FUN_001f2f08(Scene *self)
{
  FUN_001fb44c();
  FUN_001f4bbc(self);
  Cam *cam = (Cam *)DAT_003415ac;
  if (cam != 0) {
    cam->f74 = 1.0f;
    cam->f78 = 1.0f;
    Holder *h = (Holder *)DAT_003415ac;
    (*h->pp)->f60 = 240.0f;
    (*h->pp)->f64 = 136.0f;
  }
  FUN_0001cb88(&DAT_0030ebf4);
  {
    int n = self->vec8184.size;
    self->vec8184.shrink(n, Alloc());
  }
  if (self->f1368 != 0) {
    FUN_00000a00(self->f1368);
    self->f1368 = 0;
  }
  Sub *s = self->f584;
  if (s->f90 != 0) {
    delete (VC_0 *)s->f90;
    s->f90 = 0;
  }
  if (self->f510 != 0) {
    delete (VC_1 *)self->f510;
    self->f510 = 0;
  }
  Misc *misc = (Misc *)DAT_003386b0;
  int arg = misc != 0 ? misc->fa8 : 0;
  int i;
  for (i = 0; i < 2; i++) {
    FUN_001fb7cc((char *)self + i * 0xe4 + 0x10fc);
  }
  for (i = 3; i >= 0; i--) {
    if (self->a[i] != 0) {
      if (arg != 0) FUN_001b5758(arg, i);
      FUN_00092ff8(self->a[i]);
      int *p = (int *)self->b[i];
      if (p == 0) {
        self->a[i] = 0;
      } else {
        if (p != 0) {
          FUN_001ee0f8(p);
          FUN_00000a00(p);
        }
        self->b[i] = 0;
        self->a[i] = 0;
      }
    } else {
      int *p = (int *)self->b[i];
      if (p != 0) {
        if (p != 0) {
          FUN_001ee0f8(p);
          FUN_00000a00(p);
        }
        self->b[i] = 0;
      }
    }
  }
  self->f3c0 = 0;
  if (self->f39c != 0) {
    int x = FUN_0001cbfc();
    int y = FUN_0001cc54();
    FUN_0001bfb4(&DAT_0030ab9c, x, y);
    if (self->f39c != 0) {
      FUN_000104e0(self->f39c);
      self->f39c = 0;
    }
  }
}

// near miss api-hstm-20261121: 144/143 words, 82 differ (was 95): the structure matches; what is left is the saved-register
// colouring (retail self=s3, arg=s2, i=s1, loop pointer=s0, p=s4; ours self=s1, arg=s0, i=s3, ptr=s2) plus one extra word.
// Found: the slot-array loop tests `a[i] != 0` first (retail's beqzl jumps to the b-only arm laid out after), the
// vec8184 clear is `int n = size; shrink(n, Alloc())` (the 48-byte frame), `(*h->pp)->f60 = 240.0f` stores. Declaration orders of
// p/arg/i, an explicit char* walking pointer, and index forms did not move the colouring.
// improved by lane api-hstm-20261121 (unreviewed): 95 -> 82 differing words
