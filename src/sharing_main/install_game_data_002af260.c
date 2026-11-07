/* HSTMATCH module=hstp_trans_main.prx start=0x002af260 end=0x002af39c symbol=FUN_002af260 lang=c99 */
/* Hand-written reconstruction: clone of the source of install_game_data_0029ff1c.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. Matching idiom: pass the saved status as the variadic error argument. */
#include "hst/types.h"

/* Install job as seen by the worker thread (the thread argument points at a pointer to it). */
typedef struct InstallJobView {
  char pad00[8];
  u32 unk8;         /* 0x08 */
  char pad0c[0x20 - 0xc];
  int result;       /* 0x20: negative on failure, -1 once a failure was reported */
} InstallJobView;

extern char DAT_002dbe60[];
extern char DAT_002dbeb0[];
extern char DAT_002dbebc[];
extern char DAT_002dbec8[];
extern char DAT_002dbf14[];
extern char DAT_002dbf5c[];
extern char DAT_002dbf68[];
extern char DAT_002dbfb8[];
extern char DAT_002dc000[];
extern char DAT_002dc00c[];
extern u32 FUN_00103ce0();
extern int FUN_002a5ff0();
extern int FUN_002a6288();
extern int FUN_002a6634();
extern void FUN_002ad710(const char *, ...);
extern int FUN_002c7750();

/* Worker thread body: runs the install steps, logs the outcome and exits. */
u32 FUN_002af260(u32 arg_size,InstallJobView **argp)
{
  int ret;
  int status;
  InstallJobView *job;
  
  job = *argp;
  ret = FUN_00103ce0(DAT_002dc00c,DAT_002dc000,
                       job->unk8,0x218000);
  if (ret < 0) {
    FUN_002ad710(DAT_002dbfb8,job->result);
    job->result = -1;
  }
  else {
    status = FUN_002a5ff0();
    job->result = status;
    if (status < 0) {
      FUN_002ad710(DAT_002dbf68,status);
      goto report_result;
    }
    ret = FUN_002a6634(DAT_002dbf5c,job->unk8,ret);
    job->result = ret;
    if (ret < 0) {
      FUN_002ad710(DAT_002dbf14,ret);
    }
    else {
      ret = FUN_002a6288();
      job->result = ret;
      if (ret < 0) {
        FUN_002ad710(DAT_002dbec8,ret);
      }
    }
  }
report_result:
  ret = job->result;
  if (ret >= 0) {
    FUN_002ad710(DAT_002dbebc);
  } else {
    FUN_002ad710(DAT_002dbeb0,ret);
  }
  ret = FUN_002c7750(0);
  if (ret < 0) {
    FUN_002ad710(DAT_002dbe60,ret);
  }
  return 0;
}
