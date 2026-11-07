/* HSTMATCH module=EBOOT.BIN start=0x002346e0 end=0x0023471c symbol=FUN_002346e0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Tab { int k; int x; };
struct Obj { int id; };
extern Tab DAT_002c9c20[];
extern "C" int FUN_002346e0(Obj *o)
{
  int id = o->id;
  int k;
  if (id >= 0 && id < 26) k = 26; else k = DAT_002c9c20[id].k;
  return k;
}
