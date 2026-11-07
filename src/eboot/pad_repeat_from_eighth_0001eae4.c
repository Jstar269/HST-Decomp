/* HSTMATCH module=EBOOT.BIN start=0x0001eae4 end=0x0001eb44 symbol=FUN_0001eae4 lang=c99 */
/* Hand-written reconstruction from the disassembly (decomp-opus lane 2026-11-04, Claude Opus 5.5); the n < 8 case returns 0 directly, so only the 1 paths take the unsigned char conversion; exact match verified; names and types await human review. */
/* Held-button repeat for a button mask: fires on the first poll, then on every poll from the 8th on. The counts are in updates (polls), so the delay halves in time at 60 updates a second. */
struct Pad { int pad[0x80 / 4]; unsigned int held; };
unsigned char FUN_0001eae4(struct Pad *pad, unsigned int *count, unsigned int mask)
{
  unsigned char fire;
  unsigned int n;
  if (pad->held & mask) {
    n = *count + 1;
    *count = n;
    if (n == 0) {
      *count = 0xffffffff;
      return 0;
    }
  } else {
    *count = 0;
    return 0;
  }
  if (n == 1)
    fire = 1;
  else if (n < 8)
    return 0;
  else
    fire = 1;
  return fire;
}
