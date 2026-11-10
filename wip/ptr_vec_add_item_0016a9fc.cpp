/* HSTMATCH module=hstp_trans_main.prx start=0x0016a9fc end=0x0016aae4 symbol=FUN_0016a9fc lang=c++ */
/* Hand-written reconstruction from the disassembly (near miss, 1 word); names and types await human review. */
struct Mgr;
class Item {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual int v3();
    Mgr *owner;
    char pad[8];
    char flag;
};

struct Tag {};
template <class T> struct It {
    T *p_;
    It(T *p) : p_(p) {}
};
template <class T> inline void Adv(It<T> &i, int n, Tag) { i.p_ += n; }
template <class T> inline It<T> operator-(It<T> a, int n) { Adv(a, -n, Tag()); return a; }

struct Alloc {};
struct PVec;
extern "C" void FUN_0016c824(PVec *v, Item *const *x, Alloc a);
struct PVec {
    Item **data;
    unsigned size;
    unsigned capacity;
    It<Item *> end() { return It<Item *>(data + size); }
    void push_back(Item *const &x, Tag, Tag)
    {
        if (size < capacity) {
            ++size;
            unsigned i = size - 1;
            data[i] = x;
        } else {
            FUN_0016c824(this, &x, Alloc());
        }
    }
};
struct Mgr {
    char pad[0x14];
    PVec items;
};

extern "C" It<Item *> FUN_0016a9fc(Mgr *m, Item *item)
{
    if (item != 0) {
        item->owner = m;
        if (item->v3() != 0) item->flag = 0;
        m->items.push_back(item, Tag(), Tag());
        return operator-(m->items.end(), 1);
    }
    return m->items.end();
}
// Near miss, 3 words differ: code identical, only two iterator-temporary stack slots: ours by-value copy `a` (read back and also the dead result store) at 24(sp);
// retail copy at 28(sp) (read back) and a SEPARATE dead result store to 44(sp). Item* param at 20(sp), end() temp at 36(sp) and the 48-byte frame match.
// Found: unused empty by-value `Tag` params (push_back x2, operator- x1; 011/110/201/300 distributions give 5 words) place the temps; `unsigned i = size - 1; data[i] = x;` (not data[size - 1]) gives retail's addiu -1 / sll order.
// Ruled out for the separate 44(sp) result object: operator- bodies returning a named local / copy-initialised local / `It(a.p_-n)`, `It r = ...; return r;`, explicit `It(...)` cast at the return, member operator-/-=/+= chains (scalarize the temps), const-ref param; tag counts 0..3 on end/push_back/operator-.
// lane pn3 (2026-11-10, ~45 compiles): 3 -> 1 word. The decrement goes through an inline advance helper that takes
// the unused empty Tag (`Adv(It &i, int n, Tag)`, called `Adv(a, -n, Tag())` from a TAG-LESS `operator-(It a, int n)`):
// the tag slot now sits BELOW the by-value copy `a`, which moves `a` to retail's 28(sp). The only word left is the
// dead store of the decremented value in the final `b` delay slot: ours into `a` (`sw v1,28(sp)`), retail into a
// separate top-of-frame object (`sw v1,44(sp)`). Ruled out at 1 word: a named `It r = operator-(..); return r;`,
// a `CIt` return type with a converting ctor, Adv by pointer, Adv returning It& (`return Adv(a,-n,Tag())`, also via
// a named local), member `--` built on Adv (`return --a;`). Worse: a fresh result object in operator- (`It r(a)`,
// `It r(a.p_)`, `It r = a`, `It(a.p_ - n)`, a second by-value level Sub(It b,..): the dead store disappears, 16-21),
// by-value Adv (19), a named outer `it` local (24), `--end()` (24); Tag-position permutations in operator-/push_back
// with the old operator- (3 or 5).
// improved by lane pn3 (unreviewed): 3 -> 1 differing words
