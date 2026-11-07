/* HSTMATCH module=hstp_trans_main.prx start=0x0013ce94 end=0x0013d030 symbol=FUN_0013ce94 lang=c99 data=0x003823c8 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Cam { char pad[0x128]; int a, b, c; };
struct Ent { char pad[0xf0]; int id; };
struct Sub { char pad[0x58]; struct Ent *e; };
struct Pl { char pad[0x5bc]; struct Sub *sub; };
struct Sl { char pad[0x70]; struct Pl *p[1]; };
struct Mg { char pad[0x50]; int idx; };
extern struct Sl *DAT_003e19f0;
extern struct Mg *DAT_003e1e84;
int FUN_0013ce94(struct Cam *c, int k)
{
    int r = 0;
    struct Pl *pl = 0;
    float t[4] = {1.0f, 0.0f, 3.0f, 2.0f};
    switch (k) {
    case 5: pl = DAT_003e19f0->p[c->a]; break;
    case 6: pl = DAT_003e19f0->p[c->b]; break;
    case 7: pl = DAT_003e19f0->p[DAT_003e1e84->idx]; break;
    case 8: pl = DAT_003e19f0->p[(int)t[DAT_003e1e84->idx]]; break;
    case 9: pl = DAT_003e19f0->p[c->c]; break;
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
