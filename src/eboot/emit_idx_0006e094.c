/* HSTMATCH module=EBOOT.BIN start=0x0006e094 end=0x0006e110 symbol=FUN_0006e094 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
extern int *DAT_00331c2c;
extern float DAT_00331c34;
extern void FUN_0006e110(int *);
extern void FUN_0006e28c(int *, float);

void FUN_0006e094(int *obj, int idx, float unused1, float unused2, float param_3)
{
  float f;

  if (obj[1] != 0) {
    obj[0] = *(int *)(*DAT_00331c2c + idx * 4);
    FUN_0006e110(obj);
    f = param_3;
    if (param_3 < 0.0f) {
      f = DAT_00331c34;
    }
    FUN_0006e28c(obj, f);
  }
}
