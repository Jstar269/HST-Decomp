/* HSTMATCH module=EBOOT.BIN start=0x002ad758 end=0x002ad8cc symbol=FUN_002ad758 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_0024e064();
extern unsigned char FUN_002acd60(char *base, int idx, int id);

extern int DAT_002cc394[40];
extern unsigned char DAT_002cc2cc[40];
extern unsigned int DAT_002cc2f4[40];
extern int DAT_0034b590;
extern int DAT_0034b4f0[40];

void FUN_002ad758(int mask, char *base, int idx)
{
    int i;
    int id;
    unsigned char *e;
    unsigned char k;
    int n;
    int en;
    int r;

    if (idx != 0)
        r = idx;
    else
        r = FUN_0024e064();
    DAT_0034b590 = 0;
    for (i = 0; i < 40; i++) {
        id = DAT_002cc394[i];
        if (i >= 0 && i < 40)
            en = (DAT_002cc2cc[i] != 0);
        else
            en = 0;
        if (!en)
            continue;
        if ((mask & DAT_002cc2f4[id]) == 0)
            continue;
        if (id >= 0 && id < 40)
            e = (unsigned char *)base + id * 20 + 0x1058;
        else
            e = 0;
        if ((unsigned)(*e - 4) < 2)
            continue;
        if (FUN_002acd60(base, r, id) == 0)
            continue;
        n = DAT_0034b590;
        if (n < 39) {
            DAT_0034b4f0[n] = id;
            DAT_0034b590 = n + 1;
        }
    }
}
// NEAR 93/93, 7 words: retail uses the a2 param in the bnez and puts 'move s4,a2' in its delay slot; ours copies s4 first (same for all 4 spellings tried: if/ternary/if(!)/early-assign). Second diff 0x2ad834 sltiu at vs v1 is allocator choice.
// near miss from lane dsn3 (unreviewed)
