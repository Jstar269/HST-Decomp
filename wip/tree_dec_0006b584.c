/* HSTMATCH module=EBOOT.BIN start=0x0006b584 end=0x0006b5f8 symbol=FUN_0006b584 lang=c99 */

void FUN_0006b584(int *param_1)
{
  int *node = (int *)*param_1;
  int *child = (int *)*node;
  if (child == 0) {
    int *parent = (int *)(node[2] & 0xfffffffe);
    if (node == (int *)*parent) {
      int *np;
      *param_1 = (int)parent;
      while (np = (int *)(parent[2] & 0xfffffffe), node == (int *)*np) {
        *param_1 = (int)np;
        parent = np;
      }
      *param_1 = (int)np;
    }
    else {
      *param_1 = (int)parent;
    }
  }
  else {
    int *n = (int *)child[1];
    if (n == 0) {
      *param_1 = (int)child;
    }
    else {
      do {
        child = n;
        n = (int *)child[1];
      } while (n != 0);
      *param_1 = (int)child;
    }
  }
}
// NEAR 36/29 words, 36 differ: our reconstruction tests the left child with `bnezl`
// while retail tests it with `beqzl` and keeps the whole parent-climb block out of
// line; the `& 0xfffffffe` parent+colour dance additionally needs retail's exact
// register pair (a1/a2). Ruled out: the Ghidra-shaped reconstruction only.
// near miss from lane dsn8 (unreviewed)
