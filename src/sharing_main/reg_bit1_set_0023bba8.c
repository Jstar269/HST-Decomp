/* HSTMATCH module=hstp_trans_main.prx start=0x0023bba8 end=0x0023bc10 symbol=FUN_0023bba8 lang=c99 */
/* Hand-written reconstruction: clone of the source of reg_bit1_set_0022c0f4.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern unsigned char *DAT_003ea8e8;  /* address of the global system block */
struct Regs { unsigned char pad[0xd504]; unsigned short ctrl; };

/* Sets or clears bit 1 of the 16-bit control word, then stores `value` at the word before it. */
void FUN_0023bba8(int enable,int value)
{
  if (enable != 0) {
    *(unsigned short *)(DAT_003ea8e8 + 0xd504) = ((struct Regs *)DAT_003ea8e8)->ctrl | 2;
  }
  else {
    ((struct Regs *)DAT_003ea8e8)->ctrl = *(unsigned short *)(DAT_003ea8e8 + 0xd504) & 0xfffd;
  }
  *(int *)(DAT_003ea8e8 + 0xd4e0) = value;
}
/* Sibling of 0x0022c07c: same trick (one side a struct member, the other the flattened cast, so the
 * 0xd504 address is computed twice as `lui 0x1; addu` + `-11004` displacement) plus the trailing
 * `*(int *)(DAT + 0xd4e0) = value`, whose single `addu` already matched.
 */
