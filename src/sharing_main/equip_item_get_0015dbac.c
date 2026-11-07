/* HSTMATCH module=hstp_trans_main.prx start=0x0015dbac end=0x0015dc74 symbol=FUN_0015dbac lang=c99 data=0x00383a6c */
/* Hand-written reconstruction: clone of the source of equip_item_get_00151acc.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Data: its string literals (1 strings, 0x002dc584-0x002dc5a5) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
#include "hst/script_cmd.h"

enum { EQUIP_PART_COUNT = 3 };

/* One equipment part; only its item word is read. */
struct EquipPart { char pad[0x1b4]; int item; };
/* The global player object seen through its part pointers. */
struct EquipPlayer { char pad[0x54]; struct EquipPart *part[EQUIP_PART_COUNT]; };

extern unsigned char DAT_003e9630; /* script trace flag */
extern struct EquipPlayer *DAT_003f3450;
void FUN_00158b2c(ScriptCtx *);
void FUN_0015bb68(char *, int, int);
int *FUN_00157e10(int, int); /* register slot of a script register bank */

/* Script command "GetEquipItem": store the item of the player's equipment part (-1 for an unknown part) into the
   command's register. */
int FUN_0015dbac(ScriptCmd *cmd, ScriptCtx *ctx)
{
  int parts;
  int item;
  int regs;

  if (DAT_003e9630 != 0) {
    FUN_00158b2c(ctx);
    FUN_0015bb68("GetEquipItem [reg:%d, parts:%d]\n", cmd->arg0, cmd->arg1);
  }
  regs = ctx->regs;
  parts = cmd->arg1;
  item = -1;
  switch (parts) {
  case 0:
    item = DAT_003f3450->part[0]->item;
    break;
  case 1:
    item = DAT_003f3450->part[1]->item;
    break;
  case 2:
    item = DAT_003f3450->part[2]->item;
    break;
  }
  *FUN_00157e10(regs, cmd->arg0) = item;
  return 0;
}
