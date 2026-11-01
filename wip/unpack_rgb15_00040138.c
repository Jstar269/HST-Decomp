/* HSTMATCH module=EBOOT.BIN start=0x00040138 end=0x00040194 symbol=FUN_00040138 lang=c99 */

extern float DAT_002bbfec;

void FUN_00040138(float param_1, float *param_2, short param_3)
{
  unsigned int c = (unsigned short)param_3;
  param_2[2] = (float)((c & 0x7fff) >> 10) * DAT_002bbfec;
  *param_2 = (float)(c & 0x1f) * DAT_002bbfec;
  param_2[1] = (float)((c & 0x3ff) >> 5) * DAT_002bbfec;
  if (param_3 >= 0) {
    param_1 = 0.0f;
  }
  param_2[3] = param_1;
}
// NEAR 53/23 words: retail extracts the 5-bit fields with Allegrex `ext v0,a1,5,5` /
// `ext v0,a1,10,5` and converts with no sign fixup; every C spelling tried (mask+shift,
// unsigned short casts, bitfield structs) makes mwccpsp emit `srl/andi` plus a
// sign-fixup path, and the bitfield struct does not accept the (short) cast at all.
// Ruled out: unsigned/short param, unsigned casts, bitfield struct.
// near miss from lane dsn8 (unreviewed)
