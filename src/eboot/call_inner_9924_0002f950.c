/* HSTMATCH module=EBOOT.BIN start=0x0002f950 end=0x0002f958 symbol=FUN_0002f950 lang=c99 */
/* Clone of src/sharing_main/call_inner_9924_0002d600.c (hstclone.py tag c085220): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[8]; void *inner; };
extern void FUN_0002cff8(void *p);
void FUN_0002f950(struct Obj *self) { FUN_0002cff8(self->inner); }
