/* HSTMATCH module=EBOOT.BIN start=0x0001c204 end=0x0001c560 symbol=FUN_0001c204 lang=c++ */
struct Entry {
    int a;
    int b;
};
struct Archive {
    char *data;
    int count;
    int pad8;
    int idx;
    unsigned char *cur;
    char pad14[0x10];
    unsigned char table[1];
};
extern "C" {
extern int DAT_002cfae4;
extern int DAT_0030ebd0;
extern char DAT_002cfc04[];
void FUN_0001b918(char *);
void FUN_002b78f0(void);
void FUN_002b7958(int);
void FUN_0001cf14(unsigned char *, char *, void (*)());
void FUN_0001ce28();
int FUN_0001100c(const void *, const char *, int);
}

#define WaitLoaded(n) \
    { int mode = DAT_002cfae4; if (mode == 3 || mode == 2) { \
        while (DAT_0030ebd0 < (n)) { \
            FUN_002b78f0(); \
            FUN_002b7958(100); \
        } \
    } else { \
        FUN_0001b918(DAT_002cfc04); \
        for (;;) { \
        } \
    } }

static inline int Key(const unsigned char *p)
{
    return (p[1] << 8) + p[0];
}

extern "C" int FUN_0001c204(Archive *a, const char *name)
{
    int i;
    unsigned char *p;
    if (a->idx < 0) {
        WaitLoaded(8)
        a->count = *(int *)(a->data + 4);
        a->idx = a->count - 1;
        int hdr = (a->count * 2 + 2) * 4;
        WaitLoaded(hdr + 8)
        Entry *t = (Entry *)(a->data + a->count * 8 + 8);
        int size = t->b;
        if (size == 0)
            size = t->a;
        hdr += size;
        WaitLoaded(hdr)
        FUN_0001cf14(a->table, a->data + a->count * 8 + 8, FUN_0001ce28);
    }
    if (a->idx < 0)
        return -1;
    if (++a->idx < a->count) {
        a->cur += a->cur[0] + 3;
    } else {
        a->idx = 0;
        a->cur = a->table;
    }
    const char *s = name;
    int len = 0;
    int h = 0;
    while (*s) {
        h = (((h << 1) & 0xfe) | ((h >> 7) & 1)) ^ *s;
        s++;
        len++;
    }
    int key = ((h & 0xff) << 8) + (unsigned char)len;
    if (key == Key(a->cur) && !FUN_0001100c(a->cur + 2, name, (unsigned char)len))
        return a->idx;
    p = a->table;
    for (i = 0; i < a->count; i++) {
        if (key == Key(p) && !FUN_0001100c(p + 2, name, (unsigned char)len)) {
            a->idx = i;
            a->cur = p;
            return i;
        }
        p += p[0] + 3;
    }
    return -1;
}
// NEAR (lane w4f1, 153 words differ by position; 212/215 words). Control flow matches.
// Remaining: (1) saved registers: retail i/hdr s0, p s1, name s2, a s3, key s4, len s5; ours numbers them in
//     definition order (name s0, a s1, hdr s2, len s3, ...). Not moved by: locals declared at the top in any
//     order, copying both params into locals, uchar len, a Hash() inline, a Load() inline.
// (2) retail reads the mode global into a1 in all three waits (ours v1/a0); a block-local `int mode` and a
//     switch (case 2: case 3:) did not help (switch is worse, 201).
// (3) size lookup: retail computes t = base + 8 before `lw t->b` and leaves `bnez; nop`.
// (4) `hdr += size; WaitLoaded(hdr)` (this file) is 3 words better than WaitLoaded(hdr + size).
// improved by lane w4f1 (unreviewed): 156 -> 153 differing words
