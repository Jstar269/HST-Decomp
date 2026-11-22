/* HSTMATCH module=EBOOT.BIN start=0x001018d0 end=0x0010192c symbol=FUN_001018d0 lang=c++ helpers=FUN_001006b0@0x001006b0-0x001006dc */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern char DAT_002d8ab4[];
extern "C" static void FUN_001006b0(...) { }

struct Cast {
  virtual ~Cast();
  virtual void v1();
  virtual void v2();
  virtual void v3();
  virtual void v4();
  virtual void v5();
  virtual void v6();
  virtual void v7();
  virtual void v8();
  virtual void v9();
  virtual void v10();
  virtual void v11();
  virtual void v12();
  virtual void v13(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k);
  int x;
  int y;
};

extern "C" void FUN_001018d0(Cast *p, int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k)
{
  if (p->y == 0) {
    FUN_001006b0(DAT_002d8ab4);
  }
  p->v13(a, b, c, d, e, f, g, h, i, j, k);
}
