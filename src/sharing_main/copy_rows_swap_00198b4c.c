/* HSTMATCH module=hstp_trans_main.prx start=0x00198b4c end=0x00198c8c symbol=FUN_00198b4c lang=c99 */
/* Hand-written reconstruction: clone of the source of copy_rows_swap_0018c39c.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane): the two copies are guarded by
   `if (bufB != 0) { if (bufA != 0) {...} } else { <table check> }` so the bufB==0 block is laid
   out last; each row walk is `if (0 < height) { do { width = stride; .. } while (row >= 0 && row < height); }`
   with the stride load at the TOP of the body, which is what puts the reload in the bnezl delay slot.
   Exact match verified; names and types await human review. */
extern int DAT_003ea8ac[];
extern int DAT_003ea870[];
extern int FUN_00051df0(int);
extern void FUN_00011090(int, int, int);

typedef struct RowSurface {
    char pad0[0x48];
    int width;                          /* 0x48 pixels per row (2 bytes each) */
    int height;                         /* 0x4c rows */
    char pad50[0x7c - 0x50];
    int (*getBufA)(int index, int variant); /* 0x7c */
    int (*getBufB)(int index, int variant); /* 0x80 */
} RowSurface;

/* Copies the rows into the target's pixels from buffer A; with no buffer B they come from the table entry for `index`. */
void FUN_00198b4c(RowSurface *self, int target, int index, int variant)
{
    int bufA;
    int bufB;
    int offset;
    int width;
    int row;

    bufA = self->getBufA(index, variant);
    bufB = self->getBufB(index, variant);
    if (bufB != 0) {
        if (bufA != 0) {
            bufB = FUN_00051df0(target);
            row = 0;
            if (0 < self->height) {
                do {
                    width = self->width;
                    offset = row * width * 2;
                    FUN_00011090(bufB + offset, bufA + offset, width << 1);
                    row = row + 1;
                } while (row < self->height);
            }
        }
    } else {
        if (DAT_003ea8ac[index] != 0) {
            bufB = DAT_003ea870[index];
            bufA = FUN_00051df0(target);
            row = 0;
            if (0 < self->height) {
                do {
                    width = self->width;
                    offset = row * width * 2;
                    FUN_00011090(bufA + offset, bufB + offset, width << 1);
                    row = row + 1;
                } while (row < self->height);
            }
        }
    }
}
