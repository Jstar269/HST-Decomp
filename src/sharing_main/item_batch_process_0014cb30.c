/* HSTMATCH module=hstp_trans_main.prx start=0x0014cb30 end=0x0014cc34 symbol=FUN_0014cb30 lang=c99 */
/* Hand-written reconstruction: clone of the source of item_batch_process_001414e4.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma opt_strength_reduction off
extern void FUN_000d24e4(void *p);
extern void FUN_0005931c(void *p, void *src);
extern void FUN_00068be4(void *a, void *b);
extern char DAT_003b0830[];
extern char DAT_003b0808[];

void FUN_0014cb30(char *self, void *arg)
{
  char b[112];
  char *p;
  unsigned i;
  int off;
  char *pb;

  i = 0;
  if (*(int *)(self + 0x7a8) == 0) return;
  pb = b;
  for (; i < *(int *)(self + 0x7a8); i++) {
    off = i * 80;
    p = (char *)(*(int *)(self + 0x7a4) + off);
    FUN_000d24e4(pb);
    *(char **)pb = DAT_003b0830;
    FUN_0005931c(pb + 16, p);
    *(int *)(pb + 80) = 0;
    *(int *)(pb + 84) = 0x3f800000;
    *(int *)(pb + 88) = 0x453b8000;
    *(int *)(pb + 92) = 0;
    *(char *)(pb + 96) = 0;
    *(int *)(b + 80) = *(int *)((char *)*(int *)(self + 0x7a4) + off + 0x40);
    *(int *)(b + 84) = 0x3f800000;
    *(int *)(b + 88) = 0x455ac000;
    *(char *)(b + 96) = 0;
    FUN_00068be4(arg, b);
    *(char **)b = DAT_003b0808;
  }
}
