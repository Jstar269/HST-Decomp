/* HSTMATCH module=hstp_trans_main.prx start=0x0015cda4 end=0x0015cf08 symbol=FUN_0015cda4 lang=c++ */
/* Hand-written reconstruction: clone of the source of item_create_00150d6c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/script_cmd.h"

#define ITEM_ID_MAX 0x11    /* item ids at or above this are rejected */
#define ITEM_ID_SPECIAL 6   /* item id that also triggers the extra call with 10 */

extern "C" {
extern int DAT_003f344c;
extern unsigned char DAT_003e9630; /* script trace flag */
extern char DAT_00383928[];
extern char DAT_00383978[];
extern char DAT_00383964[];
extern void FUN_00000bcc(int size);
extern void FUN_00158b2c(void *owner);
extern void FUN_0015bb68(char *fmt, int a, int b, char *banner);
extern void FUN_001d5014(int ctx, int a, int b, int c, int d, int e);
extern void FUN_0023ba04(int a, int b);
}

struct Owner;

struct Base {
  Base() {}
  virtual ~Base();
};

struct Item : Base {
  Item() {}
  virtual int Init(float dist, Owner *owner);
};

/* Script command that creates an item: arg0 is the item id, arg1 its option, unk10 selects the alternate path. */
extern "C" Item *FUN_0015cda4(ScriptCmd *cmd, float dist, Owner *owner)
{
  Item *item;
  char *banner;

  if (DAT_003e9630 != 0) {
    FUN_00158b2c(owner);
    banner = (cmd->unk10 != 0) ? DAT_00383978 : DAT_00383928;
    FUN_0015bb68(DAT_00383964, cmd->arg0, cmd->arg1, banner);
  }
  if (cmd->arg0 < 0 || cmd->arg0 >= ITEM_ID_MAX) {
    return 0;
  }
  {
    if (cmd->unk10 != 0) {
      FUN_0023ba04(cmd->arg0, cmd->arg1);
    }
    else {
      FUN_001d5014(DAT_003f344c, cmd->arg0, cmd->arg1, -1, 0, 0);
    }
    if (cmd->arg0 == ITEM_ID_SPECIAL) {
      FUN_0023ba04(10, cmd->arg1);
    }
    item = new Item();
    if (item->Init(dist, owner) != 0) {
      if (item != 0) {
        delete item;
      }
      return 0;
    }
  }
  return item;
}
