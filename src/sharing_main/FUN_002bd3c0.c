/* HSTMATCH module=hstp_trans_main.prx start=0x002bd3c0 end=0x002bd49c symbol=FUN_002bd3c0 lang=c99 data=0x003abdb0 */
/* Hand-written reconstruction: clone of the source of FUN_002adb98.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag s256b) corrected by hand: readability pass (names, structs, constants); exact match verified; names and types await human review. */
#include "hst/types.h"

/* 0x80-byte record: an id block, a tick value and a short name ending in 'P'. */
typedef struct DeviceRecord {
  u32 unk00;                 /* 0x00 */
  u32 psid[5];               /* 0x04 filled by the OpenPSID query */
  u32 tick[2];               /* 0x18 current tick */
  s8 name[0x60];             /* 0x20 bytes written one by one below */
} DeviceRecord;

extern u32 * FUN_000114c0();
extern void FUN_002bd49c();
extern void FUN_002bd5c8();
extern int FUN_002c7aa0();
extern int FUN_002c7c68();

/* Constructor: clear the record, read the OpenPSID and the tick (printing errors), then store the name bytes. */
DeviceRecord * FUN_002bd3c0(DeviceRecord *self)
{
  int status;
  
  FUN_000114c0(self,0,0x80);
  self->unk00 = 0;
  status = FUN_002c7c68(self->psid);
  if (status == 0) {
    FUN_002bd5c8(self->psid);
  }
  else {
    FUN_002bd49c("ERROR: sceOpenPSIDGetOpenPSID(): 0x%08x\n",status);
  }
  status = FUN_002c7aa0(self->tick);
  if (status != 0) {
    FUN_002bd49c("ERROR: sceRtcGetCurrentTick(): 0x%08x\n",status);
  }
  self->name[0] = 0xe3;
  self->name[1] = 0x81;
  self->name[2] = 0xbf;
  self->name[3] = 0xe3;
  self->name[4] = 0x82;
  self->name[5] = 0x93;
  self->name[6] = 0xe3;
  self->name[7] = 0x83;
  self->name[8] = 0x86;
  self->name[9] = 0xe3;
  self->name[10] = 0x83;
  self->name[11] = 0x8b;
  self->name[12] = 0x50;
  return self;
}
