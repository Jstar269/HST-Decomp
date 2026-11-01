/* HSTMATCH module=EBOOT.BIN start=0x0025060c end=0x00250718 symbol=FUN_0025060c lang=c++ */
struct Tag {};
struct Item { int id; };

struct ScrollListObj {
    char pad00[0x2fc];
    int unk2fc;
    int unk300;
    int pad304;
    int count;
    char pad30c[0x350 - 0x30c];
    Item **items;
    int items_cnt;
    char pad358[4];
    int row;
    int pad360;
    int top;
};

extern "C" {
int FUN_00250cc4(ScrollListObj *self);
void FUN_00250cec(ScrollListObj *self, int idx);
}
inline int poll(ScrollListObj *self, Tag) { return FUN_00250cc4(self); }

extern "C" void FUN_0025060c(ScrollListObj *self)
{
    int target_id;
    if (poll(self, Tag()) != 0) {
        target_id = self->items[self->unk300]->id;
    } else {
        target_id = self->items[self->unk2fc]->id;
    }

    Item **p = self->items;
    int count = self->count;
    int items_cnt = self->items_cnt;
    Item **end = p + items_cnt;
    int i = 0;

    int m = (p == end) ^ 1;
    while (m) {
        if ((*p)->id == target_id) {
            if (count < 17) {
                self->row = i;
                self->top = 0;
            } else if (!(i + 12 < count)) {
                self->top = count - 16;
                self->row = i - (count - 16);
            } else if (i - 3 <= 0) {
                self->row = i;
                self->top = 0;
            } else {
                self->row = 3;
                self->top = i - 3;
            }
            FUN_00250cec(self, i);
            return;
        }
        p++;
        i++;
        m = (p == end) ^ 1;
    }
}
// Near miss: 66/67 words (55 differ, first at +0x18).
// Ruled out: C mode (frame 32 needs C++ Tag() temporary); ternary for poll; inlining helper without Tag.
// What differs: MWCC CSEs items (+0x350) between target_id setup and loop preheader into a3,
// while retail reloads items into a2 at 0x25065c (+0x50), leading to swapped a2/a3 registers in loop.
// improved by lane dsn4 (unreviewed): 56 -> 55 differing words
