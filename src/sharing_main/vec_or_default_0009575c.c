/* HSTMATCH module=hstp_trans_main.prx start=0x0009575c end=0x000957bc symbol=FUN_0009575c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern float DAT_002cead0[4];
extern char DAT_003e1a50;
struct S { char pad[0x58]; int f58; };
void *FUN_0009575c(struct S *s) {
    if (s->f58) return (char *)s->f58 + 96;
    if (!DAT_003e1a50) { DAT_002cead0[0] = 0; DAT_002cead0[1] = 0; DAT_002cead0[2] = 0; DAT_002cead0[3] = 1.0f; DAT_003e1a50 = 1; }
    return DAT_002cead0;
}
