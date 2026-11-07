/* HSTMATCH module=EBOOT.BIN start=0x0023471c end=0x00234758 symbol=FUN_0023471c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Tab { int k; int x; };
struct Obj { int id; };
extern Tab DAT_002c9c1c[];
extern "C" int FUN_0023471c(Obj *o)
{
  int id = o->id;
  int k;
  if (id >= 0 && id < 26) k = 26; else k = DAT_002c9c1c[id].k;
  return k;
}
