/* HSTMATCH module=hstp_trans_main.prx start=0x0023a868 end=0x0023a8c8 symbol=FUN_0023a868 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the biased index is held in a local (int j = i + BIAS) before the subscript, which keeps retail's separate add of the bias. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b104[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b104, DAT_0039afa8, line); for (;;) {} } } while (0)
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003ea8e8;
unsigned char FUN_0023a868(int i) { HST_ASSERT(i >= 0 && i < 1024, 244); int j = i + 0x8800; return DAT_003ea8e8->b[j]; }
