/* HSTMATCH module=EBOOT.BIN start=0x00089e9c end=0x0008a1b0 symbol=FUN_00089e9c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; the player slot is addressed as a byte offset `(char *)o->players + (s << 2)` (retail adds the scaled index first); names and types await human review. */

struct Cfg {
    char pad[0xa0];
    unsigned fa0;
    unsigned fa4;
};

struct Net {
    char pad[0x8c];
    int mode;
    char pad90[0x128 - 0x90];
    int slot;
};

struct Flags {
    char pad[0x8b];
    signed char lo : 2;
    signed char busy : 2;
};

struct Player {
    char pad[0x5bc];
    void *f5bc;
};

struct Seq {
    char pad[0x40];
    int next;
    int step;
    int timer;
    char pad4c[0x6c - 0x4c];
    int f6c;
    struct Player *players[4];
    char pad80[0xa8 - 0x80];
    void *fa8;
};

extern struct Cfg *DAT_00338b44;
extern struct Net *DAT_00340284;
extern struct Flags *DAT_002cfc8c;
extern int DAT_002d23f4;
extern int DAT_002d23f8;
extern int DAT_002d23fc;
extern int DAT_002d2400;
extern int DAT_002d2404;
extern int DAT_002d2408;
extern unsigned char DAT_003386b8;

extern int FUN_000def58(struct Cfg *);
extern void FUN_0008ab10(struct Seq *);
extern int FUN_0008a72c(struct Seq *, int);
extern void FUN_0008ad40(struct Seq *);
extern int FUN_0008add8(struct Seq *);
extern int FUN_001b5578(void *);
extern void FUN_0001f558(struct Seq *, int, int, int);
extern int FUN_000b35c4(struct Player *);
extern int FUN_00093b00(void *, int);

static inline int SlotOf(struct Net *n)
{
    return n->mode == 2 ? n->slot : -1;
}

void FUN_00089e9c(struct Seq *o)
{
    int wait = FUN_000def58(DAT_00338b44) ? DAT_002d23fc : DAT_002d23f8;
    int hold;
    if (FUN_000def58(DAT_00338b44))
        hold = DAT_002d2408;
    else {
        struct Cfg *c = DAT_00338b44;
        if (c->fa4 == 0 || c->fa4 == 1 || c->fa0 < 2)
            hold = DAT_002d2404;
        else
            hold = DAT_002d2400;
    }
    FUN_0008ab10(o);
    if (!FUN_000def58(DAT_00338b44) && FUN_0008a72c(o, 0))
        return;
    switch (o->step) {
    case 0:
        o->timer = 0;
        o->step++;
        break;
    case 1: {
        int t = o->timer + 1;
        int lt = t < DAT_002d23f4;
        o->timer = t;
        if (!lt && FUN_001b5578(o->fa8) == 0) {
            FUN_0001f558(o, 21, 0, 0);
            o->step++;
            break;
        }
        if (o->timer >= hold && DAT_002cfc8c->busy == 0)
            o->step++;
    }
    case 2: {
        struct Net *n = DAT_00340284;
        int ok = 0;
        if (n->mode == 2) {
            int s = SlotOf(n);
            if (s >= 0 && (unsigned)s < 4) {
                struct Player **pp = (struct Player **)((char *)o->players + (s << 2));
                if (FUN_000b35c4(*pp) && FUN_00093b00((*pp)->f5bc, -1))
                    ok = 1;
            }
        }
        if (!ok) {
            int t = o->timer + 1;
            o->timer = t;
            if ((t < wait || FUN_001b5578(o->fa8) != 0) && (o->timer < hold || DAT_002cfc8c->busy != 0))
                break;
        }
        o->step++;
        break;
    }
    case 3:
        if (FUN_000def58(DAT_00338b44))
            o->next = 9;
        else {
            FUN_0008ad40(o);
            if (FUN_0008add8(o)) {
                if (DAT_00338b44->fa4 < 2)
                    o->next = 8;
                else
                    o->next = 4;
            }
        }
        break;
    }
    if (DAT_003386b8 != 0 && o->f6c > 0)
        o->f6c--;
}
