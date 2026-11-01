/* HSTMATCH module=EBOOT.BIN start=0x001ff670 end=0x001ff69c symbol=FUN_001ff670 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef struct RecordRoot {
  char pad00[0x40];
  void *owner;
} RecordRoot;

extern RecordRoot *DAT_003386c8;
extern int FUN_0008d9a0(RecordRoot *);

int FUN_001ff670(void)
{
  if (DAT_003386c8) {
    return FUN_0008d9a0(DAT_003386c8);
  }
  return 0;
}
