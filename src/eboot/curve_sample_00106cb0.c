/* HSTMATCH module=EBOOT.BIN start=0x00106cb0 end=0x00106da0 symbol=FUN_00106cb0 lang=c99 */
/* Hand-written reconstruction from the disassembly (started from the lane-fm6 wip draft); exact match verified; names and types await human review. */
typedef struct {
    short x, y, z, w;
} Entry;

typedef struct {
    int count;
    Entry *entries;
} Curve;

typedef struct {
    char pad0[8];
    int w;
    int h;
    char pad1[0x54 - 0x10];
    Curve *curve;
} Obj;

int FUN_00106cb0(Obj *obj, int idx, float *a, float *b, float *c, float *d)
{
    int ret = 1;

    if (idx < 0 || obj->curve->count < 2) {
        *a = 0.0f;
        *b = 0.0f;
        *c = (float)obj->w;
        *d = (float)obj->h;
    } else if (idx + 1 >= obj->curve->count) {
        *a = 0.0f;
        *b = 0.0f;
        *c = (float)obj->w;
        *d = (float)obj->h;
        ret = 0;
    } else {
        *a = (float)obj->curve->entries[idx].x;
        *b = (float)obj->curve->entries[idx].y;
        *c = (float)(obj->curve->entries[idx].z + 1);
        *d = (float)(obj->curve->entries[idx].w + 1);
    }
    return ret;
}
