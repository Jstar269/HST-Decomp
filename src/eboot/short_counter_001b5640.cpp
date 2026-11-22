/* HSTMATCH module=EBOOT.BIN start=0x001b5640 end=0x001b5740 symbol=FUN_001b5640 lang=c++ */
/* Hand-written reconstruction from the disassembly (started from the lane-fm6 wip draft); exact match verified; names and types await human review. Matching idiom: trailing unused empty Tag parameter (reserves the 16 extra frame bytes), plain C++ otherwise. */
struct Counter {
    short cur;
    short max;
    short zero;
};
struct Tag {};

extern "C" {
extern char *DAT_003386b0;
extern char DAT_0034179c[];
extern void FUN_001b2bb8(void *, int);
}

extern "C" void FUN_001b5640(char *self, int idx, Tag)
{
    Counter *c;
    int i;

    if (*(int *)(DAT_003386b0 + 0x3c) == 7) {
        if (idx == *(int *)(self + 0xec))
            FUN_001b2bb8(DAT_0034179c, *(int *)(self + 0xec));
        return;
    }
    c = ((Counter **)(self - 600))[idx];
    c->cur = c->cur + 1;
    c->zero = 0;
    if (idx == 0xc9 && c->cur >= c->max) {
        *(int *)(self + 0xec) = 0xc9;
        FUN_001b2bb8(DAT_0034179c, 0xc9);
        c->max = c->max + 1;
        i = 0;
        do {
            i += 6;
            **(short **)(self + 0xa0) = 0;
            **(short **)(self + 0xa4) = 0;
            **(short **)(self + 0xa8) = 0;
            **(short **)(self + 0xac) = 0;
            **(short **)(self + 0xb0) = 0;
            **(short **)(self + 0xb4) = 0;
            self += 0x18;
        } while (i < 18);
    }
}
