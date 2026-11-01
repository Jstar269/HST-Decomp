/* HSTMATCH module=hstp_trans_main.prx start=0x0004d3fc end=0x0004d4b8 symbol=FUN_0004d3fc lang=c99 */
/* Clone of src/eboot/get_value_0004d084.c (hstclone.py tag dsn8b): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   Reads a two-step value from FUN_002c7578: mode 1 first, then mode 2; a negative first answer is an
   error and returns 0, a negative second answer is clamped to 0 before mode 0 commits the first value.
   The two 0 values are the middle (64-bit) argument of FUN_002c7578, so a1 is never written.
   Matching idioms: the three-globals test is a flat && chain that falls through to the work; the second
   call's result is tested inside the if condition, which makes MWCC compare v0 and move it into the
   result register in the branch delay; the function keeps working value in s2 and the mode-1 answer in
   s1 across the calls, hence the 16-byte frame. */
extern int DAT_003ba498;
extern unsigned char DAT_003b8ec5;
extern char DAT_003b3e6c[];
extern int FUN_0001bd2c(void *arg);
extern int FUN_002c7578(int handle, long long arg, int mode);

/* Read the current value, retrying once through mode 2; 0 on any negative answer. */
int FUN_0004d3fc(int param_1)
{
    int result;
    int r;

    if (DAT_003ba498 == 0 && DAT_003b8ec5 != 0) {
        return FUN_0001bd2c(DAT_003b3e6c);
    }
    result = 0;
    r = FUN_002c7578(param_1, 0, 1);
    if (r >= 0) {
        if ((result = FUN_002c7578(param_1, 0, 2)) < 0) {
            result = 0;
        }
        FUN_002c7578(param_1, r, 0);
    }
    return result;
}
