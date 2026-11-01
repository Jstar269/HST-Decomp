/* HSTMATCH module=EBOOT.BIN start=0x000643e8 end=0x0006445c symbol=FUN_000643e8 lang=c99 */

void FUN_000643e8(int *param_1)
{
  int *node = (int *)*param_1;
  int *child = (int *)node[1];
  if (child == 0) {
    int *parent = (int *)(node[2] & 0xfffffffe);
    if (node == (int *)*parent) {
      *param_1 = (int)parent;
    }
    else {
      int *np;
      *param_1 = (int)parent;
      while (np = (int *)(parent[2] & 0xfffffffe), node != (int *)*np) {
        node = parent;
        parent = np;
        *param_1 = (int)np;
      }
    }
  }
  else {
    int *n = (int *)*child;
    if (n == 0) {
      *param_1 = (int)child;
    }
    else {
      do {
        child = n;
        n = (int *)child[0];
      } while (n != 0);
      *param_1 = (int)child;
    }
  }
}
// NEAR 38/29 words, 38 differ: the mirror of 0x0006b584 (right child, offset 4); retail
// branches `beqzl` on the child and puts the climb out of line where ours inverts.
// Ruled out: the Ghidra-shaped reconstruction only.
// near miss from lane dsn8 (unreviewed)
