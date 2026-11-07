/* HSTMATCH module=hstp_trans_main.prx start=0x0024481c end=0x00244858 symbol=FUN_0024481c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Tab { int k; int x; };
struct Obj { int id; };
extern Tab DAT_002da1dc[];
extern "C" int FUN_0024481c(Obj *o)
{
  int id = o->id;
  int k;
  if (id >= 0 && id < 26) k = 26; else k = DAT_002da1dc[id].k;
  return k;
}
