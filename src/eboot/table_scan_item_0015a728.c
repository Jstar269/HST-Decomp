/* HSTMATCH module=EBOOT.BIN start=0x0015a728 end=0x0015a844 symbol=FUN_0015a728 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Searches table entries in DAT_0034040c stepping by (step >= 0 ? 1 : -1) wrapping in 0..255 until FUN_0015cce8 returns a valid entry and count reaches zero; returns index if FUN_0015d16c succeeds or flag is set. */

struct Context {
    char pad0[4];
    int index;
};

extern int DAT_0034040c[];
extern int FUN_0000d530(int a);
extern char *FUN_0015cce8(struct Context *ctx, int id);
extern int FUN_0015d16c(struct Context *ctx, int id);

int FUN_0015a728(struct Context *ctx, int step, int flag)
{
    int count = FUN_0000d530(step);
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

        char *entry = FUN_0015cce8(ctx, DAT_0034040c[ctx->index]);
        if (entry != 0 && *entry >= 0) {
            count--;
            if (count <= 0) {
                if (FUN_0015d16c(ctx, DAT_0034040c[ctx->index]))
                    break;
                if (flag)
                    break;
            }
        }

        ctx->index += (dir > 0) ? 1 : -1;
    }
    return ctx->index;
}
