/* HSTMATCH module=EBOOT.BIN start=0x00150f54 end=0x00151074 symbol=FUN_00150f54 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/script_cmd.h"

extern "C" {
extern unsigned char DAT_003402f0;
extern char DAT_002dc49c[];
extern void FUN_0014d044(void *owner);
extern void FUN_0014fd94(char *fmt, int a);
extern void FUN_0022c9f4(int a);
extern void FUN_0022bf94(int a, int b);
extern void FUN_00288624(int a, int b, int c);
extern void FUN_0028797c(int a, int b, int c);
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

/* Script command that creates an item (variant at 0x00150f54). */
extern "C" Item2 *FUN_00150f54(ScriptCmd *cmd, float dist, Owner *owner)
{
  int s1;
  Item2 *item;

  if (DAT_003402f0 != 0) {
    FUN_0014d044(owner);
    FUN_0014fd94(DAT_002dc49c, cmd->arg0);
  }
  s1 = owner->unkc;
  FUN_0022c9f4(cmd->arg0);
  FUN_0022bf94(9, cmd->arg0);
  if (s1 != 0) {
    FUN_00288624(s1, cmd->arg0, 15);
    FUN_0028797c(s1, 25, 15);
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
