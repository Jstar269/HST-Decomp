/* HSTMATCH module=hstp_trans_main.prx start=0x000bf8a8 end=0x000bf998 symbol=FUN_000bf8a8 lang=c++ helpers=FUN_000bd6f0@0x000bd6f0-0x000bd71c */
/* Hand-written reconstruction: clone of the source of obj_queue_000bc434.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Alloc {};

/* Two-pointer entry stored in the queue vector. */
typedef struct Entry Entry;
struct Entry {
    void *first;
    void *second;
};

typedef struct Vector Vector;
struct Vector {
    Entry **data;
    unsigned int size;
    unsigned int cap;
};

struct VectorAlias_000bc434 {
    Entry **data;
    unsigned int size;
    unsigned int cap;
};

typedef struct Obj Obj;
struct Obj {
    char pad0[0x34];
    void *unk34;
    char pad1[0xc];
    Vector *queue;
};

extern "C" {
extern const char DAT_0037bc98[];
extern void *FUN_00000bcc(unsigned int size);
extern void FUN_002c76c8(void *, int, int);
extern void FUN_002c76b0(void *, int);
extern void FUN_000c2108(Vector *, Entry **, Alloc);

static void FUN_000bd6f0(...) { }

/* Appends the pair (first, second) to the object's entry vector, creating the vector on first use. */
void FUN_000bf8a8(Obj *self, void *first, void *second)
{
    FUN_000bd6f0(DAT_0037bc98);
    FUN_002c76c8(self->unk34, 1, 0);

    if (self->queue == 0) {
        Vector *v = (Vector *)FUN_00000bcc(sizeof(Vector));
        if (v != 0) {
            v->data = 0;
            v->size = 0;
            v->cap = 0;
        }
        self->queue = v;
    }

    Entry *entry = (Entry *)FUN_00000bcc(sizeof(Entry));
    entry->first = first;
    entry->second = second;

    Vector *vec = self->queue;
    if (vec->size < vec->cap) {
        ++vec->size;
        VectorAlias_000bc434 *alias = (VectorAlias_000bc434 *)vec;
        unsigned int index = alias->size - 1;
        vec->data[index] = entry;
    } else {
        FUN_000c2108(vec, &entry, Alloc());
    }

    FUN_002c76b0(self->unk34, 1);
}
}

// Differing instructions (~3 logical words differ):
// 1. +0x1c..+0x28: Retail loads string address addiu a0,a0,-17256 before jal 0xbd6f0 and puts move s1,a2
//    in the delay slot. Our compilation puts move s1,a2 before the call and addiu a0,a0,-17256 in the delay slot.
// 2. +0x94..+0xc8: Vector push_back slow path. Retail emits 'beqzl at, 0xc0; move v0, zero' and at 0xc0
//    'seb a2, v0; jal 0xc2108; addiu a1, sp, 28'. Our code puts 'addiu a1, sp, 28' in the beqzl delay slot.
// Ruled out:
// - Explicit local caching of DAT_0037bc98 (does not prevent delay slot hoisting of addiu).
// - Various types for the 3rd parameter of FUN_000c2108 (char, signed char, Alloc empty struct).
// - Plain C99 mode vs C++ (both exhibit same delay slot scheduling).
// near miss from lane u1r5-3 (unreviewed)
