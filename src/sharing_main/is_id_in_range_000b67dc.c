/* HSTMATCH module=hstp_trans_main.prx start=0x000b67dc end=0x000b680c symbol=FUN_000b67dc */
/* Clone of src/eboot/is_id_in_range_000b35c4.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Inner {
    char pad[0xf0];
    int val;
};

struct Mid {
    char pad[0x58];
    struct Inner *inner;
};

struct Outer {
    char pad[0x5bc];
    struct Mid *mid;
};

int FUN_000b67dc(struct Outer *self)
{
    struct Inner *p = self->mid->inner;
    int v = p ? p->val : -1;
    return v >= 198 && v < 224;
}
