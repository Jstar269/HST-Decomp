/* HSTMATCH module=EBOOT.BIN start=0x0017358c end=0x001739ac symbol=FUN_0017358c lang=c99 */
/* Hand-written from the pseudocode; two record-table compaction scans (mode 1 and mode 2 ids) plus a vtable-driven state table select. */

typedef int code();

typedef struct Rec {
  int id;        /* 0x00 */
  int own;       /* 0x04 */
  char a;        /* 0x08 */
  char b;        /* 0x09 */
  char c;        /* 0x0a */
  char d;        /* 0x0b */
  char e;        /* 0x0c */
  char pad[3];   /* 0x0d */
  int extra;     /* 0x10 */
} Rec;           /* size 0x14 */

extern void *FUN_00193474(int id);
extern int FUN_001bb8b8(int kind, int param);
extern int FUN_000d8c24(int table, int kind, int param);
extern int FUN_0024dd40(int table, float value);
extern int FUN_0024de88(int table, int kind, int index);
extern int DAT_00338b08;
extern int DAT_0034a77c;

void FUN_0017358c(int param_1, int param_2)
{
  Rec **base;
  Rec **dest;
  Rec **p;
  Rec **last;
  Rec *r;
  Rec *d;
  void *info;
  int iVar1;
  int iVar2;
  int iVar3;
  int n;
  int id;
  int a;
  int b;

  if ((unsigned)(param_2 - 0x7ffe) < 2) {
    *(int *)(param_1 + 0x1d4) = param_1 + 0x1b0;
  }
  else {
    info = FUN_00193474(param_2);
    iVar2 = FUN_001bb8b8(1, param_2);
    iVar3 = FUN_000d8c24(DAT_00338b08, 1, iVar2);
    base = *(Rec ***)(param_1 + 0x1d8);
    dest = *(Rec ***)(param_1 + 0x1e4);
    n = 0;
    p = base;
    last = p;
    if (p != p + *(int *)(param_1 + 0x1dc)) {
      do {
        if (p == last) {
          r = *p;
          n = n + 1;
          r->id = iVar2;
          r->own = -1;
          r->extra = iVar3;
          r->a = 0;
          r->b = 0;
          r->c = 0;
          r->d = 0;
          r->e = 0;
          r = *p;
          d = *dest;
          dest = dest + 1;
          d->id = r->id;
          d->own = r->own;
          d->a = r->a;
          d->b = r->b;
          d->c = r->c;
          d->d = r->d;
          d->e = r->e;
          d->extra = r->extra;
        }
        else {
          r = *p;
          if (iVar2 != r->id) {
            d = *dest;
            dest = dest + 1;
            d->id = r->id;
            n = n + 1;
            d->own = r->own;
            d->a = r->a;
            d->b = r->b;
            d->c = r->c;
            d->d = r->d;
            d->e = r->e;
            d->extra = r->extra;
          }
        }
        p = p + 1;
        last = *(Rec ***)(param_1 + 0x1d8);
      } while (p != *(Rec ***)(param_1 + 0x1d8) + *(int *)(param_1 + 0x1dc));
    }
    *(int *)(param_1 + 0x5dc) = n;

    iVar2 = FUN_001bb8b8(2, param_2);
    iVar3 = FUN_000d8c24(DAT_00338b08, 2, iVar2);
    base = *(Rec ***)(param_1 + 0x1f0);
    dest = *(Rec ***)(param_1 + 0x1fc);
    n = 0;
    p = base;
    last = p;
    if (p != p + *(int *)(param_1 + 0x1f4)) {
      do {
        if (p == last) {
          r = *p;
          n = n + 1;
          r->id = iVar2;
          r->own = -1;
          r->extra = iVar3;
          r->a = 0;
          r->b = 0;
          r->c = 0;
          r->d = 0;
          r->e = 0;
          r = *p;
          d = *dest;
          dest = dest + 1;
          d->id = r->id;
          d->own = r->own;
          d->a = r->a;
          d->b = r->b;
          d->c = r->c;
          d->d = r->d;
          d->e = r->e;
          d->extra = r->extra;
        }
        else {
          r = *p;
          a = *(char *)((char *)info + 0x24);
          b = ((int)*(signed char *)((char *)r->extra + 0x62) << 0x1c) >> 0x1c;
          if (a != 1 || b != 1) {
            if (a == 0 && b == 2) {
            }
            else {
              id = r->id;
              if (iVar2 != id) {
                d = *dest;
                dest = dest + 1;
                d->id = r->id;
                n = n + 1;
                d->own = r->own;
                d->a = r->a;
                d->b = r->b;
                d->c = r->c;
                d->d = r->d;
                d->e = r->e;
                d->extra = r->extra;
              }
            }
          }
        }
        p = p + 1;
        last = *(Rec ***)(param_1 + 0x1f0);
      } while (p != *(Rec ***)(param_1 + 0x1f0) + *(int *)(param_1 + 0x1f4));
    }
    *(int *)(param_1 + 0x5f0) = n;

    info = (void *)(**(code **)(param_1 + 0x3e8))(param_2);
    id = FUN_0024dd40(DAT_0034a77c, *(float *)((char *)info + 4));
    iVar1 = FUN_0024de88(DAT_0034a77c, 0x10, id);
    if (iVar1 == 0) {
      iVar1 = FUN_0024de88(DAT_0034a77c, 5, id);
      if (iVar1 == 0) {
        *(int *)(param_1 + 0x1d4) = param_1 + 0x1b0;
      }
      else {
        *(int *)(param_1 + 0x1d4) = param_1 + 0x1bc;
      }
    }
    else {
      *(int *)(param_1 + 0x1d4) = param_1 + 0x1c8;
    }
  }
}

