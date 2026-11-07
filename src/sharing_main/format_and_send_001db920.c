/* HSTMATCH module=hstp_trans_main.prx start=0x001db920 end=0x001db960 symbol=FUN_001db920 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x15c]; char *target; };
extern void FUN_001d9bf4(struct Obj *self, char *buf, int a, int b);
extern void FUN_002389a0(void *dst, const char *buf);
void FUN_001db920(struct Obj *self, int a, int b) { char buf[256]; FUN_001d9bf4(self, buf, a, b); FUN_002389a0(self->target + 52, buf); }
