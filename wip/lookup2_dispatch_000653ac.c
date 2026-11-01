/* HSTMATCH module=EBOOT.BIN start=0x000653ac end=0x00065480 symbol=FUN_000653ac lang=c99 */
extern void FUN_00065c60(void *out, void *arg, void *key);

int FUN_000653ac(int param_1, int param_2, int param_3)
{
  int local_18;
  int local_14[2];
  int local_c[2];
  int key;
  int i1;
  int i2;
  int obj;
  int inst;

  key = param_2;
  FUN_00065c60(local_14, (void *)(param_1 + 0x10), &key);
  i1 = -1;
  if (local_14[0] != param_1 + 0x14) i1 = *(int *)(local_14[0] + 0x10);

  local_c[0] = param_3;
  FUN_00065c60(&local_18, (void *)(param_1 + 0x20), local_c);
  i2 = -1;
  if (local_18 != param_1 + 0x24) i2 = *(int *)(local_18 + 0x10);

  if (i1 < 0 || i2 < 0) return 0;
  obj = *(int *)(*(int *)(param_1 + 4) + i1 * 0xc);
  inst = *(int *)(obj + i2 * 4);
  return (*(int (**)(int))(*(int *)inst + 0xc))(inst);
}
// NEAR 52/53 words, 39 differ, first +0x18. Same shape as 0x000657e4: two map lookups through
// FUN_00065c60 (p+0x10 by param_2, p+0x20 by param_3), each index defaulting to -1 against p+0x14/p+0x24,
// then a virtual call at vtable+0xc returning int. Sibling residual is identical in kind: retail uses
// beql, t9 (lw t9,0(a0); lw t9,12(t9); jalr t9) and a different stack-slot order (key1 at sp+44 vs 36).
// NEXT: lang=cpp virtual dispatch for the t9 form, and declare key/local arrays in pseudocode order.
// Declaration-order variant measured the same 52/53 @39; base stays best.
// near miss from lane dsn8 (unreviewed)
