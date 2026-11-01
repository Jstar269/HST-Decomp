/* HSTMATCH module=hstp_trans_main.prx start=0x002435e0 end=0x00243618 symbol=FUN_002435e0 lang=c99 */
/* Clone of src/eboot/trigger_event_sequence_002334e0.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"

extern int DAT_0039bf2c;
extern const int DAT_0039bf4c;

extern void FUN_0024312c(void *arg, int a1);
extern void FUN_0023c434(int a0, int a1);

void FUN_002435e0(void *arg)
{
    DAT_0039bf2c = DAT_0039bf4c;
    FUN_0024312c(arg, 2);
    FUN_0023c434(7, 1);
}
