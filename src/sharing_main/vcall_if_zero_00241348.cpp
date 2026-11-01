/* HSTMATCH module=hstp_trans_main.prx start=0x00241348 end=0x00241380 symbol=FUN_00241348 lang=c++ */
/* Clone of src/eboot/vcall_if_zero_00231248.cpp (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Store a new value and, when it is zero, ask the object's virtual slot +0xc to react.  The store
   happens on both paths (retail keeps it in the branch delay slot).
   Matching idiom: the conditional call must be a single-case `switch (param_2) { case 0: ... }`,
   NOT `if (!param_2)`.  The if form makes MWCC emit `bnez` with the call inlined in the
   fall-through (12 words); the switch form makes it emit retail's `beqz a1,<body>` with the body
   out of line, `b` over it and the `lw ra` epilogue load in the delay slot (14 words).
*/

struct VC {
  virtual void m0();
  virtual void m1();
};

struct Obj {
  VC *vp;
  int f4;
};

extern "C" void FUN_00241348(Obj *param_1, int param_2)
{
  param_1->f4 = param_2;
  switch (param_2) {
  case 0:
    param_1->vp->m1();
    break;
  }
}
