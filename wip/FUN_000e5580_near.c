/* HSTMATCH module=EBOOT.BIN start=0x000e5580 end=0x000e5650 symbol=FUN_000e5580 lang=c99 */
/* VARIANT v2: retail's fill exits carry `move v0,s0` twice, so the function RETURNS the object
   pointer; the park is void, which is the missing work (2 words). Pointer walk for the fill. */
extern void __ct__10B_0001f06cFv(void *self);
extern char DAT_00307654;
char *FUN_000e5580(char *self)
{
  int i, n;
  char *p;
  __ct__10B_0001f06cFv(self);
  *(void **)self = &DAT_00307654;
  char *end = (char *)self + 192;
  p = self + 60;
  n = 192 - (int)(p - self);
  (void)i; (void)n;
  while (p < end) *p++ = 0;
  return self;
}
// improved by lane dsn3 (unreviewed): 47 -> 45 differing words
