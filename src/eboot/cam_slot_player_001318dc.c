/* HSTMATCH module=EBOOT.BIN start=0x001318dc end=0x00131a78 symbol=FUN_001318dc lang=c99 data=0x002db1bc */
/* Hand-written reconstruction: clone of the source of cam_slot_player_0013ce94.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cam { char pad[0x128]; int a, b, c; };
struct Ent { char pad[0xf0]; int id; };
struct Sub { char pad[0x58]; struct Ent *e; };
struct Pl { char pad[0x5bc]; struct Sub *sub; };
struct Sl { char pad[0x70]; struct Pl *p[1]; };
struct Mg { char pad[0x50]; int idx; };
extern struct Sl *DAT_003386b0;
extern struct Mg *DAT_00338b44;
int FUN_001318dc(struct Cam *c, int k)
{
    int r = 0;
    struct Pl *pl = 0;
    float t[4] = {1.0f, 0.0f, 3.0f, 2.0f};
    switch (k) {
    case 5: pl = DAT_003386b0->p[c->a]; break;
    case 6: pl = DAT_003386b0->p[c->b]; break;
    case 7: pl = DAT_003386b0->p[DAT_00338b44->idx]; break;
    case 8: pl = DAT_003386b0->p[(int)t[DAT_00338b44->idx]]; break;
    case 9: pl = DAT_003386b0->p[c->c]; break;
    }
    if (pl && pl->sub) {
        struct Ent *e = pl->sub->e;
        int id = e ? e->id : -1;
        switch (id) {
        case 0x3a:
        case 0x3b:
        case 0x3c:
        case 0x3d:
        case 0xe0:
        case 0xe1:
            r = 1;
            break;
        }
    }
    return r;
}
