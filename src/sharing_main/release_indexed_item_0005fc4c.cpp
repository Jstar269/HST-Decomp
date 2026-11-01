/* HSTMATCH module=hstp_trans_main.prx start=0x0005fc4c end=0x0005fd08 symbol=FUN_0005fc4c lang=c++ */
/* Clone of src/eboot/release_indexed_item_0005ee5c.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
// release_indexed_item from retail 0x0005fc4c (188 bytes)

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
void FUN_0005fe20(Cont *, int, float);
void FUN_0005e634(void *, int);
}

extern "C" void FUN_0005fc4c(Cont *self, int i)
{
    if (i < 0) return;
    if ((unsigned int)i >= (unsigned int)self->f_ac) return;

    if (self->f_a0 == i) {
        FUN_0005fe20(self, -1, 0.0f);
    }

    El **slot = &self->f_a8[i];
    El *e = *slot;
    if (e == 0) return;

    unsigned int val = self->f_b4[(unsigned int)i >> 5];
    if (val & (1u << (i & 31))) {
        if (e != 0) {
            FUN_0005e634(e, 1);
            self->f_a8[(unsigned int)i] = 0;
        }
    } else {
        *slot = 0;
    }
}
