/* HSTMATCH module=hstp_trans_main.prx start=0x000bab80 end=0x000bac2c symbol=FUN_000bab80 lang=c++ */
/* Clone of src/eboot/rotate_records_000b7890.cpp (hstclone.py tag twin11100306-hstp_trans_main.prx): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (std::rotate, forward-iterator form, 80-byte records; matching idiom: first read through `*(It *)&first.p` so it is homed in memory, trailing empty Tag parameter); exact match verified; names and types await human review. */

struct Rec {
    char data[80];
} __attribute__((aligned(16)));

struct Tag {};
struct It {
    Rec *p;
    It(Rec *q = 0) : p(q) {}
};

extern "C" {
Rec *FUN_000b759c(It *);
void FUN_000b74a8(Rec *, Rec *);
}

static inline void Swap(It a, It b)
{
    Rec *x = FUN_000b759c(&a);
    Rec *y = FUN_000b759c(&b);
    FUN_000b74a8(x, y);
}

extern "C" void FUN_000bab80(It first, It middle, It last, Tag)
{
    It i = middle;
    while (true) {
        Swap(*(It *)&first.p, i);
        ++first.p;
        if (first.p == middle.p) {
            ++i.p;
            if (i.p == last.p) return;
            middle = i;
        } else {
            ++i.p;
            if (i.p == last.p) i = middle;
        }
    }
}
