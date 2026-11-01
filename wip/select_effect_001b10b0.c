/* HSTMATCH module=EBOOT.BIN start=0x001b10b0 end=0x001b1384 symbol=FUN_001b10b0 lang=c99 */
/* Ghidra pseudocode (advisory) corrected by hand: the 0x17a test polarity was inverted, the
   table element offsets and the byte count are typed here; names and types await human review. */

typedef unsigned int uint;

typedef struct Entry {
    int           id;       /* 0x00 */
    unsigned char count;    /* 0x04 */
    char          pad[3];
    int           slots[1]; /* 0x08 */
} Entry;

typedef struct Obj {
    int  slots[0x1da];      /* 0x000 */
    char pad1[0x20];        /* 0x768 */
    int  f788;              /* 0x788 */
    int  f78c;              /* 0x78c */
    int  f790;              /* 0x790 */
    int  f794;              /* 0x794 */
    int  f798;              /* 0x798 */
    int  f79c;              /* 0x79c */
    int  f7a0;              /* 0x7a0 */
    int  f7a4;              /* 0x7a4 */
    int  f7a8;              /* 0x7a8 */
} Obj;

extern uint DAT_0034178c;
extern char DAT_002e7700[];

extern int  FUN_00062b64(int, int, int);
extern void FUN_000143b0(void *, int);
extern int  FUN_0022c988(int);

int FUN_001b10b0(Obj *obj, int idx)
{
    Entry *e;
    char tmp[32];
    int id;
    int r;
    int i;
    int a;
    int b;

    e = (Entry *)(DAT_002e7700 + idx * 0x1c);
    id = e->id;
    DAT_0034178c = DAT_0034178c * 0x343fd + 0x269ec3;
    r = ((DAT_0034178c >> 16) & 0x7fff) % e->count;

    if (id == 0x16a && obj->f788 != 0)
        return FUN_00062b64(obj->f788, e->slots[r], 0);
    if (id == 0x17a) {
        DAT_0034178c = DAT_0034178c * 0x343fd + 0x269ec3;
        a = (float)(unsigned)((DAT_0034178c >> 16) & 0x7fff) * 3.0517578e-05f < 0.5f
                ? obj->f78c : obj->f790;
        if (a != 0)
            return FUN_00062b64(a, e->slots[r], 0);
    }
    if (id == 0x1ca && obj->f794 != 0)
        return FUN_00062b64(obj->f794, e->slots[r], 0);
    if (id == 0x5c) {
        i = obj->f79c + 0x5c;
        b = 0;
        if (i < 0x1da)
            b = obj->slots[i] != 0;
        if (b)
            return FUN_00062b64(obj->slots[i], e->slots[r], 0);
    }
    if (id == 0xb4) {
        i = obj->f7a8 + 0xb4;
        b = 0;
        if (i < 0x1da)
            b = obj->slots[i] != 0;
        if (b) {
            FUN_000143b0(tmp, e->slots[r]);
            return FUN_00062b64(obj->slots[i], (int)tmp, 0);
        }
    }
    if (id == 0xae && FUN_0022c988(8) != 0)
        id = 0xaf;
    if (id < 0x1da && obj->slots[id] != 0)
        return FUN_00062b64(obj->slots[id], e->slots[r], 0);
    return -1;
}
// RESIDUAL 2026-11-01: 172 differing words, 181/181 words. MWCC hoists `e->slots[r]` to the
// function top in ours (one load into s1, then `move a1,s1` per call); retail reloads
// `sll v0,s0,2; addu v0,v0,s1; lw a1,8(v0)` at every call site and keeps r in s0, e in s1.
// Also ours turns the if-chain into plain `bne` where retail uses `bnel` with the next case
// constant in the delay slot, and retail keeps the remainder (mfhi) in s0 across the calls.
// RULED OUT: unprototyped callee (no CSE change), per-branch slot local (broke the body), pointer
// arg to the callee (compile error in that variant), ternary polarity (fixed separately).
// NEXT LEVER: stop the slot-load CSE - give FUN_00062b64 a pointer parameter so MWCC must assume
// it writes the table, or make each call site take the slot through a different expression.
// near miss from lane dsn8 (unreviewed)
