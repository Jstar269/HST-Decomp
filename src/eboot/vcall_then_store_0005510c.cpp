/* HSTMATCH module=EBOOT.BIN start=0x0005510c end=0x00055148 symbol=FUN_0005510c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
  virtual void m0();
  virtual void m1();
  virtual void m2();
  virtual void m3();
  char pad[0x88];
  int f8c;
};

extern "C" void FUN_0005510c(Obj *param_1, int param_2)
{
  param_1->m3();
  param_1->f8c = param_2;
}
