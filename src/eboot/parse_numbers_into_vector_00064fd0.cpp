/* HSTMATCH module=EBOOT.BIN start=0x00064fd0 end=0x00065078 symbol=FUN_00064fd0 lang=c++ */
/* Hand-written reconstruction from the disassembly (restructured from the lane om1 near miss wip/parse_numbers_into_vector_00064fd0.c); exact match verified; matching idioms: the inline push_back takes an unused by-value empty Tag (its never-written slot plus the grow call's Tag() temp put the cursor at sp+16) and the loop exit is `int done = (r != 0) ^ 1;`; names and types await human review. */
/* Splits a text row with FUN_00065078 and appends each converted field to an int vector. */
struct Tag {};
struct Vec;
extern "C" void FUN_00066098(Vec *self, const int *value, Tag);

struct Vec {
    int *data;
    unsigned count;
    unsigned capacity;
    void push_back(const int &x, Tag)
    {
        if (count < capacity) {
            unsigned n = count + 1;
            count = n;
            data[n - 1] = x;
        } else {
            FUN_00066098(this, &x, Tag());
        }
    }
};

extern "C" int FUN_00065078(char *a, char *b, char **out);
extern "C" int FUN_00064820(int s);

extern "C" void FUN_00064fd0(Vec *self, char *p, char *pat)
{
    int v;
    unsigned r;
    for (;;) {
        r = FUN_00065078(p, pat, &p);
        int done = (r != 0) ^ 1;
        if (done) {
            break;
        }
        v = FUN_00064820(r);
        self->push_back(v, Tag());
    }
}
