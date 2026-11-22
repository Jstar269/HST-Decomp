/* HSTMATCH module=EBOOT.BIN start=0x000888dc end=0x000889ec symbol=FUN_000888dc lang=c++ */
struct Obj_000888dc {
    char pad0[0x3c];
    int field_3c;
    char pad1[0xc];
    bool field_4c;
    char pad2[0xc];
    bool field_59;
    char pad3[0x12];
    int field_6c;
    char pad4[0x2c];
    void *field_9c;
};
extern "C" {
extern unsigned char DAT_003386b8;
extern void *DAT_00338c20;
extern void *DAT_00338b44;
int FUN_000fb7b8(void *);
int FUN_001181c0(void *);
bool FUN_000def58(void *);
}
extern "C" bool FUN_000888dc(Obj_000888dc *self)
{
    if (DAT_003386b8 != 0) {
        if (self->field_6c <= 0) {
            return false;
        }
    }
    int ok = 0;
    unsigned char v;
    if (DAT_003386b8 != 0 && FUN_000fb7b8(DAT_00338c20) != 0) {
        v = 0;
    } else {
        if (self->field_4c == 0) {
            switch (self->field_3c) {
            case 5:
            case 7:
                ok = FUN_001181c0(self->field_9c) >= 0;
                break;
            case 6:
                ok = FUN_001181c0(self->field_9c) >= 0 && !FUN_000def58(DAT_00338b44);
                break;
            }
        }
        v = ok;
    }
    bool r = false;
    if (v) {
        r = !self->field_59;
    }
    return r;
}
// NEAR, 1 word differs (matcher: 68/68 words; 1 differ; first at +0xe8): retail `andi v0,v1,0xff` in the beqz slot (return value = v1 as a byte), ours `move v0,zero`.
// Keys found: switch(case 5: case 7: ... case 6:) gives retail's 6,7,5 test order; `unsigned char v` set to 0 on the fb7b8 path and to `ok` otherwise keeps v in v1.
// Ruled out for the tail (13 spellings): bool/unsigned char/int/char r initialised from v with test on v or r (7-17 words), `return v` forms, ternary, int return.
// near miss from lane api-hste2-20261121 (unreviewed)
