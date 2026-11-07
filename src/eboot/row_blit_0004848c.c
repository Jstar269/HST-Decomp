/* HSTMATCH module=EBOOT.BIN start=0x0004848c end=0x00048548 symbol=FUN_0004848c lang=c99 */
/* Hand-written reconstruction from the disassembly, adapted from the twin src/eboot/row_blit_000483d0.c in the same module: addresses substituted; exact match verified; names and types await human review. */
extern void FUN_00047920(int a, int b, int c);
extern char DAT_002d0224;
extern void FUN_00004f60(int a, int b);
extern unsigned short *DAT_00310fc0;
extern void FUN_00011090(const void *src, void *dst, int n);
void FUN_0004848c(const char *src, int x, int y, int w, int h)
{
    unsigned short *dst;
    int i;
    FUN_00047920(0, 1, 1);
    DAT_002d0224 = 0;
    FUN_00004f60(0, 0);
    dst = DAT_00310fc0 + (y * 512 + x);
    for (i = 0; i < h; i++) {
        FUN_00011090(src, dst, w * 2);
        dst += 512;
        src += w * 2;
    }
}
