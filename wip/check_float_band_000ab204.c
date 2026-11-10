/* HSTMATCH module=hstp_trans_main.prx start=0x000ab204 end=0x000ab274 symbol=FUN_000ab204 lang=c++ */
struct Ctx {
    char pad[0x24];
    float f24;
};
struct Obj {
    char pad[4];
    float f4;
};

extern "C" Ctx *FUN_000b6e80(void *);

extern "C" int FUN_000ab204(void *param_1, Obj *param_2)
{
    Ctx *ctx = FUN_000b6e80(param_1);
    if (param_2->f4 > -(ctx->f24 * 0.01f)) {
        if (param_2->f4 < 0.0f) {
            return 1;
        }
    }
    return 0;
}

// NEAR MISS: 28/28 words, 5 differ (FPR numbering only: $f1 vs $f2). Reduced from 18 words (lane w3s1).
// Structure, branches, constants and delay slots match retail 100% (bc1tl with delay slot move v0, zero,
// and the unfilled-slot tail bc1f; nop; b; li v0, 1; move v0, zero).
// Retail loads param_2->f4 into $f1 and the 0.01f constant into $f2 (mtc1 v0, $f2; mul.s $f0, $f2, $f0;
// c.ngt.s $f1, $f0; c.lt.s $f1, $f0); ours loads param_2->f4 into $f2 and 0.01f into $f1.
// Same FPR swap is present in twins 0x000ab60c and EBOOT 0x000a8810.
// Ruled out: named local p, non-const scale local, inline check_band helper (both argument orders),
// trivial getters on Ctx and Obj, mirrored compares, GCC SDK profiles, calling context getter inside condition.
// improved by lane w3s1 (unreviewed): 18 -> 5 differing words
