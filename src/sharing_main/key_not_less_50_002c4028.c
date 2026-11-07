/* HSTMATCH module=hstp_trans_main.prx start=0x002c4028 end=0x002c4040 symbol=FUN_002c4028 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { char pad[0x50]; int key; };
struct It { int pad; struct Node *node; };
int FUN_002c4028(struct It *a, struct Node *b) { return !(a->node->key < b->key); }
