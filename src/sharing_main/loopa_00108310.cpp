/* HSTMATCH module=hstp_trans_main.prx start=0x00108310 end=0x0010838c symbol=FUN_00108310 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: pointer-wrapper iterator with operator!= as !(a == b) and const begin()/end(), so the entry test is not folded to size*4 == 0. */
extern "C" void FUN_00105358(char *);
struct Item { char pad[84]; int f84; };
struct Iter;
struct Vec {
    int pad; Item **items; int count;
    Iter begin() const;
    Iter end() const;
};
struct Iter {
    Item **p;
    Iter() {}
    Iter(Item **q) : p(q) {}
    Item *operator*() const { return *p; }
    Iter &operator++() { ++p; return *this; }
};
inline Iter Vec::begin() const { return Iter(items); }
inline Iter Vec::end() const { return Iter(items + count); }
static inline int operator!=(const Iter &a, const Iter &b) { return (a.p == b.p) ^ 1; }
extern "C" void FUN_00108310(Vec *self)
{
    Iter it = self->begin();
    Iter end = self->end();
    if (it != end) {
        do {
            FUN_00105358((char *)*it + 89);
            ++it;
            end = self->end();
        } while (it != end);
    }
}
