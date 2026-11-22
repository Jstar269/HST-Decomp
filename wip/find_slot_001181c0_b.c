/* HSTMATCH module=EBOOT.BIN start=0x001181c0 end=0x00118250 symbol=FUN_001181c0 lang=c99 */
int FUN_001181c0(int param_1)
{
    int i = *(int *)(param_1 + 0x5c);
    int r = -1;
    if (i >= 0) {
        for (;;) {
            int ok = (*(unsigned char *)(param_1 + i * 0x70 + 0xe0) != 0) ^ 1;
            if (!(ok || *(unsigned char *)(param_1 + i * 0x70 + 0xe1) != 0)) {
                break;
            }
            if (i == *(int *)(param_1 + 0x60)) {
                i = -1;
                break;
            }
            i = (i + 8) % 9;
        }
    }
    if (i >= 0) {
        r = *(int *)(i * 0x70 + param_1 + 0xb8);
    }
    return r;
}
// NEAR (34/36 words; 22 differ), lane api-hste2-20261121: same residue as find_slot_001181c0.c (retail `bnez e1,ADV; nop; b done; nop` vs ours `beqz e1,done`; ours also lacks retail's
// `i<0 -> li a3,-1` entry edge). Tried for(;;)/while forms, ok=(e0!=0)^1 locals, `else break`, `||` condition, an inline FindSlot with early returns (36 words, 30 differ).
// near miss from lane api-hste2-20261121 (unreviewed)
