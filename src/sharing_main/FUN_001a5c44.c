/* HSTMATCH module=hstp_trans_main.prx start=0x001a5c44 end=0x001a5d3c symbol=FUN_001a5c44 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: outer switch lists `default` first so the assert block is laid out first; inner switch on kind has empty cases 0,1,2 and an asserting default */
extern void FUN_001a5b08();
extern const char DAT_0038904c[], DAT_003890ac[], DAT_003890b4[], DAT_00389080[];
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_001a5b08(DAT_0038904c, txt, DAT_00389080, line); for (;;) {} } } while (0)
typedef struct Sub { char pad[44]; int kind; int m30; } Sub;
typedef struct Self { char pad[60]; char m3c[52]; Sub *sub; int pad3[2]; int mode; } Self;
extern int FUN_002c7a78(void *p);
extern char DAT_003eaab1;
void FUN_001a5c44(Self *self, int mode)
{
    self->mode = mode;
    switch (mode) {
    default:
        HST_ASSERT(0, DAT_003890ac, 248);
    case 0:
        switch (self->sub->kind) {
        case 0:
        case 1:
        case 2:
            break;
        default:
            HST_ASSERT(0, DAT_003890ac, 254);
        }
        DAT_003eaab1 = 0;
        break;
    case 1:
        self->sub->m30 = 8;
        HST_ASSERT(FUN_002c7a78(self->m3c) == 0, DAT_003890b4, 269);
        DAT_003eaab1 = 1;
        break;
    }
}
