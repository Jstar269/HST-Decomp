/* HSTMATCH module=hstp_trans_main.prx start=0x000f7c18 end=0x000f7c24 symbol=FUN_000f7c18 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x84]; unsigned char flag; };
extern void FUN_00108f54(struct Obj *self, int value);
void FUN_000f7c18(struct Obj *self) { self->flag = 1; FUN_00108f54(self, 1); }
