/* HSTMATCH module=EBOOT.BIN start=0x001c6354 end=0x001c6600 symbol=FUN_001c6354 lang=c99 */
typedef int (*FnI)(int *);
typedef void (*FnV)(int *);
typedef void (*Fn0)(void);
extern int FUN_00050534(int a);
extern void FUN_00051398(int a, int b);
extern void FUN_00051490(int a, int b, int c, int d, int e, int f, int g);
extern void FUN_00051894(float a, int b, float c, int d, float e, float f, float g, float h, int o, int col);
extern void FUN_0005185c(int a);
extern void FUN_000503d0(int a, int b);
extern void FUN_002917ac(void);

void FUN_001c6354(int param_1)
{
  char c;
  int b;
  int r;
  int n;
  int i;
  int j;
  int *p;
  int *q;
  float f;

  b = 0;
  if (*(char *)(*(int *)(param_1 + 0x15c) + 0x61) != 0) {
    b = *(char *)(*(int *)(param_1 + 0x15c) + 0x62) == 0;
  }
  if (b) {
    n = *(int *)(param_1 + 0x15c);
  } else {
    p = *(int **)(param_1 + 0x40);
    if (p != p + *(int *)(param_1 + 0x44)) {
      q = (int *)*p;
      do {
        if ((char)q[0x15] == 0) {
          n = *(int *)(param_1 + 0x44);
        } else {
          r = ((FnI)(*(int *)(*q + 0x34)))(q);
          b = r == 2;
          if (!b) {
            r = ((FnI)(*(int *)(*q + 0x34)))(q);
            b = r == 3;
          }
          if (b) {
            c = *(char *)(param_1 + 0x18c);
          lab430:
            if (c != 0) {
              r = *q;
              goto lab43c;
            }
          } else {
            r = ((FnI)(*(int *)(*q + 0x34)))(q);
            if (r == 6) {
              c = *(char *)(param_1 + 0x18c);
              goto lab430;
            }
            r = *q;
          lab43c:
            ((FnV)(*(int *)(r + 0x14)))(q);
          }
          n = *(int *)(param_1 + 0x44);
        }
        p = p + 1;
        if (p == (int *)(*(int *)(param_1 + 0x40) + n * 4)) break;
        q = (int *)*p;
      } while (1);
    }
    n = *(int *)(param_1 + 0x15c);
  }
  if (*(char *)(n + 0x61) == 0) {
    r = *(int *)(n + 4);
  } else {
    if (*(char *)(n + 0x61) != 0) {
      if (*(char *)(n + 0x62) == 0) {
        i = FUN_00050534(0x311170);
        if (i != 0) {
          FUN_00051398(0, i);
          f = 0.0f;
          FUN_00051490(i, 0, 0, 0x1e0, 0x110, 0, 0);
          j = 0;
          do {
            FUN_00051894(f, 0, f, 0, f + 120.0f, 240.0f, f + 120.0f, 240.0f, i, 0x80808080);
            j = j + 1;
            f = f + 120.0f;
          } while (j < 4);
          FUN_0005185c(i);
        }
      } else {
        FUN_000503d0(0x4579fe66, 0x311170);
        *(unsigned char *)(n + 0x62) = 0;
      }
    }
    r = *(int *)(n + 4);
  }
  if (r == 0xc) {
    if (*(int *)0x34a81c != 0 && *(int *)(n + 0xfc) <= *(int *)(n + 8)) FUN_002917ac();
  } else if (r == 2 && *(char *)(n + 99) != 0) {
    ((Fn0)(*(int *)(*(int *)(n + 0x20) + 0x14)))();
  }
}

/* NEAR 161/171 words, 169 differ, first +0x0. Prologue: ours 32-byte frame saving ra,s2,s1,s0,f20 vs
   retail 80-byte frame saving ra,s3,s2,s1,s0,f20 -- s3 is live in retail's dispatch loop and the local
   area is 40 bytes larger (retail has spills the draft never makes); body is 10 words short, so this is
   a structure/allocation gap, not a near miss. Retail also reloads `a0 = *(p+0x15c)` at +0x374 (after
   `move s2,a0`) instead of caching it in s0, and computes the entry flag as a boolean (`sltu`/`xori`)
   then `bnezl v1,0x1c6474` with `lw s0,348(s2)` in the delay slot -- i.e. the pointer is fetched once
   per use, not stored once.
   Decoded shape: entry flag = (*(p+0x15c)->b61 != 0) && (*(p+0x15c)->b62 == 0); if flag n=*(p+0x15c)
   else walk p->list[0x40..0x44], for each entry with entry->b15 != 0 call vt[0x34] twice (2 then 3),
   if 2/3 or the third call returns 6 and p->b18c != 0 invoke vt[0x14]; clamp n to list count.
   Then if (*(n+0x61)) { if (*(n+0x62)==0) build the 4x120px sprite sheet via FUN_00050534/
   FUN_00051398/FUN_00051490/FUN_00051894 x4/FUN_0005185c else FUN_000503d0(0x4579fe66,...) and
   *(n+0x62)=0 } ; r = *(n+4); if (r==0xc) { if (*(int*)0x34a81c && *(n+0xfc) <= *(n+8)) FUN_002917ac();
   } else if (r==2 && n->b63) call vt[0x14] on *(n+0x20).
   NEXT: the 10 missing words are in the dispatch loop's three vt[0x34] calls and the s3 live range --
   re-read retail 0x1c639c..0x1c6474 and write each vt call's result into its own variable used twice
   (that is what keeps s3 live), then let the entry clause deref *(p+0x15c) at each use. */
// near miss from lane dsn8 (unreviewed)
