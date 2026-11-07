/* HSTMATCH module=EBOOT.BIN start=0x000483d0 end=0x0004848c symbol=FUN_000483d0 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00047920(int a, int b, int c);
extern char DAT_002d0224;
extern void FUN_00004f60(int a, int b);
extern unsigned short *DAT_00310fc0;
extern void FUN_00011090(const void *src, void *dst, int n);
void FUN_000483d0(const char *src, int x, int y, int w, int h)
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
