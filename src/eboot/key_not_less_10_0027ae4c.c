/* HSTMATCH module=EBOOT.BIN start=0x0027ae4c end=0x0027ae64 symbol=FUN_0027ae4c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { char pad[0x10]; int key; };
struct It { int pad; struct Node *node; };
int FUN_0027ae4c(struct It *a, struct Node *b) { return !(a->node->key < b->key); }
