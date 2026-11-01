/* HSTMATCH module=EBOOT.BIN start=0x0016b720 end=0x0016b7d0 symbol=FUN_0016b720 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Looks up an entry base via FUN_0016bb88, clamps value between DAT_002e0478 and DAT_002e047c using inline Clamp, and stores to entry offset 6 if 0 <= idx < 15. */

extern int DAT_002e0478;
extern int DAT_002e047c;

extern "C" void *FUN_0016bb88(int a, int b);

inline int Clamp(const int &v, const int &lo, const int &hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

extern "C" void FUN_0016b720(int idx, int value) {
    void *base = FUN_0016bb88(idx, value);
    int v = (unsigned short)value;
    int c = (unsigned short)Clamp(v, DAT_002e0478, DAT_002e047c);

    char *p;
    if (idx < 0)
        goto zero;
    if (idx < 15) {
        p = (char *)base + idx * 56 + 4;
        goto store;
    }
zero:
    p = 0;
store:
    *(short *)(p + 2) = (short)c;
}
