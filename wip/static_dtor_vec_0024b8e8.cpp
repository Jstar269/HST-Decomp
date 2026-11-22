/* HSTMATCH module=EBOOT.BIN start=0x0024b8e8 end=0x0024ba68 symbol=FUN_0024b8e8 lang=c++ */
#pragma opt_strength_reduction off
extern "C" {
extern void *DAT_00309910;
void FUN_00000a00(void *);
void FUN_00053718(void *, int);
void FUN_0022909c(void *, void *, void *, ...);
void FUN_001466dc(void *, int);
void FUN_00056cb0(void *, int);
void FUN_0001f0f0(void *, int);
}

struct Alloc {};

struct Tag {
    char dummy;
    char pad[3];
};

template <class T> inline void Swap(T &a, T &b) { T t = a; a = b; b = t; }

struct VecVoid {
    void **data;
    unsigned int size;
    unsigned int cap;

    void shrink(int n, Alloc) { size -= n; }
    void clear(Alloc a = Alloc()) { shrink(size, a); }
};

struct Obj {
    void *vptr;
};

extern "C" {
extern VecVoid DAT_0034a768;
extern int DAT_0034a774;
}

extern "C" void *FUN_0024b8e8(Obj *self, short param_2) {
    if (self != 0) {
        self->vptr = &DAT_00309910;
        DAT_0034a774 = DAT_0034a774 - 1;
        if (DAT_0034a774 < 1) {
            unsigned int i = 0;
            if (DAT_0034a768.size != 0) {
                do {
                    if (DAT_0034a768.data[i] != 0) {
                        FUN_00053718(DAT_0034a768.data[i], 1);
                        DAT_0034a768.data[i] = 0;
                    }
                    i++;
                } while (i < DAT_0034a768.size);
            }

            DAT_0034a768.clear();

            VecVoid temp;
            Tag tag;
            temp.data = 0;
            temp.size = 0;
            temp.cap = 0;
            FUN_0022909c(&temp, DAT_0034a768.data, DAT_0034a768.data + DAT_0034a768.size, tag.dummy);
            Swap(temp.cap, DAT_0034a768.cap);
            Swap(temp.data, DAT_0034a768.data);
            Swap(temp.size, DAT_0034a768.size);
            FUN_001466dc(&temp, 0);
        }
        FUN_00056cb0((char *)self + 0x50, -1);
        FUN_0001f0f0(self, 0);
        if (param_2 > 0) {
            FUN_00000a00(self);
        }
    }
    return self;
}
// NEAR, 10 words differ (matcher: 96/96 words; 10 differ; first at +0xa8), lane api-hste2-20261121 (was 34 in wip/FUN_0024b8e8_near_96of96_34diff.c). Same recipe as wip/deleting_dtor_00266d94_b.cpp:
// C++ with a VecVoid{data,size,cap} + inline clear()/shrink(), `#pragma opt_strength_reduction off` (retail recomputes `sll s0,s1,2` in the destroy loop), and the swap written TEMP FIRST
// (Swap(temp.cap, v.cap); Swap(temp.data, v.data); Swap(temp.size, v.size)). Left: the `v.clear()` block: retail keeps `lw a1,data` AFTER `sw size` (fresh `lui` each), ours hoists the data load.
// Tried: whole shrink-to-fit in an inline helper taking VecVoid &, pointer-local clear().
// near miss from lane api-hste2-20261121 (unreviewed)
