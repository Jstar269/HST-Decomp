/* HSTMATCH module=hstp_trans_main.prx start=0x0023bacc end=0x0023bb30 symbol=FUN_0023bacc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00239f10();
extern const char DAT_0039af44[], DAT_0039b1bc[], DAT_0039afa8[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00239f10(DAT_0039af44, DAT_0039b1bc, DAT_0039afa8, line); for (;;) {} } } while (0)
struct G { char pad[0xd4e6]; short h[1]; };
extern struct G *DAT_003ea8e8;
short FUN_0023bacc(int i) { HST_ASSERT(i >= 0 && i < 15, 496); return DAT_003ea8e8->h[i]; }
