/* HSTMATCH module=hstp_trans_main.prx start=0x00047da4 end=0x000481dc symbol=FUN_00047da4 lang=c++ */
/* Hand-written reconstruction: clone of the source of frame_body_00047920.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (decomp-opus lane 2026-11-04, Claude Opus 5.5); the state test is a switch with its cases in reverse order (retail's beq chain) and FUN_000481dc takes pointer arguments; exact match verified; names and types await human review. */
/* 60 Hz note: once per frame. playTime (save +0x48) counts frames, clamped at 0xfffffff0. DAT_003ba2e0 selects the rate passed to FUN_00062fd4: 30.0f when 0, 60.0f when set. DAT_003ba31c counts frames while FUN_00048958() returns 0. */
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
void FUN_002c7690(void);
void FUN_002c7628(int);
void FUN_0001d84c(void);
void FUN_002c7790(int);
void FUN_0001f1cc(void *, int);
void FUN_00064614(int);
void FUN_002a45ac(int);
void FUN_00061070(int, int);
int FUN_00048958(void);
void FUN_000481dc(void *, void *, ...);
void FUN_0005c8e0(int);
void FUN_00050e20(void *);
void FUN_000646b4(int);
void FUN_002a46ac(int);
void FUN_00000038(void);
void FUN_0001d428(void);
void FUN_0001d420(void);
void FUN_00051440(void *);
void FUN_00062fd4(float);
void FUN_000516a0(void *, int);
void FUN_000c4c80(int);
int FUN_002c79e0(int);
void FUN_00046664(char *, int);
void FUN_002aa2cc(int);
void FUN_002b4674(int);
void FUN_000483e8(void);
int FUN_002c7800(void);
void FUN_002a29a0(int);
void FUN_00051500(void *, void *, int, int);
}

extern int DAT_003ba2d8;
extern Pad *DAT_003751d4;
extern Ui *DAT_003ea8ec;
extern Gfx *DAT_003ba2c4;
extern char DAT_003b9120[];
extern int DAT_003daec4;
extern int DAT_003daec8;
extern int DAT_003f3b50;
extern void (*DAT_003ba2f8)(void);
extern unsigned char DAT_003ba2f4;
extern int DAT_003f39a8;
extern Save *DAT_003ea8e8;
extern int DAT_003ba300;
extern char DAT_003b8f08[];
extern char DAT_00375e3c[];
extern int DAT_003da704;
extern char DAT_003ba4b0[];
extern int DAT_003ba2e0;
extern unsigned char DAT_003eaab0;
extern char DAT_00375e44[];
extern int DAT_003f4668;
extern int DAT_003f469c;
extern int DAT_003ba380;
extern int DAT_003f3b48;
extern char DAT_003b92c0[];
extern unsigned char DAT_0037596c;
extern unsigned int DAT_003ba31c;

extern "C" void FUN_00047da4(int resetFrame, int active, int draw)
{
  FUN_002c7690();
  if (DAT_003ba2d8)
    FUN_002c7628(100);
  if (active && !DAT_003ba2d8) {
    FUN_0001d84c();
    if (DAT_003751d4->held & 0xf0000)
      FUN_002c7790(6);
  }
  DAT_003ea8ec->Update();
  if (resetFrame) {
    FUN_0001f1cc(DAT_003b9120, 0);
    FUN_00064614(DAT_003daec4);
    FUN_00064614(DAT_003daec8);
    FUN_002a45ac(DAT_003f3b50);
    DAT_003ba2c4->Begin();
  }
  FUN_0001f1cc(DAT_003b9120, 1);
  DAT_003ba2c4->Update();
  void (*callback)(void) = DAT_003ba2f8;
  if (callback) {
    DAT_003ba2f8 = 0;
    DAT_003ba2f4 = 1;
    callback();
    DAT_003ba2f4 = 0;
  }
  switch (DAT_003f39a8) {
  case 0:
  case 2:
  case 0x20:
  case 0x2a:
  case 0x31:
  case 0x44:
    DAT_003ea8e8->playTime++;
    if (DAT_003ea8e8->playTime > 0xfffffff0)
      DAT_003ea8e8->playTime = 0xfffffff0;
    break;
  }
  if (active && !DAT_003ba2d8) {
    if (DAT_003ba300)
      FUN_00061070(DAT_003ba300, 0);
    if (FUN_00048958())
      FUN_000481dc(DAT_003b8f08, DAT_00375e3c);
    FUN_0005c8e0(DAT_003da704);
    FUN_00050e20(DAT_003ba4b0);
    if (draw) {
      FUN_0001f1cc(DAT_003b9120, 2);
      if (!DAT_003ba2f4) {
        DAT_003ea8ec->Draw();
        FUN_000646b4(DAT_003daec4);
        FUN_000646b4(DAT_003daec8);
        FUN_002a46ac(DAT_003f3b50);
        DAT_003ba2c4->End();
      }
    }
    FUN_00000038();
    FUN_0001d428();
    FUN_0001d420();
    FUN_00051440(DAT_003ba4b0);
    if (active && !DAT_003ba2d8) {
      float rate = 30.0f;
      if (DAT_003ba2e0)
        rate = 60.0f;
      FUN_00062fd4(rate);
    }
    FUN_000516a0(DAT_003ba4b0, 0);
    FUN_000c4c80(2);
    if (DAT_003eaab0) {
      int v = FUN_002c79e0(2);
      if (v)
        FUN_00046664(DAT_00375e44, v);
    }
    if (DAT_003f4668)
      FUN_002aa2cc(DAT_003f4668);
    if (DAT_003f469c)
      FUN_002b4674(DAT_003f469c);
    int noImage = DAT_003ba2e0 == 0;
    FUN_000483e8();
    DAT_003ba380 = FUN_002c7800();
    if (DAT_003f3b48)
      FUN_002a29a0(DAT_003f3b48);
    FUN_00051500(DAT_003ba4b0, DAT_003b92c0, 0x1000, noImage);
    DAT_0037596c = 1;
  }
  if (!FUN_00048958())
    DAT_003ba31c++;
}
