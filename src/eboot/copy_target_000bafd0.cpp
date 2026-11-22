/* HSTMATCH module=EBOOT.BIN start=0x000bafd0 end=0x000bb034 symbol=FUN_000bafd0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Matching idiom: the callee FUN_002b7dc8 takes four arguments and the incoming a2/a3 are forwarded untouched (Ghidra shows two),
   which keeps a2/a3 live so the six-byte memberwise copy loop takes t0/t1. */
struct Target { unsigned char id[6]; Target() {} };

struct Obj {
  char pad[0x16c];
  void *f16c;
};

extern "C" {
extern char DAT_002d51e0[];
int FUN_002b7dc8(void *a, void *b, int c, int d);
void FUN_000ba3ac(char *msg, int code);
}

extern "C" bool FUN_000bafd0(Obj *self, unsigned char *param_2, int p3, int p4)
{
  Target t;
  t = *(Target *)param_2;
  int r = FUN_002b7dc8(self->f16c, &t, p3, p4);
  if (r < 0) {
    FUN_000ba3ac(DAT_002d51e0, r);
    return 0;
  }
  return 1;
}
