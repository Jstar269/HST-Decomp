/* HSTMATCH module=EBOOT.BIN start=0x00170068 end=0x00170344 symbol=FUN_00170068 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   Matching idiom: the two 4-short copies are `struct { short a,b,c,d; }` assignments (retail emits 4x lh hoisted
   then 4x sh), and the type must be declared at FILE scope so both copies share it; four separate u16 stores
   instead give lhu/sh pairs, and a `short s[4]` struct gives 2x lw/sw. */
#include "hst/types.h"
typedef int code();
struct S4 { short a, b, c, d; };

extern int DAT_0034a77c;
extern void *DAT_0030ebf4;
extern void *DAT_002e05e0;
extern void *DAT_0034179c;
extern int FUN_0024dd40(int, float);
extern int FUN_0024de88(int, int, int);
extern int FUN_0016bb88();
extern void FUN_00011090(void *, void *, int);
extern void FUN_0017523c(int, void *);
extern void FUN_0001ca58(void *, void *);
extern void FUN_001b340c(void *, u32);
extern void FUN_0001cb88(void *);
extern void FUN_00171588(int, int);
extern void FUN_0017c284(u32);

void FUN_00170068(int param_1)
{
  u32 uVar4;
  u32 uVar5;
  int iVar5;
  int index;

  uVar4 = (**(code **)(param_1 + 0x558))();
  iVar5 = (**(code **)(param_1 + 1000))(uVar4);
  index = FUN_0024dd40(DAT_0034a77c, *(float *)(iVar5 + 4));
  if (FUN_0024de88(DAT_0034a77c, 9, index) != 0) {
    *(int *)(param_1 + 0x5bc) = 5;
    (*(int **)(param_1 + 0x5c0))[0] = 0;
    (*(int **)(param_1 + 0x5c0))[1] = 1;
    (*(int **)(param_1 + 0x5c0))[2] = 2;
    (*(int **)(param_1 + 0x5c0))[3] = 3;
    (*(int **)(param_1 + 0x5c0))[4] = 4;
  }
  else {
    *(int *)(param_1 + 0x5bc) = 4;
    (*(int **)(param_1 + 0x5c0))[0] = 0;
    (*(int **)(param_1 + 0x5c0))[1] = 1;
    (*(int **)(param_1 + 0x5c0))[2] = 2;
    (*(int **)(param_1 + 0x5c0))[3] = 0x7fffffff;
    (*(int **)(param_1 + 0x5c0))[4] = 4;
  }
  if (FUN_0024de88(DAT_0034a77c, 0xc, index) != 0) {
    *(int *)(param_1 + 0x584) = 3;
  }
  else {
    if (FUN_0024de88(DAT_0034a77c, 7, index) != 0) {
      *(int *)(param_1 + 0x584) = 2;
    }
    else {
      if (FUN_0024de88(DAT_0034a77c, 2, index) != 0) {
        *(int *)(param_1 + 0x584) = 1;
      }
      else {
        *(int *)(param_1 + 0x584) = 0;
      }
    }
  }
  *(u8 *)(param_1 + 0x59c) = 1;
  {
    u16 *puVar7;
    uVar4 = (**(code **)(param_1 + 0x558))();
    uVar5 = (**(code **)(param_1 + 0x558))();
    uVar5 = (**(code **)(param_1 + 0x550))(uVar5);
    puVar7 = (u16 *)(**(code **)(param_1 + 0x3ec))(uVar4, uVar5);
    *(struct S4 *)(param_1 + 0x59e) = *(struct S4 *)puVar7;
    *(u8 *)(param_1 + 0x5a6) = *(u8 *)((char *)puVar7 + 8);
    *(u8 *)(param_1 + 0x5a7) = *(u8 *)((char *)puVar7 + 9);
    *(u8 *)(param_1 + 0x5a8) = *(u8 *)((char *)puVar7 + 10);
  }
  *(struct S4 *)(param_1 + 0x5aa) = *(struct S4 *)(param_1 + 0x59e);
  *(u8 *)(param_1 + 0x5b2) = *(u8 *)(param_1 + 0x5a6);
  *(u8 *)(param_1 + 0x5b3) = *(u8 *)(param_1 + 0x5a7);
  *(u8 *)(param_1 + 0x5b4) = *(u8 *)(param_1 + 0x5a8);
  iVar5 = FUN_0016bb88();
  FUN_00011090((void *)(param_1 + 0x63b), (void *)(iVar5 + 0x84c), 0x280);
  iVar5 = FUN_0016bb88();
  FUN_00011090((void *)(param_1 + 0x8bb), (void *)(iVar5 + 0xacc), 0x280);
  iVar5 = FUN_0016bb88();
  FUN_00011090((void *)(param_1 + 0xb3b), (void *)(iVar5 + 0xd4c), 0x280);
  uVar4 = (**(code **)(param_1 + 0x558))();
  uVar5 = (**(code **)(param_1 + 0x550))(uVar4);
  FUN_0017523c(param_1, (void *)(**(code **)(param_1 + 0x3ec))(uVar4, uVar5));
  FUN_0001ca58(&DAT_0030ebf4, &DAT_002e05e0);
  uVar4 = (**(code **)(param_1 + 0x558))();
  FUN_001b340c(&DAT_0034179c, uVar4);
  FUN_0001cb88(&DAT_0030ebf4);
  FUN_00171588(param_1, 0);
  FUN_0017c284(*(u32 *)(param_1 + 0x178));
}

// Retail structure (tools/hstdis.py, 0x00170068..0x00170344):
//   s1 = param_1; a0 = f558(); iVar5 = f3e8(a0); index = FUN_0024dd40(DAT_0034a77c, *(float*)(iVar5+4))
//   branch 9 -> 0x5bc = 4 with bounds {0,1,2,0x7fffffff,4}, else 0x5bc = 5 with {0,1,2,3,4};
//   each array store re-reads *(int**)(param_1+0x5c0) (store aliasing, same rule as 0x000bb190).
//   nested 0xc/7/2 lookups -> 0x584 = 3/2/1/0; 0x59c = 1;
//   4 shorts + 3 bytes copied out of f3ec(f558(), f550(f558())), then re-read from 0x59e into 0x5aa;
//   3 x memcpy(640) from FUN_0016bb88() + 0x84c/0xacc/0xd4c;
//   FUN_0017523c(param_1, f3ec(f558(), f550(f558()))) with only ONE f558 local kept;
//   FUN_0001ca58(&DAT_0030ebf4, &DAT_002e05e0); FUN_001b340c(&DAT_0034179c, f558());
//   FUN_0001cb88(&DAT_0030ebf4); FUN_00171588(param_1, 0); FUN_0017c284(*(u32*)(param_1+0x178)).
// Levers if the body misses: (1) the 0x5aa block must re-read from 0x59e (struct copy), never reuse the
// locals; (2) keep exactly one f558 temp in the last dispatch pair, the earlier block needs two;
// (3) if the frame or the register colouring is off, move the two `u16 *puVar7` block into its own scope
// exactly as written (MWCC colours block-scoped temps differently).
