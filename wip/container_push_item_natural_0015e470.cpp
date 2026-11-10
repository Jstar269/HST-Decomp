/* HSTMATCH module=EBOOT.BIN start=0x0015e470 end=0x0015e558 symbol=FUN_0015e470 lang=c++ */
struct Item {
    virtual ~Item();
    virtual void dummy1();
    virtual void dummy2();
    virtual int check();
    void *parent;   /* +4 */
    char pad_8[8];  /* +8 */
    char flag;      /* +0x10 */
};

struct EmptyTag {};

extern "C" void FUN_00160298(void *vec, Item **item, EmptyTag tag);

struct Iter {
    Item **ptr;
    Iter &operator--() { --ptr; return *this; }
};

struct CIter {
    Item *const *ptr;
    CIter(const Iter &i) : ptr(i.ptr) {}
};

struct Container {
    char pad[20];
    Item **data;   /* +20 */
    unsigned size; /* +24 */
    unsigned cap;  /* +28 */

    Iter end() { Iter it; it.ptr = data + size; return it; }
    void push_back(Item *const &x, EmptyTag)
    {
        if (size < cap) {
            size++;
            unsigned index = size - 1;
            data[index] = x;
        } else {
            FUN_00160298(&data, (Item **)&x, EmptyTag());
        }
    }
};

extern "C" CIter FUN_0015e470(Container *self, Item *item)
{
    if (item != 0) {
        item->parent = self;
        if (item->check())
            item->flag = 0;
        self->push_back(item, EmptyTag());
        Iter it = self->end(); --it; return it;
    }
    return self->end();
}
// NEAR 7 (lane opus2): natural vector form; every instruction matches retail except stack slot offsets.
// Needed: Iter WITHOUT user ctors (end() temp copied lwc1/swc1), a CIter return type with a converting ctor
// (direct `sw v1,0(s1)` for both returns), and an unused EmptyTag on the inline push_back (48-byte frame).
// Left: item param homed at 28(sp) (retail 20), `it` at 44 (retail 28): retail has two more slots below.
// Tried: trailing Tag on the function, Tag on end(), operator-/operator-=/--end() forms, named CIter.
// near miss from lane opus2 (unreviewed)
