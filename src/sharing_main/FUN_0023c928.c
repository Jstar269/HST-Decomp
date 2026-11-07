/* HSTMATCH module=hstp_trans_main.prx start=0x0023c928 end=0x0023ca18 symbol=FUN_0023c928 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039afa8[], DAT_0039b2f8[], DAT_0039b330[], DAT_0039b36c[];
#define HST_ASSERT(cond, txt, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, txt, DAT_0039afa8, line); for (;;) {} } } while (0)
typedef struct G { char pad[13568]; char t[1]; } G;
extern G *DAT_003ea8e8;
unsigned char FUN_0023c928(int a, int b, int c)
{
    HST_ASSERT(a >= 0 && (unsigned)a < 10, DAT_0039b2f8, 823);
    HST_ASSERT(b >= 0 && (unsigned)b < 6, DAT_0039b330, 824);
    HST_ASSERT(c >= 0 && (unsigned)c < 16, DAT_0039b36c, 825);
    int j = c + 36112;
    return DAT_003ea8e8->t[a * 96 + b * 16 + j];
}
