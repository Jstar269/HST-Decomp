/* HSTMATCH module=hstp_trans_main.prx start=0x002ad2dc end=0x002ad3b4 symbol=FUN_002ad2dc lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: second argument of the first call is a symbol address (array), not an int constant */
extern const char DAT_002fffb0[], DAT_003b7ebc[], DAT_003a8e20[], DAT_003a8e34[], DAT_003a8e54[], DAT_003a8eac[];
extern void FUN_0001c66c(const char *s, const char *v);
extern void FUN_0001c73c(const char *s);
extern int FUN_0004cd8c(const char *s, int *p, int n, int f);
extern int FUN_002a9808(const char *s, ...);
extern int FUN_002c74c0(int a, int b, int c, int d);
extern int FUN_002c7490(int a, int b, int c, int *d, int e);
extern int DAT_003f466c;
void FUN_002ad2dc(void)
{
    int n;
    int m;
    int r;
    FUN_0001c66c(DAT_003b7ebc, DAT_002fffb0);
    r = FUN_0004cd8c(DAT_003a8e20, &n, 64, 1);
    if (r == 0 || n <= 0)
        r = FUN_002a9808(DAT_003a8e34, DAT_003a8e20);
    r = FUN_002c74c0(n, r, 0, 0);
    DAT_003f466c = r;
    if (r < 0)
        FUN_002a9808(DAT_003a8e54, r);
    r = FUN_002c7490(DAT_003f466c, 0, 0, &m, 0);
    if (r < 0)
        FUN_002a9808(DAT_003a8eac, r);
    FUN_0001c73c(DAT_003b7ebc);
}
