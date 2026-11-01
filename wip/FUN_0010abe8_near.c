/* HSTMATCH module=EBOOT.BIN start=0x0010abe8 end=0x0010acbc symbol=FUN_0010abe8 lang=c99 */
struct Self {
    char pad0[28];
    int f1c;
};

extern char DAT_002d90ec;
extern char DAT_002d8f58;
extern char DAT_002d9130;
extern void FUN_0010acbc(struct Self *self);
extern int FUN_002b7ca8(void);
extern int FUN_002b7c28(void);
extern void FUN_0010a4a8(const void *fmt, ...);

void FUN_0010abe8(struct Self *self, int *out)
{
    FUN_0010acbc(self);
    int r = FUN_002b7ca8();
    if (r >= 0) {
        switch (r) {
        case 3: {
            int e = FUN_002b7c28();
            if (e < 0) {
                FUN_0010a4a8(&DAT_002d90ec, e);
            }
            break;
        }
        case 0:
            if (self->f1c < 0) {
                FUN_0010a4a8(&DAT_002d8f58, self->f1c);
                *out = -1;
            } else if (self->f1c == 0) {
                *out = 1;
            } else {
                FUN_0010a4a8(&DAT_002d9130, self->f1c);
                *out = -1;
            }
            break;
        }
    }
}

// 40 differing words out of 53 words (50/53 generated words, 3 words shorter due to branch layout).
// Twin/companion function to 0x0010a7cc.
// Differs at +0x24:
// Retail emits:
//   bgez v0, 0x10ac1c
//   nop
//   b 0x10acac
//   lw ra, 12(sp)
// While MWCC generates a single inverted branch-likely:
//   bltzl v0, 0x10aca0
//   lw ra, 12(sp)
// Same branch inversion pattern as 0x0010a7cc.
// improved by lane dsn5 (unreviewed): 44 -> 40 differing words
