/* HSTMATCH module=EBOOT.BIN start=0x00217294 end=0x002172f4 symbol=FUN_00217294 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Node { char pad[0x20]; int value; };
extern struct Node *FUN_001026b8(void *table, const unsigned char *key);
void FUN_00217294(int *out, void **tables, const unsigned char *key)
{
  int found = 1;
  struct Node *n = FUN_001026b8(tables[key[0]], key + 1);
  if (n == 0) found = 0;
  if (found) *out = n->value;
}
