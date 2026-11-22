/* HSTMATCH module=EBOOT.BIN start=0x002a51ec end=0x002a5370 symbol=FUN_002a51ec lang=c99 */
/* near miss api-hstm-20261121: 93/97 words, 19 differ. */

extern int DAT_00300a8c;
extern char DAT_00300a90;
extern void *DAT_00310fc4;

extern void *FUN_00011090(char *dest, const char *src, int n);
extern void FUN_000149a8(void *dst, const void *src);
extern int FUN_00014a40(const char *str);
extern void FUN_0004344c(void *ctx, const char *str, int *p4, int *p8, void *p0);

void FUN_002a51ec(char *param_1, void *param_2, void *param_3)
{
    int local_4;
    int local_8;
    char buf[128];
    int local_24;
    int total_len;
    int count;
    int offset;

    local_4 = 0;
    local_8 = 0;
    FUN_0004344c(DAT_00310fc4, param_1, &local_4, &local_8, 0);
    total_len = FUN_00014a40(param_1);
    count = 0;
    FUN_000149a8(param_2, &DAT_00300a90);
    FUN_000149a8(param_3, &DAT_00300a90);

    offset = 0;
    if (total_len > 0) {
        do {
            int iVar4 = 1;
            char c = param_1[offset];
            local_24 = 0;
            do {
                if (((int)c & (1 << (7 - local_24))) == 0) {
                    int *piVar3;
                    if (local_24 < DAT_00300a8c) {
                        piVar3 = &DAT_00300a8c;
                    } else {
                        piVar3 = &local_24;
                    }
                    iVar4 = *piVar3;
                    break;
                }
                local_24++;
            } while (local_24 < 8);

            offset += iVar4;
            FUN_00011090(buf, param_1, offset);
            buf[offset] = 0;
            FUN_0004344c(DAT_00310fc4, buf, &local_4, &local_8, 0);
            count++;
            if (count > 11) {
                break;
            }
        } while (offset < total_len);

        FUN_000149a8(param_2, buf);
        FUN_000149a8(param_3, param_1 + offset);
    }
}

// Fixed vs the older wip: count declared before offset (offset s1, count s2); no walking char pointer (param_1[offset] read once per
// outer pass into `char c` before the inner loop, giving retail's lb a1 / li a2,1 order); the post-loop copies sit inside
// `if (total_len > 0)` (retail's blez goes straight to the epilogue); offset = 0 set just before that test.
// Left (4 words short): retail's latch is `slti at,s2,12; beqzl at,EXIT; slt v1,s1,s0; bnez v1,T; nop; EXIT:..; b EPI; T: bnezl v1,top`
// (v1 tested twice, the back-edge trampoline placed after the exit code); ours folds it to `slt v0; bnezl v0,top`. Latch spellings tried:
// do/while(count<12&&offset<len), if(count>11)break + while(offset<len), ++count form, for(;;) with two breaks, continue form: all 19.
// improved by lane api-hstm-20261121 (unreviewed): 68 -> 19 differing words
