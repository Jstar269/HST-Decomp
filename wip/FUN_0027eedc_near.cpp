/* HSTMATCH module=EBOOT.BIN start=0x0027eedc end=0x0027f04c symbol=FUN_0027eedc lang=c++ */
/* Hand-written reconstruction from the disassembly; near miss (41 words differ). */

inline void *operator new(unsigned int, void *p) { return p; }

extern "C" {
extern void *__vt__10C_0027e6d4;
}

struct False {};

struct Pair {
    char a, b;
};

struct C_0027e6d4 {
    virtual ~C_0027e6d4();
    short s[7];
    Pair p[5];
    char c[9];
    unsigned char b;
    short end;

    C_0027e6d4(const C_0027e6d4 &src) {
        for (int i = 0; i < 7; i++) s[i] = src.s[i];
        const char *ps = (const char *)src.p;
        char *pd = (char *)p;
        int i5 = 5;
        do {
            char b1 = ps[1];
            char b0 = ps[0];
            pd[1] = b1;
            pd[0] = b0;
            ps += 2;
            pd += 2;
        } while (--i5 > 0);
        const char *cs = src.c;
        char *cd = c;
        int i9 = 9;
        do {
            char b = *cs++;
            *cd++ = b;
        } while (--i9 > 0);
        b = src.b;
        end = src.end;
    }
};

struct Vector {
    C_0027e6d4 *data;
    unsigned int size;
    unsigned int capacity;
};

extern "C" {
void FUN_0027f814(Vector *, unsigned int);
void FUN_0027f480(Vector *, unsigned int, const C_0027e6d4 &);
}

extern "C" void FUN_0027eedc(Vector *self, unsigned int n, const C_0027e6d4 &elem, False)
{
    unsigned int cap = self->capacity;
    unsigned int sz = self->size;
    if (n > cap || sz > cap - n) {
        FUN_0027f814(self, (sz + n) - cap);
        goto realloc;
    }

    {
        C_0027e6d4 *dest = (C_0027e6d4 *)((char *)self->data + sz * sizeof(C_0027e6d4));
        while (n != 0) {
            new (dest) C_0027e6d4(elem);
            n--;
            dest++;
            self->size++;
        }
        return;
    }

realloc:
    FUN_0027f480(self, n, elem);
}

// 41 words differ.
// vec_append_fast with non-trivial 40-byte element (C_0027e6d4).
// Reaches exact frame 32 via empty struct parameter `False`.
// Inlined copy constructor with rolled 5-pair and 9-char copy loops.
// Hits the MWCC build 219 delay-slot duplication wall documented in docs/MWCC-NOTES.md
// (2026-10-31, 0x00091090, 0x000e863c): MWCC duplicates hoisted v->data/sll and tail call move a0, s2
// across the in-place copy loop when filling beqzl and b delay slots.
// near miss from lane dsn4 (unreviewed)
