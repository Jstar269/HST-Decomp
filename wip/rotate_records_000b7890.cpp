/* HSTMATCH module=EBOOT.BIN start=0x000b7890 end=0x000b793c symbol=FUN_000b7890 lang=c++ */
/* Hand-written reconstruction from the disassembly; STL std::rotate for forward iterator with 80-byte Record; 4 differing words out of 43; names and types await human review. */

struct Rec {
    char data[80];
} __attribute__((aligned(16)));

struct It {
    Rec *p;
    It(Rec *q = 0) : p(q) {}
};

extern "C" {
Rec *FUN_000b42ac(It *);
void FUN_000b41b8(Rec *, Rec *);
}

static inline void Swap(It a, It b)
{
    Rec *x = FUN_000b42ac(&a);
    Rec *y = FUN_000b42ac(&b);
    FUN_000b41b8(x, y);
}

extern "C" void FUN_000b7890(It first, It middle, It last)
{
    It i = middle;
    while (true) {
        Swap(*(It *)(volatile Rec **)&first.p, i);
        ++first.p;
        if (first.p == middle.p) {
            ++i.p;
            if (i.p == last.p) return;
            middle = i;
        } else {
            ++i.p;
            if (i.p == last.p) i = middle;
        }
    }
}
// Near miss: 4 words differ out of 43 (first at +0x20).
// Control flow, loop structure, branch-likely delay slot filling, register assignments (s0=middle, s1=last, s2=i, s3=first, s4=temp), and instruction schedule are 100% exact.
// Only difference is the stack offset of first: retail homes first at 36(sp) (0x24), ours at 40(sp) (0x28).
// Ruled out: raw pointer params (reduces frame to -48), tag parameter (frame -48), pass-by-reference Swap (spills i), user copy ctor (changes calling convention), volatile parameter (frame mismatch).
// near miss from lane dsn5 (unreviewed)
