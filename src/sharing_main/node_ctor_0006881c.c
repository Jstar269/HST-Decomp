/* HSTMATCH module=hstp_trans_main.prx start=0x0006881c end=0x00068850 symbol=FUN_0006881c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Link { int pad; int id; };
struct Node { void *vtbl; int owner; int id; float weight; unsigned char done; };
extern char DAT_003b045c[];
struct Node *FUN_0006881c(struct Node *n, int owner, struct Link *link, float weight)
{
  n->vtbl = DAT_003b045c; n->owner = owner; n->id = link ? link->id : 0; n->weight = weight; n->done = 0; return n;
}
