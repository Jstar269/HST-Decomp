/* HSTMATCH module=hstp_trans_main.prx start=0x001a5dd4 end=0x001a5e8c symbol=FUN_001a5dd4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the result test is a one-case switch, which gives retail's beq/nop/b layout. */
extern void FUN_001a5b08();
extern const char DAT_0038904c[], DAT_00389078[], DAT_00389080[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_001a5b08(DAT_0038904c, DAT_00389078, DAT_00389080, line); for (;;) {} } } while (0)
extern int FUN_002c7a70(void);
extern int FUN_002c79d0(void);
extern void FUN_0001ed28(void *, int);
extern void FUN_00000a00(void *);
extern char DAT_003b2460[];
extern unsigned char DAT_003eaab1;
struct Obj { void *vt; };
struct Obj *FUN_001a5dd4(struct Obj *self, short flag)
{
    if (self != 0) {
        self->vt = DAT_003b2460;
        switch (FUN_002c7a70()) {
        case 3:
            HST_ASSERT(FUN_002c79d0() == 0, 184);
            break;
        }
        DAT_003eaab1 = 0;
        FUN_0001ed28(self, 0);
        if (flag > 0) FUN_00000a00(self);
    }
    return self;
}
