/* HSTMATCH module=EBOOT.BIN start=0x00055798 end=0x000558dc symbol=FUN_00055798 lang=c99 */
/* NEAR MISS (not attempted to a word count): 324-byte method that (a) substitutes a global
   fallback object when the second argument is null (DAT_003313c4), logging through
   0x00050824/0x00050a10 with the message at 0x00311170, (b) asserts the float at this+0x10
   is >= 0.0f, (c) builds a projection matrix from .rodata floats 0x002d0ef0..0x002d0f04 and
   the value at this+0x80 scaled by 0x3d088889, (d) calls 0x000555b4 with (sp+16, this),
   (e) makes a virtual call through *(this)->vtable+0x20, and (f) calls 0x0003bc08 with
   {DAT_0033b17c, *(this+0x8c)->+8, result, sp+16, sp+88}. Needs the decompiler pseudocode
   for the matrix/assert sequence; not reconstructed here. */
// near miss from lane dsn8 (unreviewed)
