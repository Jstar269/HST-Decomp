/* HSTMATCH module=hstp_trans_main.prx start=0x0006d6e4 end=0x0006d71c symbol=FUN_0006d6e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { struct Node *left; struct Node *right; int pad; int key; };
struct Node *FUN_0006d6e4(void *tree, const int *key, struct Node *x, struct Node *y)
{
  while (x) { if (!(x->key < *key)) { y = x; x = x->left; } else x = x->right; }
  return y;
}
