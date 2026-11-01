/* HSTMATCH module=EBOOT.BIN start=0x000026d8 end=0x00002774 symbol=FUN_000026d8 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   Scans the 16-entry id table at DAT_0030a040, skipping empty entries and entries whose id range does not
   cover the requested id, and copies the matched entry's two pointers into out[3]/out[4]. An entry is a
   pair of pointers: the first points at the entry's first id, the second at a {base, size} range whose low
   bit is a flag (masked with ~1 before it is added to the base). Returns 1 on a hit, 0 after all 16 entries.
   Matching idioms: the range base test is written with the swapped operands (*first > id), which makes MWCC
   compare into $at instead of reusing the dead loaded register; the table is indexed by i inside the loop
   (the strength-reduced pointer is only used for the checks, so the success block recomputes base + i*8). */
typedef struct RangeInfo {
    unsigned int base;   /* +0x0 */
    unsigned int size;   /* +0x4, low bit is a flag */
} RangeInfo;

typedef struct RangeEntry {
    unsigned int *first; /* +0x0: points at the entry's first id */
    RangeInfo *range;    /* +0x4 */
} RangeEntry;

extern RangeEntry DAT_0030a040[16];

int FUN_000026d8(int *out, unsigned int id)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (DAT_0030a040[i].first == 0) {
            continue;
        }
        if (*DAT_0030a040[i].first > id) {
            continue;
        }
        if (id >= DAT_0030a040[i].range->base + (DAT_0030a040[i].range->size & ~1u)) {
            continue;
        }
        out[3] = (int)DAT_0030a040[i].first;
        out[4] = (int)DAT_0030a040[i].range;
        return 1;
    }
    return 0;
}
