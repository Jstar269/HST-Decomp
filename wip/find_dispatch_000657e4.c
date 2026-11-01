/* HSTMATCH module=EBOOT.BIN start=0x000657e4 end=0x00065888 symbol=FUN_000657e4 lang=c99 */
extern void FUN_00065c60(void *out, void *arg, void *key);

float FUN_000657e4(int param_1, int param_2, int param_3)
{
  int local_c[2];
  int key;
  int idx;
  int obj;
  int inst;

  key = param_2;
  FUN_00065c60(local_c, (void *)(param_1 + 0x10), &key);
  idx = -1;
  if (local_c[0] != param_1 + 0x14) idx = *(int *)(local_c[0] + 0x10);
  if (idx < 0 || param_3 < 0) return 0.0f;
  obj = *(int *)(*(int *)(param_1 + 4) + idx * 0xc);
  inst = *(int *)(obj + param_3 * 4);
  return (*(float (**)(int))(*(int *)(*(int *)inst + 0x10)))(inst);
}
// NEAR 41/41 words, 24 differ, first +0x14. Decoded from retail: map lookup FUN_00065c60(local, p+0x10,
// &key); idx = -1 then node!=end ? node[0x10] : -1; two negative guards return 0.0f; virtual call through
// *(*(inst)+0x10) with inst in a0. NEXT IDEAS: (1) write it as C++ (lang=cpp) virtual dispatch to get the
// `lw t9; lw t9,16(t9); jalr t9` form (ours uses v0); (2) retail's frame has local_c at sp+20 and key at
// sp+28 whereas ours puts key at sp+20 -- try declaring/using key after local_c's array, or a struct.
// Split-guard variant measured 26 (worse); OR-guard base stays best at 24.
// near miss from lane dsn8 (unreviewed)
