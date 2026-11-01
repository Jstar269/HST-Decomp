/* HSTMATCH module=EBOOT.BIN start=0x002a1398 end=0x002a15fc symbol=FUN_002a1398 lang=c99 */
/* NEAR MISS (not attempted to a word count): 612-byte blend-motion routine. Per-slot stride
   0x34: reads slot+0xf4 (motion object, float at *(obj)+8), slot+0x104 (current time),
   slot+0x110 (previous request) and slot+0xf0; picks 3.0f or 10.0f (param_2 == 1); looks up
   two ids with FUN_0005787c(param_1, id) and compares their +4 fields; logs through
   FUN_002a0b30 with "Error: cant blend motion" / "OK: blend motion success" and format
   s__s__d__d_____d__d__003006f8; then FUN_00057bc0/FUN_00057b58/FUN_002a15fc by case
   (param_2 == 0, ids differ, ids match) and continues with more float time arithmetic.
   Needs the full pseudocode transcribed; parked before that. */
// near miss from lane dsn8 (unreviewed)
