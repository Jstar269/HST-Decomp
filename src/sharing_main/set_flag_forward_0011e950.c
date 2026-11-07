/* HSTMATCH module=hstp_trans_main.prx start=0x0011e950 end=0x0011e968 symbol=FUN_0011e950 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Sub { char pad[0x90a]; unsigned char flag; };
struct Obj { char pad[0x54]; struct Sub *sub; };
extern void FUN_0011e968(struct Obj *self, int a, int b, int c, int d);
void FUN_0011e950(struct Obj *self, int a, int b, int d) { self->sub->flag = 1; FUN_0011e968(self, a, b, 0, d); }
