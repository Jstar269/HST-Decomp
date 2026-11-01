/* HSTMATCH module=hstp_trans_main.prx start=0x000bf394 end=0x000bf498 symbol=FUN_000bf394 lang=c++ */
/* Clone of src/eboot/FUN_000bbf20.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" {
void FUN_000104e0(int);
void FUN_000c1f48(void *, int);
void __dl__FPv(void *);
}

struct Item {
    virtual ~Item();
    char pad04[8];
    int field_c;
};

struct Vector {
    Item **data;
    int size;
};

struct Owner {
    char pad00[80];
    Vector *vec;
};

struct False {};

/* Cleans up the item vector: deletes each element after clearing its field_c, then deletes the vector. */
extern "C" void FUN_000bf394(Owner *self, False tag)
{
    Vector *vec = self->vec;
    if (vec != 0) {
        Item **it = vec->data;
        Item **last = (Item **)((char *)it + (vec->size << 2));
        if (((it == last) ^ 1) != 0) {
            do {
                int field = (*it)->field_c;
                if (field != 0) {
                    FUN_000104e0(field);
                    (*it)->field_c = 0;
                }
                if (*it != 0) {
                    delete *it;
                    *it = 0;
                }
                vec = self->vec;
                it++;
            } while (((it == (Item **)((char *)vec->data + (vec->size << 2))) ^ 1) != 0);
        }
        if (vec != 0) {
            if (vec != 0) {
                if (vec != 0) {
                    FUN_000c1f48(vec, 0);
                }
                __dl__FPv(vec);
            }
            self->vec = 0;
        }
    }
}
