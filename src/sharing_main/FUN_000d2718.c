/* HSTMATCH module=hstp_trans_main.prx start=0x000d2718 end=0x000d2828 symbol=FUN_000d2718 lang=c99 */
/* Hand-written reconstruction: clone of the source of FUN_000ceb1c.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern float DAT_003e1aac;
extern float DAT_003e1aa0[];
extern void FUN_00059ca4(void *p, int a);
extern void FUN_0005ff00(void *p, float v);
extern void FUN_00059d68(void *p, int a);
extern void FUN_00055be8(void *p, float *v);
extern void FUN_00059e58(void *p);
extern void FUN_0005fec8(void *p);

void FUN_000d2718(char *self)
{
  float *v;
  float buf[4];
  char *ctl;
  int none = -1;

  if (DAT_003e1aac > 0.0f) v = DAT_003e1aa0;
  else v = 0;
  if (v != 0) {
    if (*(unsigned char *)(self + 0x3c) == 0) {
      ctl = self + 0x40;
      FUN_00059ca4(ctl, none);
      FUN_0005ff00(ctl, 0.0f);
      *(int *)(self + 0x110) = 0;
      FUN_00059d68(ctl, -1);
      *(float *)(self + 0x110) = *(float *)(self + 0x104);
    }
    buf[0] = v[0];
    buf[1] = v[1];
    buf[2] = v[2];
    buf[3] = v[3];
    buf[1] = v[1] - 0.099999994f;
    FUN_00055be8(self + 0x40, buf);
    *(unsigned char *)(self + 0x3c) = 1;
  } else {
    *(unsigned char *)(self + 0x3c) = 0;
  }
  if (*(unsigned char *)(self + 0x3c) != 0) {
    ctl = self + 0x40;
    FUN_00059e58(ctl);
    FUN_0005fec8(ctl);
    *(float *)(self + 0xc0) = *(float *)(self + 0x144);
  }
}
