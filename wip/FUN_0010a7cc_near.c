/* HSTMATCH module=EBOOT.BIN start=0x0010a7cc end=0x0010a8a0 symbol=FUN_0010a7cc lang=c99 */
struct Self {
    char pad0[40];
    int f28;
};

extern char DAT_002d8f1c;
extern char DAT_002d8f58;
extern char DAT_002d8f74;
extern void FUN_0010a8a0(struct Self *self);
extern int FUN_002b7c58(void);
extern int FUN_002b7ce0(void);
extern int FUN_0010a4a8(const void *fmt, ...);

void FUN_0010a7cc(struct Self *self, int *out)
{
    FUN_0010a8a0(self);
    int r = FUN_002b7c58();
    if (r >= 0) {
        switch (r) {
        case 3: {
            int e = FUN_002b7ce0();
            if (e < 0) {
                FUN_0010a4a8(&DAT_002d8f1c, e);
            }
            break;
        }
        case 0:
            if (self->f28 < 0) {
                FUN_0010a4a8(&DAT_002d8f58, self->f28);
                *out = -1;
            } else if (self->f28 == 0) {
                *out = 1;
            } else {
                FUN_0010a4a8(&DAT_002d8f74, self->f28);
                *out = -1;
            }
            break;
        }
    }
}

// 40 differing words out of 53 words (50/53 generated words, 3 words shorter due to branch layout).
// Differs at +0x24:
// Retail emits:
//   bgez v0, 0x10a800
//   nop
//   b 0x10a890
//   lw ra, 12(sp)
// While MWCC generates a single inverted branch-likely:
//   bltzl v0, 0x10a884
//   lw ra, 12(sp)
// Ruled out:
// - if (r < 0) return; / goto end; (MWCC still inverts to bltzl)
// - do { if (r < 0) break; } while(0); (inverts to bltzl)
// - if ((r = ...) >= 0) / if (r > -1) (same bltzl)
// - lang=c++ vs lang=c99 (c++ also inverts to bltzl and re-orders switch blocks)
// improved by lane dsn5 (unreviewed): 42 -> 40 differing words
