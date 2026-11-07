/* HSTMATCH module=hstp_trans_main.prx start=0x0028c130 end=0x0028c148 symbol=FUN_0028c130 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { char pad[0x10]; int key; };
struct It { int pad; struct Node *node; };
int FUN_0028c130(struct It *a, struct Node *b) { return !(a->node->key < b->key); }
