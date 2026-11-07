/* HSTMATCH module=hstp_trans_main.prx start=0x00048250 end=0x000483e8 symbol=FUN_00048250 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0000449c(), FUN_000045d8(), FUN_00004c3c(), FUN_00005054(), FUN_000050c8(), FUN_0000511c(), FUN_000051d0();
extern void FUN_00005294(), FUN_000052d0(), FUN_00005270(), FUN_00005234(), FUN_0000520c(), FUN_00005174(), FUN_00005198();
extern void FUN_00005188(), FUN_000051a8(), FUN_00004f44(), FUN_00004f78(), FUN_00005b4c(), FUN_000483e8(), FUN_00062d10();
extern unsigned FUN_00004f60();
extern void FUN_00046664();
extern void FUN_00048200();
extern const char DAT_00375e68[];
extern unsigned char DAT_0037596c;
extern int DAT_003d458c;
extern unsigned char DAT_003b92c0;
void FUN_00048250(void)
{
    unsigned u;
    FUN_0000449c();
    FUN_000045d8(0, &DAT_003b92c0, 0x1000);
    FUN_00004c3c(1, 0, 0x200);
    FUN_00005054(0x1e0, 0x110, 0x44000, 0x200);
    FUN_000050c8(0x88000, 0x200);
    FUN_0000511c(6);
    DAT_003d458c = 1;
    FUN_000051d0(0x710, 0x778);
    FUN_00005294(0, 0, 0x1e0, 0x110);
    FUN_0000511c(2);
    FUN_0000511c(10);
    FUN_0000511c(8);
    FUN_000052d0(0, 2, 3, 0, 0);
    FUN_0000511c(4);
    FUN_00005270(7);
    FUN_0000511c(1);
    FUN_00005234(6, 0, 0xff);
    FUN_0000511c(0);
    FUN_0000520c(1);
    FUN_00005174(0);
    FUN_00005198(0);
    FUN_00005188(0);
    FUN_000051a8(0x17);
    u = FUN_00004f60();
    if (u > 0x1000) {
        FUN_00046664(DAT_00375e68, FUN_00004f60());
        for (;;) {}
    }
    FUN_00004f44();
    FUN_00004f78(0, 0);
    FUN_00005b4c(0, FUN_00048200, 0);
    DAT_0037596c = 1;
    FUN_000483e8();
    FUN_00062d10();
}
