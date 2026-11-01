/* HSTMATCH module=hstp_trans_main.prx start=0x00003eb4 end=0x00003ed4 symbol=FUN_00003eb4 lang=c99 */
/* Clone of src/eboot/vec3_copy_00003eb4.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
typedef struct Vec3f {
  float x;
  float y;
  float z;
} Vec3f;

Vec3f *FUN_00003eb4(Vec3f *param_1, const Vec3f *param_2)
{
  *param_1 = *param_2;
  return param_1;
}
