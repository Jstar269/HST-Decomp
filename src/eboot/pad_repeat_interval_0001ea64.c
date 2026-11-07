/* HSTMATCH module=EBOOT.BIN start=0x0001ea64 end=0x0001eae4 symbol=FUN_0001ea64 lang=c99 */
/* Hand-written reconstruction from the disassembly (decomp-opus lane 2026-11-04, Claude Opus 5.5); exact match verified; names and types await human review. */
/* Held-button repeat for a button mask: fires on the first poll, then once `delay` polls are reached and every
   `interval` polls after that. The counts are in updates (polls), so they halve in time at 60 updates a second. */
struct Pad { int pad[0x80 / 4]; unsigned int held; };
int FUN_0001ea64(struct Pad *pad, unsigned int *count, unsigned int mask, int interval, unsigned int delay)
{
  int fire;
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
  else if (n < delay)
    fire = 0;
  else {
    int d = n - delay;
    fire = !(d && d % interval);
  }
  return fire;
}
