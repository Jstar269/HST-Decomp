/* HSTMATCH module=EBOOT.BIN start=0x000a84c0 end=0x000a8608 symbol=FUN_000a84c0 lang=c++ */
/* Hand-written reconstruction: clone of the source of clamped_y_band_check_000aaeb4.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Follows the sibling src/sharing_main/clamp_vec4_x_000aaba8.cpp (same x clamp); FUN_00022434 takes the two vectors Ghidra dropped (a1/a2 passed through); y is read into a local before the two context calls, so it stays in $f20. */
struct Vec4 {
    float x, y, z, w;
};

struct Ctx {
    char pad00[0x24];
    float f24;
    char pad28[0x14];
    float f3c;
};

struct Obj {
    char pad00[0x150];
    int f150;
};

extern "C" {
extern const float DAT_002d46a4;
extern const float DAT_002d46a8;
extern Vec4 *FUN_00022434(Vec4 *dst, const Vec4 *a, const Vec4 *b);
extern Ctx *FUN_000b3c68(void *obj);
}

template <typename T>
static inline const T &max(const T &a, const T &b) {
    return (a < b) ? b : a;
}

template <typename T>
static inline const T &min(const T &a, const T &b) {
    return (b < a) ? b : a;
}

/* Returns 1 when the y component of (a - b), with x clamped as in FUN_000aaba8, lies in
   (-(0.01 * f3c), -(0.01 * f24)] of the context returned by FUN_000b3c68; otherwise 0. */
extern "C" int FUN_000a84c0(Obj *self, const Vec4 *a, const Vec4 *b, unsigned char flag)
{
    Vec4 d;
    Vec4 diff;

    FUN_00022434(&diff, a, b);
    d = diff;
    if (flag == (self->f150 == 0)) {
        d.x = max(DAT_002d46a4, d.x);
    } else {
        d.x = min(DAT_002d46a8, d.x);
    }
    float y = d.y;
    if (y > -(FUN_000b3c68(self)->f3c * 0.01f) &&
        y <= -(FUN_000b3c68(self)->f24 * 0.01f)) {
        return 1;
    }
    return 0;
}
