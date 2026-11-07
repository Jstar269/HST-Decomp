/* HSTMATCH module=hstp_trans_main.prx start=0x001b1b54 end=0x001b1d2c symbol=FUN_001b1b54 lang=c++ */
/* Hand-written reconstruction: clone of the source of game_slot_select_001a4588.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
// matching idioms: empty `False` by-value tag parameter (48-byte frame); pointer-wrapper `Iter` with `!(cur == o.cur)` operator!=; the slot count is read twice (inline End() and a local `n` before the loop, int vs. inline getter) so MWCC keeps two loads; `int c = **it; if (c == param_2)` for the bnel operand order; the range test spelled `slot_id <= 0 || slot_id >= 0x15` (gives `slti v1`, the `0x14 < slot_id` spelling gives `slti at`).
#include "hst/slot_select.h"

struct False {};

static inline int **Begin(SlotSelect *s) { return s->slots; }
static inline int **End(SlotSelect *s) { return s->slots + s->slotCount; }
static inline int Count(const SlotSelect *s) { return s->slotCount; }

struct Iter {
    int **cur;
    Iter(int **p) { cur = p; }
    bool operator!=(const Iter &o) const { return !(cur == o.cur); }
    Iter &operator++() { cur++; return *this; }
    int *operator*() const { return *cur; }
};
extern "C" int FUN_00290228(int);
extern "C" int FUN_00290230(int, int);
extern "C" int DAT_003ba2c8;
extern "C" int DAT_003f3b1c;

enum {
    ENTRY_KIND_OFFSET = 0x12, /* byte inside a table entry */
    ENTRY_KIND_PICKED = 2,    /* entry kind chosen when the automatic pick looks one up */
    LIST_ROWS = SLOT_SELECT_VISIBLE_ROWS,
    SCROLL_MARGIN = 3         /* rows kept above the selection when scrolled */
};

/* Put the cursor on the slot whose id is slot_id (0 = automatic pick, bad ids fall back to 1). */
extern "C" void FUN_001b1b54(SlotSelect *self, int slot_id, False)
{
    int count;
    int index;
    int found;
    int i;
    int t;
    int **base;
    int **end;
    int **p;

    if (slot_id <= 0 || slot_id >= SLOT_SELECT_ID_LIMIT) {
        if (slot_id == SLOT_SELECT_AUTO) {
            if (self->autoPick != 0) {
                found = 0;
                for (i = 0; i < FUN_00290228(DAT_003f3b1c); i++) {
                    int entry = FUN_00290230(DAT_003f3b1c, i);
                    if (entry != 0
                        && *(signed char *)(entry + self->unk54 + ENTRY_KIND_OFFSET) == ENTRY_KIND_PICKED) {
                        found = 1;
                        slot_id = i;
                        break;
                    }
                }
                if (!found)
                    slot_id = SLOT_SELECT_DEFAULT;
            } else {
                /* LCG step, then a random slot's id */
                DAT_003ba2c8 = DAT_003ba2c8 * 0x343fd + 0x269ec3;
                slot_id = *self->slots[((DAT_003ba2c8 >> 0x10) & 0x7fff) % (unsigned int)self->slotCount];
            }
        } else {
            slot_id = SLOT_SELECT_DEFAULT;
        }
    }
    int n = Count(self);
    index = 0;
    for (Iter it(Begin(self)), last(End(self)); it != last; ++it, index++) {
        int c = **it;
        if (c == slot_id) {
            if (n <= LIST_ROWS) {
                self->cursor.row = index;
                self->cursor.top = 0;
            } else if (index + 2 >= n) {
                /* near the end: window is pinned to the last rows */
                t = n - LIST_ROWS;
                self->cursor.top = t;
                self->cursor.row = index - t;
            } else if (index - SCROLL_MARGIN < 1) {
                self->cursor.row = index;
                self->cursor.top = 0;
            } else {
                self->cursor.row = SCROLL_MARGIN;
                self->cursor.top = index - SCROLL_MARGIN;
            }
            self->cursor.index = index;
            return;
        }
    }
}
