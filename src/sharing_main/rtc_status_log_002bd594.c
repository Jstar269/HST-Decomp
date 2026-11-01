/* HSTMATCH module=hstp_trans_main.prx start=0x002bd594 end=0x002bd5c8 symbol=FUN_002bd594 lang=c99 */
/* Clone of src/eboot/rtc_status_log_002add6c.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern int FUN_002c7ab0(void *);
extern void FUN_002bd49c(const char *, int);
extern char DAT_003abe04[];

void FUN_002bd594(int param_1)
{
  int r = FUN_002c7ab0((void *)(param_1 + 0x38));
  if (r != 0) {
    FUN_002bd49c(DAT_003abe04, r);
  }
}
