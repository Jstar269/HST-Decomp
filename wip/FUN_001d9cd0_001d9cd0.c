/* HSTMATCH module=EBOOT.BIN start=0x001d9cd0 end=0x001d9cfc symbol=FUN_001d9cd0 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Inner {
    char pad[0x54];
    int val;
};

struct Obj {
    char pad[0x3c];
    struct Inner *inner;
};

int FUN_001d9cd0(struct Obj *self)
{
    int val = self->inner->val;
    int res = 1;
    if (val != 1 && val != 2) {
        res = 0;
    }
    return res;
}
// Near miss: 8/11 words match (3 differ).
// Retail emits:
//   li a1, 1
//   lw a0, 84(v1)
//   beq a0, a1, exit
//   li v0, 1 (delay slot)
//   li v1, 2
//   beq a0, v1, exit
//   nop
//   move v0, zero
//   exit: jr ra; nop
// Whereas MWCC hoists li v0, 1 to function entry and uses v0 for the first equality check.
// Tried: nested if, goto ladder, ternaries, separate locals, unsigned types, c++.
// near miss from lane dsn7 (unreviewed)
