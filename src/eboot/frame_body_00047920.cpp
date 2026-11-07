/* HSTMATCH module=EBOOT.BIN start=0x00047920 end=0x00047d58 symbol=FUN_00047920 lang=c++ */
/* Hand-written reconstruction from the disassembly (decomp-opus lane 2026-11-04, Claude Opus 5.5); the state test is a switch with its cases in reverse order (retail's beq chain) and FUN_00047d58 takes pointer arguments; exact match verified; names and types await human review. */
/* 60 Hz note: once per frame. playTime (save +0x48) counts frames, clamped at 0xfffffff0. DAT_00310fe0 selects the rate passed to FUN_00062150: 30.0f when 0, 60.0f when set. DAT_0031101c counts frames while FUN_000487c8() returns 0. */
struct Pad { int pad[0x80 / 4]; unsigned int held; };
struct Ui {
  virtual void v0();
  virtual void v1();
  virtual void Update();
  virtual void Draw();
};
struct Gfx {
  virtual void v0();
  virtual void Begin();
  virtual void Update();
  virtual void End();
};
struct Save { int pad[0x48 / 4]; unsigned int playTime; };

extern "C" {
void FUN_002b78f0(void);
void FUN_002b7888(int);
void FUN_0001dc00(void);
void FUN_002b79f0(int);
void FUN_0001f394(void *, int);
void FUN_00063750(int);
void FUN_00292fa0(int);
void FUN_00060224(int, int);
int FUN_000487c8(void);
void FUN_00047d58(void *, void *, ...);
void FUN_0005bc58(int);
void FUN_00050544(void *);
void FUN_000637f0(int);
void FUN_002930a0(int);
void FUN_00000038(void);
void FUN_0001d7dc(void);
void FUN_0001d7d4(void);
void FUN_00050b2c(void *);
void FUN_00062150(float);
void FUN_00050d8c(void *, int);
void FUN_000c15e0(int);
int FUN_002b7c40(int);
void FUN_00046564(char *, int);
void FUN_0029b05c(int);
void FUN_002a4ee4(int);
void FUN_00048258(void);
int FUN_002b7a60(void);
void FUN_00291298(int);
void FUN_00050bec(void *, void *, int, int);
}

extern int DAT_00310fd8;
extern Pad *DAT_002cfc8c;
extern Ui *DAT_003415ac;
extern Gfx *DAT_00310fc4;
extern char DAT_0030fe20[];
extern int DAT_00331b84;
extern int DAT_00331b88;
extern int DAT_0034a820;
extern void (*DAT_00310ff8)(void);
extern unsigned char DAT_00310ff4;
extern int DAT_0034a678;
extern Save *DAT_003415a8;
extern int DAT_00311000;
extern char DAT_0030fc10[];
extern char DAT_002d06d4[];
extern int DAT_003313c4;
extern char DAT_00311170[];
extern int DAT_00310fe0;
extern unsigned char DAT_00341770;
extern char DAT_002d06dc[];
extern int DAT_0034b328;
extern int DAT_0034b350;
extern int DAT_00311058;
extern int DAT_0034a818;
extern char DAT_0030ffc0[];
extern unsigned char DAT_002d0224;
extern unsigned int DAT_0031101c;

extern "C" void FUN_00047920(int resetFrame, int active, int draw)
{
  FUN_002b78f0();
  if (DAT_00310fd8)
    FUN_002b7888(100);
  if (active && !DAT_00310fd8) {
    FUN_0001dc00();
    if (DAT_002cfc8c->held & 0xf0000)
      FUN_002b79f0(6);
  }
  DAT_003415ac->Update();
  if (resetFrame) {
    FUN_0001f394(DAT_0030fe20, 0);
    FUN_00063750(DAT_00331b84);
    FUN_00063750(DAT_00331b88);
    FUN_00292fa0(DAT_0034a820);
    DAT_00310fc4->Begin();
  }
  FUN_0001f394(DAT_0030fe20, 1);
  DAT_00310fc4->Update();
  void (*callback)(void) = DAT_00310ff8;
  if (callback) {
    DAT_00310ff8 = 0;
    DAT_00310ff4 = 1;
    callback();
    DAT_00310ff4 = 0;
  }
  switch (DAT_0034a678) {
  case 0:
  case 2:
  case 0x20:
  case 0x2a:
  case 0x31:
  case 0x44:
    DAT_003415a8->playTime++;
    if (DAT_003415a8->playTime > 0xfffffff0)
      DAT_003415a8->playTime = 0xfffffff0;
    break;
  }
  if (active && !DAT_00310fd8) {
    if (DAT_00311000)
      FUN_00060224(DAT_00311000, 0);
    if (FUN_000487c8())
      FUN_00047d58(DAT_0030fc10, DAT_002d06d4);
    FUN_0005bc58(DAT_003313c4);
    FUN_00050544(DAT_00311170);
    if (draw) {
      FUN_0001f394(DAT_0030fe20, 2);
      if (!DAT_00310ff4) {
        DAT_003415ac->Draw();
        FUN_000637f0(DAT_00331b84);
        FUN_000637f0(DAT_00331b88);
        FUN_002930a0(DAT_0034a820);
        DAT_00310fc4->End();
      }
    }
    FUN_00000038();
    FUN_0001d7dc();
    FUN_0001d7d4();
    FUN_00050b2c(DAT_00311170);
    if (active && !DAT_00310fd8) {
      float rate = 30.0f;
      if (DAT_00310fe0)
        rate = 60.0f;
      FUN_00062150(rate);
    }
    FUN_00050d8c(DAT_00311170, 0);
    FUN_000c15e0(2);
    if (DAT_00341770) {
      int v = FUN_002b7c40(2);
      if (v)
        FUN_00046564(DAT_002d06dc, v);
    }
    if (DAT_0034b328)
      FUN_0029b05c(DAT_0034b328);
    if (DAT_0034b350)
      FUN_002a4ee4(DAT_0034b350);
    int noImage = DAT_00310fe0 == 0;
    FUN_00048258();
    DAT_00311058 = FUN_002b7a60();
    if (DAT_0034a818)
      FUN_00291298(DAT_0034a818);
    FUN_00050bec(DAT_00311170, DAT_0030ffc0, 0x1000, noImage);
    DAT_002d0224 = 1;
  }
  if (!FUN_000487c8())
    DAT_0031101c++;
}
