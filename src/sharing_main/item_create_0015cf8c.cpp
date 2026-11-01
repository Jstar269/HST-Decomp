/* HSTMATCH module=hstp_trans_main.prx start=0x0015cf8c end=0x0015d0ac symbol=FUN_0015cf8c lang=c++ */
/* Clone of src/eboot/item_create_00150f54.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/script_cmd.h"

extern "C" {
extern unsigned char DAT_003e9630;
extern char DAT_00383984[];
extern void FUN_00158b2c(void *owner);
extern void FUN_0015bb68(char *fmt, int a);
extern void FUN_0023c6d4(int a);
extern void FUN_0023ba04(int a, int b);
extern void FUN_00299d58(int a, int b, int c);
extern void FUN_002990b0(int a, int b, int c);
}

struct Owner {
  char pad00[12];
  int unkc;
};

struct Base {
  Base() {}
  virtual ~Base();
};

struct Item2 : Base {
  int unk4;
  Item2() : unk4(17) {}
  virtual int Init(float dist, Owner *owner);
};

/* Script command that creates an item (variant at 0x0015cf8c). */
extern "C" Item2 *FUN_0015cf8c(ScriptCmd *cmd, float dist, Owner *owner)
{
  int s1;
  Item2 *item;

  if (DAT_003e9630 != 0) {
    FUN_00158b2c(owner);
    FUN_0015bb68(DAT_00383984, cmd->arg0);
  }
  s1 = owner->unkc;
  FUN_0023c6d4(cmd->arg0);
  FUN_0023ba04(9, cmd->arg0);
  if (s1 != 0) {
    FUN_00299d58(s1, cmd->arg0, 15);
    FUN_002990b0(s1, 25, 15);
  }
  item = new Item2();
  if (item->Init(dist, owner) != 0) {
    if (item != 0) {
      delete item;
    }
    return 0;
  }
  return item;
}
