/* HSTMATCH module=hstp_trans_main.prx start=0x001778e4 end=0x00177994 symbol=FUN_001778e4 lang=c++ */
/* Clone of src/eboot/set_clamped_val_0016b720.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Looks up an entry base via FUN_00177d4c, clamps value between DAT_00387a34 and DAT_00387a38 using inline Clamp, and stores to entry offset 6 if 0 <= idx < 15. */

extern int DAT_00387a34;
extern int DAT_00387a38;

extern "C" void *FUN_00177d4c(int a, int b);

inline int Clamp(const int &v, const int &lo, const int &hi) {
    return v < lo ? lo : (hi < v ? hi : v);
}

extern "C" void FUN_001778e4(int idx, int value) {
    void *base = FUN_00177d4c(idx, value);
    int v = (unsigned short)value;
    int c = (unsigned short)Clamp(v, DAT_00387a34, DAT_00387a38);

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
