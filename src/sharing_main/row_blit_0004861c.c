/* HSTMATCH module=hstp_trans_main.prx start=0x0004861c end=0x000486d8 symbol=FUN_0004861c lang=c99 */
/* Hand-written reconstruction from the disassembly, adapted from the twin src/eboot/row_blit_000483d0.c in the other module: addresses substituted; exact match verified; names and types await human review. */
extern void FUN_00047da4(int a, int b, int c);
extern char DAT_0037596c;
extern void FUN_00004f78(int a, int b);
extern unsigned short *DAT_003ba2c0;
extern void FUN_00011090(const void *src, void *dst, int n);
void FUN_0004861c(const char *src, int x, int y, int w, int h)
{
    unsigned short *dst;
    int i;
    FUN_00047da4(0, 1, 1);
    DAT_0037596c = 0;
    FUN_00004f78(0, 0);
    dst = DAT_003ba2c0 + (y * 512 + x);
    for (i = 0; i < h; i++) {
        FUN_00011090(src, dst, w * 2);
        dst += 512;
        src += w * 2;
    }
}