/* PARKED 2026-11-01 (batch 1 spent): NEAR 211/264 words, 257 differ, first at +0x0.
 * RESIDUAL: frame 32 against retail's 48 (+0x0 differs: 27bdffe0 vs 27bdffd0), and the register
 * mapping is shifted by one - ours keeps param_1 in s2, retail in s3, and retail frees a slot for
 * 16 bytes of locals.
 * NEXT LEVER (in order):
 *  (1) retail holds 0x7fffffff in s1 at 0x1735d0 (lui 0x7fff / ori 0xffff) inside the scan prologue,
 *      so the free-slot cursor is very likely an INDEX initialised to 0x7fffffff (comparing indices),
 *      not the pointer `last = p` this draft used. Respell both scans with an int cursor.
 *  (2) retail keeps FUN_00193474's result in s0 across both scans, so `info` must be declared before
 *      the first scan and stay live (ours moved it into s3 and reused s3 for the vtable result).
 *  (3) the 16 extra frame bytes are locals for that cursor pair - expect them to appear only after (1).
 * The two read-time corrections held: the call is (*(int (**)(int))(param_1 + 0x3e8))(param_2) and
 * FUN_0024dd40 takes (DAT_0034a77c, *(float *)(info + 4)).
 */

/* PARKED 2026-11-01 (batch 1 spent): NEAR 211/264 words, 257 differ, first at +0x0.
 * RESIDUAL: frame 32 against retail's 48 (+0x0 differs: 27bdffe0 vs 27bdffd0), and the register
 * mapping is shifted by one - ours keeps param_1 in s2, retail in s3, and retail frees a slot for
 * 16 bytes of locals.
 * NEXT LEVER (in order):
 *  (1) retail holds 0x7fffffff in s1 at 0x1735d0 (lui 0x7fff / ori 0xffff) inside the scan prologue,
 *      so the free-slot cursor is very likely an INDEX initialised to 0x7fffffff (comparing indices),
 *      not the pointer `last = p` this draft used. Respell both scans with an int cursor.
 *  (2) retail keeps FUN_00193474's result in s0 across both scans, so `info` must be declared before
 *      the first scan and stay live (ours moved it into s3 and reused s3 for the vtable result).
 *  (3) the 16 extra frame bytes are locals for that cursor pair - expect them to appear only after (1).
 * The two read-time corrections held: the call is (*(int (**)(int))(param_1 + 0x3e8))(param_2) and
 * FUN_0024dd40 takes (DAT_0034a77c, *(float *)(info + 4)).
 */

/* BATCH 2 (2026-11-01): best = memberwise-copy, NEAR 223/264, 257 differ, first at +0x0.
 *   argarea-call (typedef int code(); unprototyped indirect call): 211/264, frame still 32 ->
 *     an unprototyped function pointer does NOT make MWCC reserve the 16-byte area. Hypothesis dead.
 *   memberwise-copy (id/own/5 chars/extra instead of *d = *r): +12 words, 223/264 - kept, promoted.
 *   rec0-live (FUN_00193474's result in its own local): identical to memberwise.
 * SAVED SETS: ours ra+s0-s3 (5) = retail's ra+s0-s3 (5). Retail still has frame 48 vs our 32, and
 *   retail never touches a stack slot other than those saves (checked every (sp) operand), and none of
 *   its callees is variadic (FUN_000d8c24, FUN_001bb8b8, FUN_00193474, FUN_0024de88 prologues checked).
 * CORRECTION TO THE OLD LEVER: the 0x7fffffff is DEAD. s1 is loaded from lui/ori at 0x1735d0 but the
 *   never-taken 0x1735e0 block (the one Ghidra deleted as unreachable) falls through to
 *   0x173604 `move s1,v0`, so the constant never survives a path. Do not chase it again.
 * NEXT LEVER: the 16 bytes belong to a source-level dead branch near 0x1735e0 - write the scan so the
 *   first iteration has an explicit "no free slot yet" branch with a 16-byte temporary, or look for a
 *   second, eliminated scan of the same arrays whose local array survived MWCC's DCE into the frame.
 */
// near miss from lane dsn8 (unreviewed)
