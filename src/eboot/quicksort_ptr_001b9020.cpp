/* HSTMATCH module=EBOOT.BIN start=0x001b9020 end=0x001b9364 symbol=FUN_001b9020 lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* steer=pragma:opt_dead_assignments (off, scoped to FUN_001b9020 by #pragma push/pop; found by the 7-decomp pragma sweep; no natural source matched) */
/* Hand-written reconstruction from the disassembly; EXACT with #pragma opt_dead_assignments off for this function. Randomised-pivot quicksort over an array of pointers to records
   whose first member is a short key (descending order): ranges below 21 elements go to the insertion sort 0x1b9470, the pivot is chosen with
   0x1b9364 (median of three) at first + n/4 and first + 3n/4, each shifted by a static counter cycling -4..4 (DAT_002e9a70 % 5, bumped after each
   use), the partition scans from both ends, an i == first case swaps the pivot to the front and skips equal keys, and the smaller side is
   recursed into while the larger one loops. Matching idioms: the element swaps go through `void **` (any word-typed access -- unsigned, int, long, void* -- works, float does not; a differently typed access stops MWCC from
   reusing the pointer loaded by the preceding compare; retail reloads *i but reuses *j); comparisons are `a->key > b->key` (a `<` form gives
   `slt v1`/`slt a0` instead of retail's `slt at`) except the loop-4 while, which needs `(*i)->key < (*first)->key` for the *first-before-*i load
   order; the j scan of phase 1 is `do { --j; } while (i != j && ...)` (an `if (i == j) break;` inside gives `beql j,i` instead of `beql i,j`);
   the counter bump is `>= 5` (`slti v0`). Remaining difference: in the i == first block retail loads last[-1] BEFORE *first
   (`lw a0,-4(s2); lw a1,0(s3); lh a0; lh a1`), ours *first first (`lw a1,0(s3); lw a0,-4(s2); lh a1; lh a0`). Ruled out: Item* temps in either
   order, key temps in either order, `>=`/`<=`/`!(<)`/`!(>)` spellings (the `<`/`>=` forms also lose the `slt at`), reading last[-1] through lm,
   j2 declared earlier or later. Names provisional. */
struct Item { short key; };
struct Less {};
#define GT(a, b) ((a)->key > (b)->key)
inline void SwapW(Item **a, Item **b)
{
    void **pa = (void **)a;
    void **pb = (void **)b;
    void *t = *pa;
    *pa = *pb;
    *pb = t;
}
extern int DAT_002e9a70;
extern "C" void FUN_001b9364(Item **a, Item **b, Item **c, Less comp);
extern "C" void FUN_001b9470(Item **first, Item **last, Less comp);

inline void Bump()
{
    DAT_002e9a70 = DAT_002e9a70 + 1;
    if (DAT_002e9a70 >= 5)
        DAT_002e9a70 = -4;
}

#pragma push
#pragma opt_dead_assignments off
extern "C" void FUN_001b9020(Item **first, Item **last, Less comp)
{
    for (;;) {
        int n = last - first;
        if (n <= 1)
            return;
        if (n < 21) {
            FUN_001b9470(first, last, comp);
            return;
        }
        int q1 = n / 4;
        Item **m1 = first + (q1 + DAT_002e9a70 % 5);
        Bump();
        int q2 = (n * 3) / 4;
        Item **m2 = first + (q2 + DAT_002e9a70 % 5);
        Bump();
        Item **lm = last - 1;
        FUN_001b9364(m1, m2, lm, comp);
        Item **i = first;
        Item **j = lm;
        while (GT(*i, *lm))
            ++i;
        do {
            --j;
        } while (i != j && !GT(*j, *lm));
        if (i < j) {
            SwapW(i, j);
            ++i;
            for (;;) {
                while (GT(*i, *lm))
                    ++i;
                do {
                    --j;
                } while (!GT(*j, *lm));
                if (!(i < j))
                    break;
                SwapW(i, j);
                ++i;
            }
        }
        if (i == first) {
            SwapW(i, lm);
            ++i;
            Item **j2 = last - 1;
            Item *pl = last[-1];
            if (!GT(*first, pl)) {
                while (i != last && !GT(*first, *i))
                    ++i;
                if (i < j2)
                    SwapW(i, j2);
            }
            if (i < j2) {
                for (;;) {
                    while (!((*i)->key < (*first)->key))
                        ++i;
                    do {
                        --j2;
                    } while (GT(*first, *j2));
                    if (!(i < j2))
                        break;
                    SwapW(i, j2);
                    ++i;
                }
            }
            first = i;
        } else {
            if (i - first < last - i) {
                FUN_001b9020(first, i, comp);
                first = i;
            } else {
                FUN_001b9020(i, last, comp);
                last = i;
            }
        }
    }
}
#pragma pop
// near miss: 4 words differ (0x1b9218..0x1b9224). Retail: lw a0,-4(s2); lw a1,0(s3); lh a0,0(a0); lh a1,0(a1); slt at,a0,a1. Ours: the two lw and the two lh swapped.
// ruled out: Item* temps pl/pf in either order, short key temps in either order, !GT/<=/>=/!(<)/!(>) spellings, last[-1] read via lm, j2 moved. The hot `slt at` forms need the GT macro.
