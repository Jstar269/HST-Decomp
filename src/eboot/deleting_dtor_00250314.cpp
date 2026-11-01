/* HSTMATCH module=EBOOT.BIN start=0x00250314 end=0x00250470 symbol=FUN_00250314 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" {
void FUN_00000a00(void *);
void FUN_00179a70(void *, short);
void FUN_00102ea4(void *, short);
extern void *DAT_00309948;
}

struct BaseObj {
    virtual ~BaseObj();
};

struct Item {};

struct False {};

struct MyClass {
    void *vtable;
    char pad04[476 - 4];
    BaseObj *arr[16];
    BaseObj *unk540;
    char pad544[848 - 544];
    Item **items;
    int items_cnt;
};

extern "C" MyClass *FUN_00250314(MyClass *self, short flags, False)
{
    if (self != 0) {
        self->vtable = &DAT_00309948;

        for (int i = 0; i < 16; ++i) {
            if (self->arr[i] != 0) {
                delete self->arr[i];
                self->arr[i] = 0;
            }
        }

        if (self->unk540 != 0) {
            delete self->unk540;
            self->unk540 = 0;
        }

        int a1 = self->items_cnt;
        Item **s0 = self->items;
        Item **end = (Item **)((char *)s0 + (a1 << 2));
        int more = (s0 == end) ^ 1;
        if (more) {
            do {
                if (*s0 != 0) {
                    FUN_00000a00(*s0);
                }
                a1 = self->items_cnt;
                s0++;
                more = (s0 == (Item **)((char *)self->items + (a1 << 2))) ^ 1;
            } while (more);
        }

        self->items_cnt -= a1;

        if (&self->items != 0 && &self->items != 0) {
            FUN_00179a70(&self->items, 0);
        }
        FUN_00102ea4(self, 0);

        if (flags > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
