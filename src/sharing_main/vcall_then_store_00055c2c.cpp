/* HSTMATCH module=hstp_trans_main.prx start=0x00055c2c end=0x00055c68 symbol=FUN_00055c2c lang=c++ */
/* Clone of src/eboot/vcall_then_store_0005510c.cpp (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
  virtual void m0();
  virtual void m1();
  virtual void m2();
  virtual void m3();
  char pad[0x88];
  int f8c;
};

extern "C" void FUN_00055c2c(Obj *param_1, int param_2)
{
  param_1->m3();
  param_1->f8c = param_2;
}
