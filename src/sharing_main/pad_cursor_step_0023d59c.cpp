/* HSTMATCH module=hstp_trans_main.prx start=0x0023d59c end=0x0023d77c symbol=FUN_0023d59c lang=c++ */
/* Hand-written reconstruction: clone of the source of pad_cursor_step_0022d49c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); readability pass; exact match verified; names and types await human review. */
/* Pad cursor step: reads four held-button tests (left, right, L and R trigger), builds a small range container from
   the pad-state array at +0x374, and turns whichever test kept its value (the container
   keeps only the entries still held when it holds fewer than two) into a +/-1 or +/-10
   step of the cursor at +0x254, or into FUN_0023fddc when nothing was pressed. The
   early exit reads the same analog byte twice instead of caching it, the "nothing
   pressed" test is written as an OR of the two zero cases, and the four direction
   flags are cleared with one chained assignment - all three are needed for the exact
   match. The 4th argument of FUN_000d8ff4 is an uninitialised empty tag local whose
   slot is re-read (lb a3,32(sp)). Header end is 0x0022d67c, not the ledger's 0x0022d65c. */


struct False_22d49c {};

/* Growable range container (data, size, capacity). */
struct Vec_22d49c {
    int *p;
    unsigned size;
    unsigned cap;
};

extern "C" {
extern char *DAT_003751d4;
extern char DAT_003eaad8[];
extern unsigned char FUN_0020c850(int *p, int a, int b);
extern void FUN_000d8ff4(struct Vec_22d49c *v, int first, int last, False_22d49c unk);
extern void FUN_000550a0(struct Vec_22d49c *v, char flag);
extern void FUN_0023fddc(char *self, int a);
extern void FUN_001bfc3c(char *p, int a);
extern int FUN_0016a3c0(int a, int b, int c);
}

enum {
    BUTTON_RIGHT = 0x20,
    BUTTON_LEFT = 0x80,
    BUTTON_L_TRIGGER = 0x100,
    BUTTON_R_TRIGGER = 0x200,
    BUTTON_TRIGGERS = 0x300,
    CURSOR_STEP_SMALL = 1,
    CURSOR_STEP_BIG = 10
};

/* Moves the cursor at +0x254 with left/right (+-1) or the triggers (+-10), then passes it through FUN_0016a3c0. */
extern "C" void FUN_0023d59c(char *self)
{
    int old;  /* cursor before the step */
    struct Vec_22d49c v;
    False_22d49c unk;
    unsigned char left, right, l_trigger, r_trigger;
    int cursor;

    old = *(int *)(self + 0x254);
    left = FUN_0020c850((int *)(self + 0x234), BUTTON_LEFT, BUTTON_TRIGGERS);
    right = FUN_0020c850((int *)(self + 0x238), BUTTON_RIGHT, BUTTON_TRIGGERS);
    l_trigger = FUN_0020c850((int *)(self + 0x23c), BUTTON_L_TRIGGER, 0);
    r_trigger = FUN_0020c850((int *)(self + 0x240), BUTTON_R_TRIGGER, 0);
    v.p = 0;
    v.size = 0;
    v.cap = 0;
    FUN_000d8ff4(&v, *(int *)(self + 0x374),
                 *(int *)(self + 0x374) + *(int *)(self + 0x378) * 4, unk);
    if (v.size < 2) {
        left = right = l_trigger = r_trigger = 0;
    }
    (void)0;
    if (left) {
        *(int *)(self + 0x254) = *(int *)(self + 0x254) - CURSOR_STEP_SMALL;
    } else if (right) {
        *(int *)(self + 0x254) = *(int *)(self + 0x254) + CURSOR_STEP_SMALL;
    } else if (l_trigger) {
        *(int *)(self + 0x254) = *(int *)(self + 0x254) - CURSOR_STEP_BIG;
    } else if (r_trigger) {
        *(int *)(self + 0x254) = *(int *)(self + 0x254) + CURSOR_STEP_BIG;
    } else {
        if (((*(char *)(DAT_003751d4 + 0x8b) << 28) >> 30) == 0 ||
            ((*(char *)(DAT_003751d4 + 0x8b) << 26) >> 30) == 0) {
            FUN_0023fddc(self, 0);
            if (((*(char *)(DAT_003751d4 + 0x8b) << 26) >> 30) != 0) {
            } else
                FUN_001bfc3c(DAT_003eaad8, 4);
        } else {
            goto done;
        }
    }
done:
    cursor = *(int *)(self + 0x254);
    if (old != cursor) {
        FUN_001bfc3c(DAT_003eaad8, 1);
        *(int *)(self + 0x254) =
            FUN_0016a3c0(*(int *)(self + 0x21c), *(int *)(self + 0x254), 0);
    }
    FUN_000550a0(&v, 0);
}
