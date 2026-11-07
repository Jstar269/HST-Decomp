/* HSTMATCH module=hstp_trans_main.prx start=0x00157be4 end=0x00157c78 symbol=FUN_00157be4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the 3-argument FUN_0015827c gets the list pointer as its second argument; the element word is read as *(int *)((char *)data + (idx << 3) + 4) so the index shift comes first. */
extern void FUN_00157280();
extern const char DAT_003834cc[], DAT_0038355c[], DAT_0038350c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00157280(DAT_003834cc, DAT_0038355c, DAT_0038350c, line); for (;;) {} } } while (0)
struct Ent { int a; int b; };
struct List { struct Ent *data; unsigned count; };
struct Obj { char pad[8]; int i8; char pad2[12]; char sub[24]; };
extern void FUN_0015827c(void *, void *, int);
extern void FUN_0015b528(void *);
extern void FUN_001583fc(void *, void *);
extern void FUN_00158308(void *, int);
int FUN_00157be4(struct Obj *self, struct List *l, unsigned idx)
{
    char tmp[36];
    HST_ASSERT(idx < l->count, 317);
    FUN_0015827c(tmp, l, *(int *)((char *)l->data + (idx << 3) + 4));
    self->i8 = 0;
    FUN_0015b528(self->sub);
    FUN_001583fc(self, tmp);
    FUN_00158308(tmp, -1);
    return 0;
}
