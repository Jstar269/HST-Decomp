/* HSTMATCH module=EBOOT.BIN start=0x00288af0 end=0x00288b2c symbol=FUN_00288af0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
  virtual int m0();
  virtual int m1();
  virtual int m2();
  virtual int m3();
  virtual int m4();
  virtual int m5();
  virtual int m6();
  virtual int m7();
  virtual int m8();
  virtual int m9();
  virtual int m10();
  virtual int m11();
};

extern "C" Obj *FUN_00288af0(Obj *param_1)
{
  if (param_1->m11() == 4) {
    return param_1;
  }
  return 0;
}
