/* HSTMATCH module=EBOOT.BIN start=0x000e5580 end=0x000e5650 symbol=FUN_000e5580 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern void __ct__10B_0001f06cFv(void *self);
extern char DAT_00307654;

char *FUN_000e5580(char *self)
{
    __ct__10B_0001f06cFv(self);
    *(void **)self = &DAT_00307654;
    char *p = self + 60;
    int n = 192 - (int)(p - self);
    for (int i = 0; i < n; i++) {
        *p++ = 0;
    }
    return self;
}
}
