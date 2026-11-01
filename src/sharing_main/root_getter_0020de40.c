/* HSTMATCH module=hstp_trans_main.prx start=0x0020de40 end=0x0020de6c symbol=FUN_0020de40 lang=c99 */
/* Clone of src/eboot/root_getter_001ff670.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct RecordRoot {
  char pad00[0x40];
  void *owner;
} RecordRoot;

extern RecordRoot *DAT_003e1a08;
extern int FUN_0008f17c(RecordRoot *);

int FUN_0020de40(void)
{
  if (DAT_003e1a08) {
    return FUN_0008f17c(DAT_003e1a08);
  }
  return 0;
}
