/* HSTMATCH module=hstp_trans_main.prx start=0x0001ec44 end=0x0001ecfc symbol=FUN_0001ec44 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_0001ecfc();
extern const char DAT_0037521c[], DAT_00375248[], DAT_0037525c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_0001ecfc(DAT_0037521c, DAT_00375248, DAT_0037525c, line); for (;;) {} } } while (0)
extern void FUN_0001f870(void *);
extern char DAT_003b022c[];
extern int DAT_003b915c;
struct Obj {
    void *vt; int magic; char base[4]; char sent[8];
    void *p20; void *p24; int i28;
    unsigned char b32, b33, p34, p35, b36, b37, p38, p39;
    int i40; unsigned char b44, b45, p46, p47;
    int i48; unsigned char b52, b53, p54, p55;
    int i56;
};
struct Obj *FUN_0001ec44(struct Obj *self)
{
    self->vt = DAT_003b022c;
    FUN_0001f870((char *)self + 8);
    HST_ASSERT(DAT_003b915c == 0, 25);
    self->magic = 0x1234567;
    self->p20 = self->sent;
    self->p24 = self->sent;
    self->i28 = 0;
    self->b32 = 1;
    self->b33 = 1;
    self->b36 = 1;
    self->b37 = 1;
    self->i40 = 0;
    self->b44 = 1;
    self->b45 = 1;
    self->i48 = 0;
    self->b52 = 1;
    self->b53 = 1;
    self->i56 = 0;
    return self;
}
