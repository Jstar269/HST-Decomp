/* HSTMATCH module=EBOOT.BIN start=0x001fed8c end=0x001fedc4 symbol=FUN_001fed8c */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

int FUN_001fed8c(int current, int step, int wrap, int max)
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
