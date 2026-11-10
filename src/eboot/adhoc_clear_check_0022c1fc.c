/* HSTMATCH module=EBOOT.BIN start=0x0022c1fc end=0x0022c308 symbol=FUN_0022c1fc lang=c99 */
/* Hand-written reconstruction from the disassembly (lane fm5 near miss wip/adhoc_clear_check_0022c1fc.c, finished by lane pn4: the log call takes the cell coordinates, `FUN_0022c308(fmt, u, v)`, which Ghidra dropped); exact match verified; names and types await human review. */
typedef unsigned int undefined4;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned char u8;

struct Ent { char pad[0x20]; unsigned short f20; unsigned short f22; char pad2[0x1e]; u8 f42; };
struct Own { char pad[0x36]; short f36; short f38; short f3a; short f3c; };

extern int FUN_00193118(void);
extern int FUN_001930a8(void);
extern int FUN_00192fd0(int);
extern void FUN_0022c308(char *, ...);
extern u8 *DAT_003415a8;
extern char DAT_002f2b78[];

undefined4 FUN_0022c1fc(int param_1)
{
  struct Own *own;
  struct Ent *e;
  undefined4 r;
  int i;
  unsigned int u;
  unsigned int v;
  u8 *row;

  r = 1;
  if (param_1 != 0) {
    own = (struct Own *)FUN_00193118();
  } else {
    own = 0;
  }
  for (i = FUN_001930a8() - 1; 0 <= i; i--) {
    e = (struct Ent *)FUN_00192fd0(i);
    if (e->f42 == 0) {
      continue;
    }
    if (own != 0 &&
        ((own->f36 == e->f20 && own->f38 == e->f22) ||
         (own->f3a == e->f20 && own->f3c == e->f22))) {
      continue;
    }
    u = e->f20;
    v = e->f22;
    row = (u8 *)DAT_003415a8 + u * 0x14;
    if (row[v + 0x3d00] < 4) {
      FUN_0022c308(DAT_002f2b78, u, v);
      r = 0;
      break;
    }
  }
  return r;
}
