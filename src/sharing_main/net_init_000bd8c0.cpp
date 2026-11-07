/* HSTMATCH module=hstp_trans_main.prx start=0x000bd8c0 end=0x000bdad0 symbol=FUN_000bd8c0 lang=c++ data=0x0037b62c */
/* Hand-written reconstruction: clone of the source of net_init_000ba57c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   The eight-word config copy is written field by field: a struct assignment of the same 32 bytes makes MWCC
   copy through $f0..$f3 with lwc1/swc1 instead of lw/sw (see docs/MWCC-NOTES.md). Data: its string literals (8 strings, 0x002d4d64-0x002d4e68) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
#include "hst/adhoc_net.h"

extern "C" void FUN_000bdd24(void *, void *);
extern "C" int FUN_000bd6f0(char *, ...);
extern "C" int FUN_002c7b20(int, int, int, int, int);
extern "C" int FUN_002c7be8(void);
extern "C" int FUN_002c7b08(int, int, const void *);
extern "C" int FUN_002c7ae8(void *, int);
extern "C" void *FUN_00000a90(int);
extern "C" void FUN_000bdd78(void *, int);
extern "C" void FUN_000bf57c(void);

extern int DAT_003e1e08;
extern int DAT_0037b378;

/* Brings the net stack up (net, ad-hoc, ad-hoc control) and allocates the working buffers; returns 1 on success. */
extern "C" int FUN_000bd8c0(AdhocNet *self, AdhocConfig *cfg)
{
  int ret;
  int i;

  if (cfg != 0) {
    self->config.bufSizeA = cfg->bufSizeA;
    self->config.bufSizeB = cfg->bufSizeB;
    self->config.unk08 = cfg->unk08;
    self->config.unk0c = cfg->unk0c;
    self->config.poolSize = cfg->poolSize;
    self->config.unk14 = cfg->unk14;
    self->config.product = cfg->product;
    self->config.groupName = cfg->groupName;
  } else {
    FUN_000bdd24(self, &self->config);
  }

  ret = FUN_002c7b20(self->config.poolSize, 0x29, 0x2000, 0x29, 0x2000);
  if (ret < 0) {
    FUN_000bd6f0("sceNetInit() failed(0x%X)\n", ret);
    return 0;
  }
  FUN_000bd6f0("sceNetInit() success\n");
  ret = FUN_002c7be8();
  if (ret < 0) {
    FUN_000bd6f0("sceNetAdhocInit() failed(0x%X)\n", ret);
    return 0;
  }
  FUN_000bd6f0("sceNetAdhocInit() success\n");
  ret = FUN_002c7b08(0x2000, 0x30, self->config.product);
  if (ret < 0) {
    FUN_000bd6f0("sceNetAdhocctlInit() failed(0x%X)\n", ret);
    return 0;
  }
  FUN_000bd6f0("sceNetAdhocctlInit() success\n");
  ret = FUN_002c7ae8((void *)FUN_000bf57c, 0);
  if (ret < 0) {
    FUN_000bd6f0("sceNetAdhocctlAddHandler() failed(0x%X)\n", ret);
    return 0;
  }
  self->unk24 = FUN_000bd6f0("sceNetAdhocctlAddHandler() success\n");
  self->unk190 = 0;
  self->unk194 = 0;
  self->unk274 = 0;
  self->unk168 = 0;
  DAT_003e1e08 = 0;
  DAT_0037b378 = -1;

  for (i = 0; i < ADHOC_SLOT_COUNT; i++) {
    self->slotFlag[i] = 0;
    self->slotObj[i] = 0;
  }

  self->bufA = FUN_00000a90(self->config.bufSizeA);
  self->bufB = FUN_00000a90(self->config.bufSizeB);
  self->bufC = FUN_00000a90(self->config.bufSizeA);
  self->unk284 = 0;
  FUN_000bdd78(self, 0);
  self->unk278 = 1;
  return 1;
}
