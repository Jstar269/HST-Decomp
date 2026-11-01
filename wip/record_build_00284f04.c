/* HSTMATCH module=EBOOT.BIN start=0x00284f04 end=0x00285110 symbol=FUN_00284f04 lang=c99 */
/* NEAR MISS (not attempted to a word count): 524-byte serializer. Builds a local 256-byte
   record on the stack: FUN_0008d388(rec,0,0,0,0,0,0), FUN_0008d2ec(rec), FUN_00097b2c(rec,
   param_2), copies bytes param_2+0x9c..0xa0 and words param_2+0xa4..0xb8 plus the
   six-halfword array at param_2+0xbc (3 x lhu/lhu/sh/sh pairs), FUN_000149a8(local_54,
   param_2+0xc9) (32 bytes), copies param_2+0xec..0xf1, then FUN_00285110(param_1,rec) and
   scatters the record fields back out to param_1+0xc8..0x11f (0xc9..0xcb bytes, 0xcc..0xeb
   in 16 two-byte pairs, 0xec..0x101 bytes/words, 0x108..0x117 in 8 two-byte pairs,
   0x118/0x11c words) and finally FUN_000d7e30(rec, -1). Needs the stack record laid out
   exactly as Ghidra's auStack_120/local_* sequence to match; not reconstructed here. */
// near miss from lane dsn8 (unreviewed)
