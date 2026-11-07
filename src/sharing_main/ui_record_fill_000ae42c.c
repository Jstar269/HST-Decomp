/* HSTMATCH module=hstp_trans_main.prx start=0x000ae42c end=0x000ae658 symbol=FUN_000ae42c lang=c99 */
/* Hand-written reconstruction: clone of the source of ui_record_fill_000aba38.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Found with hstnearperm.py (tag d2) from the lane-sx1 near miss; the one change that makes it exact is the statement order at the top: both halfword loads after the memset call (field 328 first), then `w.b0 = 0;`. Exact match verified; names and types await human review. */
typedef unsigned char undefined;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long long ulonglong;
typedef long long longlong;
typedef unsigned char uchar;
typedef unsigned char bool;
typedef unsigned long long undefined8;
typedef int code();
#define true 1
#define false 0
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

extern int DAT_003e19f0;
extern int DAT_003e1e84;

typedef struct { float x, y, z, w; } Vec4;

typedef struct {
  Vec4 a;
  Vec4 b;
  int z0;
  int z1;
  int z2;
  int w44;
  char b0 : 1;
  char b1 : 2;
  char b2 : 3;
  short b3 : 3;
  char e : 5;
  char f : 2;
  char h : 1;
  char i : 1;
  char j : 1;
  short g : 8;
  short s52;
  unsigned char c54;
  unsigned char c55;
  char pad[8];
} Rec;

extern void FUN_000918d8(int, float *, float *);
extern void FUN_000114c0(void *, int, int);
extern void FUN_00054e7c(void *, float *);
extern void FUN_000a2a0c(void *, int);
extern void FUN_001228c0(int, void *, int, int);

void FUN_000ae42c(int param_1, int param_2, int param_3)
{
  float va[4];
  float vb[4];
  Rec w;
  int s0;
  int s1;

  *(Vec4 *)va = *(Vec4 *)param_2;
  *(Vec4 *)vb = *(Vec4 *)param_3;
  FUN_000918d8(*(int *)(DAT_003e19f0 + 0x88), va, vb);
  if (*(int *)(DAT_003e19f0 + 0x3c) == 7) return;
  w.w44 = 1;
  FUN_000114c0(&w, 0, 0x40);
  s1 = *(short *)(param_1 + 0x148);
  s0 = *(short *)(param_1 + 0x144);
  w.b0 = 0;
  FUN_00054e7c(&w, va);
  FUN_00054e7c(&w.b, vb);
  w.b1 = s0;
  w.b2 = s1;
  w.b3 = 0;
  w.e = 0;
  w.g = 0;
  w.f = 1;
  w.h = 0;
  w.i = 0;
  w.j = 0;
  w.z0 = 0;
  w.z1 = 0;
  w.z2 = 0;
  w.s52 = 0;
  FUN_000a2a0c((char *)&w + 44, param_1 + 0x168);
  w.c54 = 0;
  w.c55 = 0;
  FUN_001228c0(*(int *)(DAT_003e19f0 + 0x9c), &w, !*(unsigned char *)(DAT_003e1e84 + 0x5c), 1);
  return;
}
// Levers: the 64-byte work record is a struct with bitfields declared in memory order with plain (signed) char / short container types (signed containers
// give retail's lb/lh + seb); every literal 0 assignment is kept (mwcc emits `andi r,zero,N` for each); `w.s52 = 0;` before the FUN_000a2a0c call fills its jal
// slot; `!*(unsigned char *)` for the sltu/xori argument. The two halfword loads come after the memset call, field 328 before field 324, with `w.b0 = 0;` after them.
