/* HSTMATCH module=hstp_trans_main.prx start=0x000648f4 end=0x000649e4 symbol=FUN_000648f4 lang=c99 */
/* WIP near miss: 60/60 words, 3 differ (+0x18, +0x1c, +0x20).
   The entire body (57 words) matches retail exactly, including register colors:
   s1 = self, s0 = count, $f20 = param_2.
   The only difference is the prologue move order:
   ours:   move s1, a0; move s0, a1; mov.s $f20, $f12
   retail: move s0, a1; mov.s $f20, $f12; move s1, a0
   Tried: declaration order variations, type-converted locals (as in sibling 0x649e4),
   register keywords, signed/unsigned permutations, and C++ member functions.
*/

typedef unsigned int uint;
typedef struct { float x; float y; float z; float w; } Vec4;
typedef struct { uint x; uint y; uint z; uint w; } UInt4;

extern int FUN_000648d0(void);
extern void FUN_00064418(Vec4 *, char *);
extern Vec4 FUN_0004c898(const Vec4 *);
extern void FUN_00051d40(uint, int);

void FUN_000648f4(char *param_1, int param_3, float param_2)
{
  unsigned int self;
  unsigned int count;
  Vec4 transformed;
  UInt4 identity;
  Vec4 input;
  Vec4 *source;

  count = (unsigned int)param_3;
  self = (unsigned int)param_1;

  identity.w = 0x3f800000;
  identity.x = 0;
  identity.y = 0;
  identity.z = 0;

  if (FUN_000648d0() != 0) {
    source = (Vec4 *)&identity;
  } else {
    FUN_00064418(&input, (char *)self);
    source = &input;
  }
  transformed = FUN_0004c898(source);

  *(float *)(self + 0x20) = transformed.x;
  *(float *)(self + 0x24) = transformed.y;
  *(float *)(self + 0x28) = transformed.z;
  *(float *)(self + 0x2c) = transformed.w;

  *(uint *)(self + 0x30) = 0;
  *(uint *)(self + 0x34) = 0;
  *(uint *)(self + 0x38) = 0;
  *(uint *)(self + 0x3c) = 0;

  *(float *)(self + 8) = (float)(int)count;
  *(float *)(self + 0xc) = (float)(int)count;
  *(uint *)(self + 0x10) = 0xbf800000;

  FUN_00051d40(*(uint *)(self + 4), 0);

  *(unsigned char *)(self + 0x40) = 0;
  *(unsigned char *)(self + 0x41) = 0;
  *(uint *)(self + 0x44) = 0x457a0000;
  *(unsigned char *)(self + 0x50) = 0;
  *(unsigned char *)(self + 0x51) = 0;
  *(unsigned char *)(self + 0x52) = 1;
  *(unsigned char *)(self + 0x53) = 1;
  *(float *)(self + 0x54) = param_2;
}
// near miss from lane ag6 (unreviewed)
// lane pn3 (2026-11-10, ~6 compiles here plus ~38 on the identical-shape Sharing 0x647e0): 3 -> 2 words. The two
// int params are copied into `unsigned` locals assigned count-then-self (declared self-then-count): MWCC copies
// locals in ASSIGNMENT order and colours them by DECLARATION order (first = s1), which gives retail's s1=self,
// s0=count and the a1-before-a0 order; the float param copy (`mov.s $f20,$f12`) is a parameter-phase copy and
// always lands FIRST (retail: between the two int copies). A float local assigned between them is coalesced with
// the param. The other way round (count left a parameter, self a converted local, as in the EXACT sibling
// src/sharing_main/setup_transform_000649e4.c) gives retail's order but param count=s1/self=s0 (24 words): params
// always colour above locals. See wip/setup_transform_000647e0.c for the full ruled-out list.
// improved by lane pn3 (unreviewed): 3 -> 2 differing words
