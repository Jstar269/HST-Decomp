/* HSTMATCH module=hstp_trans_main.prx start=0x0023c3cc end=0x0023c434 symbol=FUN_0023c3cc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b2c4[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b2c4, DAT_0039afa8, line); for (;;) {} } } while (0)
struct G { char pad[0x3500]; unsigned char b[1]; };
extern struct G *DAT_003ea8e8;
int FUN_0023c3cc(int i) { HST_ASSERT(i >= 0 && (unsigned)i < 16, 40); int j = i + 0x8d00; return (DAT_003ea8e8->b[j] & 1) != 0; }
