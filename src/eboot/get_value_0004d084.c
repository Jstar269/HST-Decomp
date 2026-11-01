/* HSTMATCH module=EBOOT.BIN start=0x0004d084 end=0x0004d140 symbol=FUN_0004d084 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review.
   Reads a two-step value from FUN_002b77a0: mode 1 first, then mode 2; a negative first answer is an
   error and returns 0, a negative second answer is clamped to 0 before mode 0 commits the first value.
   The two 0 values are the middle (64-bit) argument of FUN_002b77a0, so a1 is never written.
   Matching idioms: the three-globals test is a flat && chain that falls through to the work; the second
   call's result is tested inside the if condition, which makes MWCC compare v0 and move it into the
   result register in the branch delay; the function keeps working value in s2 and the mode-1 answer in
   s1 across the calls, hence the 16-byte frame. */
extern int DAT_00311158;
extern unsigned char DAT_0030fbfd;
extern char DAT_0030ab9c[];
extern int FUN_0001c1fc(void *arg);
extern int FUN_002b77a0(int handle, long long arg, int mode);

/* Read the current value, retrying once through mode 2; 0 on any negative answer. */
int FUN_0004d084(int param_1)
{
    int result;
    int r;

    if (DAT_00311158 == 0 && DAT_0030fbfd != 0) {
        return FUN_0001c1fc(DAT_0030ab9c);
    }
    result = 0;
    r = FUN_002b77a0(param_1, 0, 1);
    if (r >= 0) {
        if ((result = FUN_002b77a0(param_1, 0, 2)) < 0) {
            result = 0;
        }
        FUN_002b77a0(param_1, r, 0);
    }
    return result;
}
