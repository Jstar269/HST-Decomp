/* HSTMATCH module=hstp_trans_main.prx start=0x00166d00 end=0x00166e1c symbol=FUN_00166d00 lang=c99 */
/* Clone of src/eboot/table_scan_item_0015a728.c (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Searches table entries in DAT_003e974c stepping by (step >= 0 ? 1 : -1) wrapping in 0..255 until FUN_00169290 returns a valid entry and count reaches zero; returns index if FUN_00169714 succeeds or flag is set. */

struct Context {
    char pad0[4];
    int index;
};

extern int DAT_003e974c[];
extern int FUN_0000d548(int a);
extern char *FUN_00169290(struct Context *ctx, int id);
extern int FUN_00169714(struct Context *ctx, int id);

int FUN_00166d00(struct Context *ctx, int step, int flag)
{
    int count = FUN_0000d548(step);
    int dir;
    if (step >= 0) {
        dir = 1;
    } else {
        dir = -1;
    }

    ctx->index += dir;
    while (1) {
        if (ctx->index < 0) {
            ctx->index = 255;
        } else if (ctx->index >= 256) {
            ctx->index = 0;
        }

        char *entry = FUN_00169290(ctx, DAT_003e974c[ctx->index]);
        if (entry != 0 && *entry >= 0) {
            count--;
            if (count <= 0) {
                if (FUN_00169714(ctx, DAT_003e974c[ctx->index]))
                    break;
                if (flag)
                    break;
            }
        }

        ctx->index += (dir > 0) ? 1 : -1;
    }
    return ctx->index;
}
