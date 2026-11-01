/* HSTMATCH module=EBOOT.BIN start=0x00049da4 end=0x00049e68 symbol=FUN_00049da4 lang=c99 */
float FUN_00049da4(int *param_1)
{
  float f;
  int v;
  int *obj;

  if (param_1[2] < 0) {
    f = 0.7853982f;
  }
  else if (param_1[6] < 0) {
    obj = (int *)*param_1;
    v = (*(int (**)(int *))(*(int *)obj + 0x18))(obj);
    f = -*(float *)(v + param_1[5] * 0x40 + 0x30) * 0.017453292f;
  }
  else {
    obj = (int *)*param_1;
    v = (*(int (**)(int *))(*(int *)obj + 0x18))(obj);
    f = -*(float *)(v + param_1[5] * 0x40 + 0x34) * 0.017453292f * 0.57f;
  }
  return f;
}
// NEAR 49/49 words, 31 differ, first +0x10. Retail decoded: p[2]<0 -> 0.7853982f at the tail; else the
// virtual call (*(obj)+0x18) is made INSIDE each arm (two jalr sites, hence 49 words not 44); p[6]<0 reads
// +0x30 and stops, otherwise +0x34 and multiplies by 0.57f (0x3f11eb85). Constants: 0x3c8efa35=pi/180.
// NEXT: ours uses `jalr v0`; retail uses t9 (`lw t9,0(a0); lw t9,24(t9); jalr t9`). Try lang=cpp virtual
// dispatch at vtable+0x18, as recorded for jalr t9 in MWCC-NOTES. First draft 44/49 @44 (one call site).
// near miss from lane dsn8 (unreviewed)
