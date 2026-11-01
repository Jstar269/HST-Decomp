/* HSTMATCH module=EBOOT.BIN start=0x0005ee5c end=0x0005ef18 symbol=FUN_0005ee5c lang=c++ */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
// release_indexed_item from retail 0x0005ee5c (188 bytes)

struct El;

struct Cont {
    char pad0[160];
    int f_a0;       // 160 (0xa0)
    int pad1;       // 164
    El **f_a8;      // 168 (0xa8)
    int f_ac;       // 172 (0xac)
    int pad2;       // 176
    unsigned int *f_b4; // 180 (0xb4)
};

extern "C" {
void FUN_0005f030(Cont *, int, float);
void FUN_0005d818(void *, int);
}

extern "C" void FUN_0005ee5c(Cont *self, int i)
{
    if (i < 0) return;
    if ((unsigned int)i >= (unsigned int)self->f_ac) return;

    if (self->f_a0 == i) {
        FUN_0005f030(self, -1, 0.0f);
    }

    El **slot = &self->f_a8[i];
    El *e = *slot;
    if (e == 0) return;

    unsigned int val = self->f_b4[(unsigned int)i >> 5];
    if (val & (1u << (i & 31))) {
        if (e != 0) {
            FUN_0005d818(e, 1);
            self->f_a8[(unsigned int)i] = 0;
        }
    } else {
        *slot = 0;
    }
}
