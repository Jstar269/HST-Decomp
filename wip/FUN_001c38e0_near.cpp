/* HSTMATCH module=EBOOT.BIN start=0x001c38e0 end=0x001c39f8 symbol=FUN_001c38e0 lang=c++ */

extern "C" void *FUN_00193118(void *);

struct BaseItem {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual int getType();
    char pad[0x30];
    int id;
};

struct FirstBase {
    char pad[0xe0];
};

struct Derived : FirstBase, BaseItem {
};

struct Info {
    char pad[0x36];
    short f36;
    short pad2;
    short f3a;
};

struct Container {
    char pad[0x40];
    BaseItem **first;
    int count;
};

struct Self {
    char pad[0x18];
    Container *container;
};

struct Tag {};

extern "C" bool FUN_001c38e0(Self *self, void *param_2, Derived *&param_3, Derived *&param_4, Tag)
{
    BaseItem *item;
    Info *info;
    BaseItem **it;
    BaseItem **last;

    info = (Info *)FUN_00193118(param_2);
    if (info == 0) {
        return false;
    }

    param_3 = 0;
    param_4 = 0;

    it = self->container->first;
    last = it + self->container->count;

    for (; (it == last) ^ 1; ++it) {
        item = *it;
        if (item->getType() == 3) {
            if (info->f36 == item->id) {
                param_3 = (Derived *)item;
            }
            if (info->f3a == item->id) {
                param_4 = (Derived *)item;
            }
        }
    }

    return param_3 != 0;
}

// Differing words: 13 / 70 words.
// Residual difference: single register colour pair (s1 vs s2).
// Retail assigns s1 = it, s2 = info, s3 = item.
// Ours assigns s1 = info, s2 = it, s3 = item.
// All instruction semantics, control flow, delay slots, 48-byte stack frame (via Tag parameter),
// virtual call slot 13 (getType), downcast -0xe0 (via Derived : FirstBase, BaseItem),
// and loop guard ((it == last) ^ 1) match retail byte-for-byte.
// Tested declaration permutations of {it, info, last, item}; s1/s2 swap is an MWCC register allocation preference.
// near miss from lane dsn4 (unreviewed)
