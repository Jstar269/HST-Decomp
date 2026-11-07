/* HSTMATCH module=hstp_trans_main.prx start=0x00049020 end=0x00049084 symbol=FUN_00049020 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int FUN_00049084(void);
extern int FUN_002c76b0(int, int);
extern int DAT_00375e84;
extern void FUN_00048d18();
extern const char DAT_00375e94[], DAT_00375ec0[], DAT_00375ecc[];
void FUN_00049020(void) { int r = 0; if (FUN_00049084()) { r = FUN_002c76b0(DAT_00375e84, 1); if (!(r >= 0)) { FUN_00048d18(DAT_00375e94, DAT_00375ec0, DAT_00375ecc, 207); for (;;) {} } }  }
