/* HSTMATCH module=hstp_trans_main.prx start=0x002447e0 end=0x0024481c symbol=FUN_002447e0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Tab { int k; int x; };
struct Obj { int id; };
extern Tab DAT_002da1e0[];
extern "C" int FUN_002447e0(Obj *o)
{
  int id = o->id;
  int k;
  if (id >= 0 && id < 26) k = 26; else k = DAT_002da1e0[id].k;
  return k;
}
