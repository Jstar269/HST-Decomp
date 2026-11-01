/* HSTMATCH module=EBOOT.BIN start=0x0006b0f4 end=0x0006b154 symbol=FUN_0006b0f4 lang=c99 */
void FUN_0006b0f4(int *param_1, int param_2, int *param_3)
{
  int key = *param_3;
  int *parent = (int *)(param_2 + 4);
  int *node = *(int **)((int *)(param_2 + 4));
  if (node != 0) {
    int k = node[3];
    int *prev = parent;
    parent = node;
    while (1) {
      if (k < key) {
        node = (int *)parent[1];
        parent = prev;
      }
      else {
        node = (int *)*parent;
      }
      if (node == 0) {
        break;
      }
      k = node[3];
      prev = parent;
      parent = node;
    }
  }
  if (parent == (int *)(param_2 + 4) || key < parent[3]) {
    *param_1 = (int)(int *)(param_2 + 4);
  }
  else {
    *param_1 = (int)parent;
  }
}
// Ruling: PROGRESS.md VERDICTS (0x0006b0f4); experiment: try/hsttry/tree_find_0006b0f4-17220
// improved by lane dsn8 (unreviewed): 29 -> 28 differing words
