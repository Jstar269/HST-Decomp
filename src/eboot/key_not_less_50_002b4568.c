/* HSTMATCH module=EBOOT.BIN start=0x002b4568 end=0x002b4580 symbol=FUN_002b4568 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { char pad[0x50]; int key; };
struct It { int pad; struct Node *node; };
int FUN_002b4568(struct It *a, struct Node *b) { return !(a->node->key < b->key); }
