/* HSTMATCH module=hstp_trans_main.prx start=0x002b3c70 end=0x002b3d2c symbol=FUN_002b3c70 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_002aff58();
extern const char DAT_003a97cc[], DAT_003a97f8[], DAT_003a9828[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002aff58(DAT_003a97cc, DAT_003a97f8, DAT_003a9828, line); for (;;) {} } } while (0)
struct Obj { char pad[432]; int i432; char pad2[4]; int i440; int i444; int i448; };
extern int FUN_0023bc78(struct Obj *);
extern int FUN_0023c5e0(int);
extern void FUN_002b1820(struct Obj *, int);
extern int DAT_002dc11c[];
void FUN_002b3c70(struct Obj *self)
{
    int idx = FUN_0023bc78(self);
    HST_ASSERT(idx >= 0 && (unsigned)idx < 10, 1828);
    if (FUN_0023c5e0(idx) == 0) {
        int v = DAT_002dc11c[idx];
        if (self->i432 != 5) {
            self->i448 = self->i440;
            self->i440 = v;
            self->i444 = 0;
            FUN_002b1820(self, 1);
        }
    }
}
