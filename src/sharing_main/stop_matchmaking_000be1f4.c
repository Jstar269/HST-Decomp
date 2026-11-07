/* HSTMATCH module=hstp_trans_main.prx start=0x000be1f4 end=0x000be288 symbol=FUN_000be1f4 lang=c99 */
/* Hand-written reconstruction: clone of the source of stop_matchmaking_000bae04.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag from inputs/generated/FUN_000be1f4.c) corrected by hand: FUN_002c7b38 takes the dropped handle argument (loaded into a0 before the test), the handle is a struct member tested as `h > -1` inside one outer if (gives slti/bnezl, not bgez); readability pass; exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/adhoc_net.h"

extern char DAT_0037ba38[];
extern char DAT_0037ba68[];
extern char DAT_0037ba9c[];
extern void FUN_000bd6f0();
extern int FUN_002c7b38(int);
extern int FUN_002c7b80();

/* Stops and deletes the matching session; returns 1 when it was closed. */
u32 FUN_000be1f4(AdhocNet *net)
{
  int result;
  if (net->matchingId > -1) {
    result = FUN_002c7b38(net->matchingId);
    if (result < 0) {
      FUN_000bd6f0(DAT_0037ba38, result);
      return 0;
    }
    result = FUN_002c7b80(net->matchingId);
    if (result < 0) {
      FUN_000bd6f0(DAT_0037ba68, result);
      return 0;
    }
    FUN_000bd6f0(DAT_0037ba9c);
    net->matchingId = -1;
    return 1;
  }
  return 0;
}
