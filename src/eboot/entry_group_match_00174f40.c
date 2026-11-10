/* HSTMATCH module=EBOOT.BIN start=0x00174f40 end=0x0017523c symbol=FUN_00174f40 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Registry entry returned by FUN_000d8c24; the low six bits of byte 0x63 hold a signed group number. */
typedef struct Ent {
    char pad0[0x63];
    signed char group : 6; /* 0x63 */
} Ent;

extern void *DAT_00338b08;
extern int FUN_0017951c(void *self, int kind);
extern Ent *FUN_000d8c24(void *reg, int kind, int id);

/* Compares the group numbers of the kind-1/3/4 entries selected by mode (0, 2 or 3) and raises the matching flags; returns whether any flag was raised. */
unsigned char FUN_00174f40(void *self, unsigned char *a, unsigned char *b, unsigned char *c, int mode, int id)
{
    unsigned char r = 0;
    int id0 = FUN_0017951c(self, 0);
    int id2 = FUN_0017951c(self, 2);
    int id3 = FUN_0017951c(self, 3);
    Ent *e1;
    Ent *e3;
    Ent *e4;
    *a = 0;
    *b = 0;
    *c = 0;
    switch (mode) {
    case 0:
        e1 = FUN_000d8c24(DAT_00338b08, 1, id);
        e3 = FUN_000d8c24(DAT_00338b08, 3, id2);
        e4 = FUN_000d8c24(DAT_00338b08, 4, id3);
        if (e1->group > 0) {
            if (e1->group == e3->group) {
                *b = 1;
                r = 1;
            }
            if (e1->group == e4->group) {
                *c = 1;
                r = 1;
            }
        }
        break;
    case 2:
        e1 = FUN_000d8c24(DAT_00338b08, 1, id0);
        e3 = FUN_000d8c24(DAT_00338b08, 3, id);
        e4 = FUN_000d8c24(DAT_00338b08, 4, id3);
        if (e3->group > 0) {
            if (e3->group == e1->group) {
                *a = 1;
                r = 1;
            }
            if (e3->group == e4->group) {
                *c = 1;
                r = 1;
            }
        }
        if (id != 0 && id == id3) {
            *c = 1;
            r = 1;
        }
        break;
    case 3:
        e1 = FUN_000d8c24(DAT_00338b08, 1, id0);
        e3 = FUN_000d8c24(DAT_00338b08, 3, id2);
        e4 = FUN_000d8c24(DAT_00338b08, 4, id);
        if (e4->group > 0) {
            if (e4->group == e1->group) {
                *a = 1;
                r = 1;
            }
            if (e4->group == e3->group) {
                *b = 1;
                r = 1;
            }
        }
        if (id != 0 && id == id2) {
            *b = 1;
            r = 1;
        }
        break;
    }
    return r;
}
