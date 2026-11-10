/* HSTMATCH module=EBOOT.BIN start=0x00106bb4 end=0x00106c08 symbol=FUN_00106bb4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct List { int count; int pad; char *data; };
struct Owner { char pad[0x54]; struct List *f54; char pad2; char name[1]; };
char *FUN_00106bb4(struct Owner *self, int idx)
{
  struct List *list = self->f54;
  int count;
  if (list == 0) return self->name;
  count = list->count;
  if (idx >= 0 && count >= 2 && idx + 1 < count) return list->data + idx * 0x80;
  return self->name;
}
