/* HSTMATCH module=hstp_trans_main.prx start=0x000e415c end=0x000e4290 symbol=FUN_000e415c lang=c++ */
/* Clone of src/eboot/decay_value_000dff70.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    char pad0[0x6c];
    float f6c;
    char pad1[8];
    int f78;
    int f7c;
};

extern "C" void FUN_000e415c(Obj *o)
{
    if (o->f7c == 0) {
        double d = (double)o->f6c;
        d -= 13.0 / 3.0;
        o->f6c = (float)d;
    } else if (o->f7c == 1) {
        double d = (double)o->f6c;
        d -= 20.0 / 3.0;
        o->f6c = (float)d;
    } else if (o->f7c == 2) {
        double d = (double)o->f6c;
        d -= 5.0;
        o->f6c = (float)d;
    } else if (o->f7c == 3) {
        o->f6c = 128.0f - (o->f78 * 128.0f) / 7.0f;
    }
    if (o->f6c < 0.0f) {
        o->f6c = 0.0f;
    }
    o->f78++;
}
