/* HSTMATCH module=hstp_trans_main.prx start=0x000ea194 end=0x000ea264 symbol=FUN_000ea194 lang=c++ */
/* Hand-written reconstruction: clone of the source of ctor_000e5580.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern "C" {
extern void __ct__10B_0001f06cFv(void *self);
extern char DAT_003b0920;

char *FUN_000ea194(char *self)
{
    __ct__10B_0001f06cFv(self);
    *(void **)self = &DAT_003b0920;
    char *p = self + 60;
    int n = 192 - (int)(p - self);
    for (int i = 0; i < n; i++) {
        *p++ = 0;
    }
    return self;
}
}
