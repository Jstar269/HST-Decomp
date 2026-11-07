/* HSTMATCH module=EBOOT.BIN start=0x0016c030 end=0x0016c040 symbol=FUN_0016c030 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x23]; char sub[1]; };
extern char DAT_0034179c[];
extern void FUN_001b2bb8(void *mgr, void *sub);
void FUN_0016c030(struct Obj *self) { FUN_001b2bb8(DAT_0034179c, self->sub); }
