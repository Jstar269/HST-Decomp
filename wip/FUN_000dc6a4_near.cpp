/* HSTMATCH module=EBOOT.BIN start=0x000dc6a4 end=0x000dc820 symbol=FUN_000dc6a4 lang=c++ */
/* status: CURRENT - 95/95 words, 46 differ, +0x30; nine shapes measured, block-layout wall. */
/* Fresh reconstruction from the disassembly; best variant 95/95 words, 46 differ, first diff +0x30. */
typedef unsigned char undefined;
typedef unsigned int undefined4;

extern "C" {
extern int *DAT_003386b0;
extern unsigned char DAT_0034189b;
extern unsigned char DAT_003419bb;
extern void FUN_000114c0(int,int,int);
extern void FUN_000db868(int);
}

extern "C" void FUN_000dc6a4(int param_1,int param_2)
{
  int *g;
  int v;

  *(int *)(param_1 + 60) = param_2;
  if (param_2 == 3) {
    return;
  }
  if (param_2 == 2) {
    return;
  }
  if (param_2 != 1) {
    *(unsigned char *)(param_1 + 68) = 1;
    if (param_2 == 0) {
      g = DAT_003386b0;
      *(int *)(param_1 + 64) = g[34];
      FUN_000114c0(param_1 + 216,0,48);
      *(int *)(param_1 + 208) = DAT_0034189b;
      *(int *)(param_1 + 196) = 0;
      *(int *)(param_1 + 184) = 0;
      *(int *)(param_1 + 212) = DAT_003419bb;
      *(int *)(param_1 + 200) = 0;
      *(int *)(param_1 + 188) = 0;
      *(int *)(param_1 + 72) = 1;
      *(int *)(param_1 + 76) = 1;
      *(int *)(param_1 + 80) = 0;
      *(int *)(param_1 + 84) = 0;
      *(int *)(param_1 + 88) = 1;
      *(unsigned char *)(param_1 + 264) = 0;
      FUN_000db868(param_1);
      return;
    }
    return;
  }
  if (*(unsigned char *)(param_1 + 176) != 0) {
    *(unsigned char *)(param_1 + 176) = 0;
    v = *(int *)(param_1 + 72);
    *(int *)(param_1 + 72) = -v;
  }
  *(int *)(param_1 + 80) = *(int *)(param_1 + 84);
  if (0 <= *(int *)(param_1 + 156)) {
    *(unsigned char *)(param_1 + 92) = 0;
    FUN_000db868(param_1);
    return;
  }
  *(int *)(param_1 + 144) = 0;
  *(int *)(param_1 + 148) = -1;
  *(int *)(param_1 + 152) = -1;
  *(int *)(param_1 + 172) = 0;
  *(unsigned char *)(param_1 + 100) = 0;
  *(unsigned char *)(param_1 + 101) = 0;
  *(unsigned char *)(param_1 + 102) = 0;
  *(unsigned char *)(param_1 + 103) = 0;
  *(int *)(param_1 + 104) = 3;
  *(int *)(param_1 + 108) = 13;
  *(unsigned char *)(param_1 + 112) = 0;
  *(int *)(param_1 + 120) = -1;
  *(int *)(param_1 + 124) = 0;
  *(int *)(param_1 + 128) = 0;
  *(unsigned char *)(param_1 + 96) = 0;
  *(unsigned char *)(param_1 + 97) = 0;
  *(unsigned char *)(param_1 + 98) = 0;
  *(unsigned char *)(param_1 + 99) = 0;
  *(unsigned char *)(param_1 + 113) = 0;
  *(unsigned char *)(param_1 + 114) = 0;
  *(unsigned char *)(param_1 + 115) = 0;
  *(unsigned char *)(param_1 + 116) = 0;
  *(unsigned char *)(param_1 + 117) = 0;
  *(unsigned char *)(param_1 + 93) = 0;
  *(unsigned char *)(param_1 + 94) = 0;
  *(unsigned char *)(param_1 + 132) = 0;
  *(unsigned char *)(param_1 + 133) = 0;
  return;
}
// parked: 95/95 46 differ; store-before-test reorder family already tried (worse: 53, +0x14). Retail hoists sb v1,68(s0) above beqzl a1; all block-orderings measured.
// near miss from lane dsn3 (unreviewed)
