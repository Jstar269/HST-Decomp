/* HSTMATCH module=hstp_trans_main.prx start=0x0020d55c end=0x0020d594 symbol=FUN_0020d55c */
/* Clone of src/eboot/step_wrapped_index_001fed8c.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_0020d55c(int current, int step, int wrap, int max)
{
    int res = current + step;
    if (res < 0) {
        if (wrap) {
            res = max - 1;
        } else {
            res = current;
        }
        goto end;
    }
    if (res < max) goto end;
    if (wrap) {
        res = 0;
    } else {
        res = current;
    }
end:
    return res;
}
