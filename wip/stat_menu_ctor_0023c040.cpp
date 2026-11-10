/* HSTMATCH module=EBOOT.BIN start=0x0023c040 end=0x0023c2ac symbol=FUN_0023c040 lang=c++ */
/* Hand-written reconstruction from the disassembly (Ghidra pseudocode ram-0x0023c040.c as a guide).
   NEAR MISS (lane pn4): 155/155 words, 9 differ, all in the final rect block (+0x244..+0x270). */
struct Item {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5(int);
  virtual void v6(float);
};

struct Gfx {
  virtual void v0();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual void v8(int, int, int, int, int);
  char pad[0x5c];
  float x;
  float y;
};

struct Area {
  int pad;
  int w;
  int h;
  int a;
  int b;
  int x;
  int y;
  void Set(short w_, short h_, int x_, int y_) { a = 0; b = 0; w = w_; h = h_; x = x_; y = y_; }
};

extern "C" {
extern char DAT_0030ebf4[];
extern void *DAT_0034a6c0;
extern char DAT_002f548c[];
extern char DAT_002f54a0[];
extern char DAT_002f54c8[];
extern char *DAT_002f5468[];
extern char DAT_002c9f9c[];
extern char DAT_002ca16c[];
extern char DAT_002ca424[];
extern char DAT_002ca448[];
extern char DAT_002ca46c[];
extern int DAT_003386c8;
extern int DAT_0034a114;

void FUN_0001cad0(char *, int, char *);
void FUN_0001cb88(char *);
void FUN_002172f4(void *, char *, int);
void FUN_00104118(void *, char *, int);
void FUN_00103540(void *, float);
void FUN_001037c8(void *, int);
void *FUN_0008d994(int);
void *FUN_001cd804(int);
void FUN_00216e90(void *, void *, void *, int);
void FUN_00216f14(void *, void *, void *, int);
void FUN_00216fb0(void *, void *, void *, int);
void FUN_002170ec(void *, void *, void *, int);
void FUN_0021704c(void *, void *, void *, int);
void FUN_00217188(void *, void *, void *);
void FUN_00217294(void *, void *, void *);
void FUN_00103628(void *, int);

int FUN_0023c040(int param_1)
{
  FUN_0001cad0(DAT_0030ebf4, 1, DAT_002f548c);
  FUN_002172f4(&DAT_0034a6c0, DAT_002f54a0, 0xcb);
  FUN_00104118(*(void **)(param_1 + 4), DAT_002f54c8, 0);
  FUN_00103540(*(void **)(param_1 + 4), 2000.0f);
  FUN_001037c8(*(void **)(param_1 + 4), 0x10);
  FUN_0001cb88(DAT_0030ebf4);
  if (DAT_003386c8 != 0) {
    *(void **)(param_1 + 0x1750) = FUN_0008d994(DAT_003386c8);
  } else if (DAT_0034a114 != 0) {
    *(void **)(param_1 + 0x1750) = FUN_001cd804(DAT_0034a114);
  }
  if (*(void **)(param_1 + 0x1750) != 0) {
    FUN_00103540(*(void **)(param_1 + 0x1750), 2300.0f);
  }
  FUN_00216e90((void *)(param_1 + 8), *(void **)(param_1 + 4), DAT_002f5468, 9);
  FUN_00216f14((void *)(param_1 + 0x2c), (void *)(param_1 + 8), DAT_0034a6c0, 0xcb);
  FUN_00216fb0((void *)(param_1 + 0x1008), (void *)(param_1 + 8), DAT_002c9f9c, 0xe);
  FUN_002170ec((void *)(param_1 + 0x10b0), (void *)(param_1 + 8), DAT_002ca16c, 0x15);
  FUN_0021704c((void *)(param_1 + 0x11ac), (void *)(param_1 + 8), DAT_002ca424, 1);
  FUN_00217188((void *)(param_1 + 0x11b8), (void *)(param_1 + 8), DAT_002ca448);
  FUN_00217294((void *)(param_1 + 0x11bc), (void *)(param_1 + 8), DAT_002ca46c);

  Item *it = *(Item **)(param_1 + 0x11b8);
  *(int *)((char *)it + 0xd8) = 0;
  *(char *)((char *)it + 0x114) = 1;
  (*(Item **)(param_1 + 0x11b8))->v6(2300.0f);
  (*(Item **)(param_1 + 0x11b8))->v5(-256);

  short *r = *(short **)(param_1 + 0xe8c);
  (*(Gfx **)(param_1 + 0x20))->v8(0, r[2], r[3], r[4], r[5]);

  (*(Area **)(param_1 + 0x14c0))->Set((*(short **)(param_1 + 0xdc4))[4] - (*(short **)(param_1 + 0xdc4))[2], (*(short **)(param_1 + 0xdc4))[5] - (*(short **)(param_1 + 0xdc4))[3], (int)(*(Gfx **)(param_1 + 0x20))->x, (int)(*(Gfx **)(param_1 + 0x20))->y);

  short *rb = *(short **)(param_1 + 0xa04);
  *(int *)(param_1 + 0x174c) = (short)(rb[4] - rb[2]);
  FUN_00103628(*(void **)(param_1 + 4), 0);
  return 1;
}
}
// near miss from lane fm4 (unreviewed)
// pn4: was 19 words. The rect block is an inline member `Area::Set(short w, short h, int x, int y)` called with
// the three pointers re-read inline (no locals): that gives retail's gfx-in-a0 / rc-in-v1 / dst-in-v0 colouring and
// load order (y +100 first). Body order a,b,w,h,x,y = 9 words; a,b,x,y,w,h (retail's store order) = 12.
// Remaining: retail truncates x (f0) first and seh's w/h into fresh regs a2/a3, storing 12,16,20,24 then 4,8;
// ours seh's in place and stores w/h before x/y.
// Ruled out (all 12-23 words): locals for any subset of dst/g/rc (all 7 subsets), Set parameter orders
// xywh/hwxy/whyx/yxhw/xyhw/hwyx, float params truncated inside, const float& params, Vec2 member by value/const&,
// int params with (short) casts at the call, nested SetRect/SetSize inlines, two separate caller statements.
// improved by lane pn4 (unreviewed): 19 -> 9 differing words
