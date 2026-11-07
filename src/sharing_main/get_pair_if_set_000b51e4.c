/* HSTMATCH module=hstp_trans_main.prx start=0x000b51e4 end=0x000b520c symbol=FUN_000b51e4 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x480]; unsigned char valid; char pad1[3]; int value; char pad2[0x4ce - 0x488]; unsigned char extra; };
int FUN_000b51e4(struct Obj *self, int *value, unsigned char *extra)
{
    if (self->valid) { *value = self->value; *extra = self->extra; return 1; }
    return 0;
}
