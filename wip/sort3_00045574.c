/* HSTMATCH module=EBOOT.BIN start=0x00045574 end=0x00045690 symbol=FUN_00045574 lang=c99 */
/* HSTMATCH module=EBOOT.BIN start=0x00045574 end=0x00045690 symbol=FUN_00045574 lang=c99 */
typedef struct Pair {
    unsigned short key;
    unsigned short value;
} Pair;

void FUN_00045574(Pair *param_1, Pair *param_2, Pair *param_3)
{
    unsigned char bVar4 = param_3->key < param_1->key;
    unsigned short uVar3 = param_2->key;

    if (bVar4 || uVar3 < param_3->key) {
        if (bVar4 && uVar3 < param_3->key) {
            Pair volatile t1; t1.key = param_1->key; t1.value = param_1->value;
            *param_1 = *param_2;
            param_2->key = t1.key; param_2->value = t1.value;
        }
        else {
            if (uVar3 < param_1->key) {
                Pair volatile t2; t2.key = param_1->key; t2.value = param_1->value;
                *param_1 = *param_2;
                *param_2 = t2;
            }
            if (bVar4) {
                Pair volatile t3; t3.key = param_1->key; t3.value = param_1->value;
                *param_1 = *param_3;
                *param_3 = t3;
            }
            else {
                Pair volatile t4; t4.key = param_1->key; t4.value = param_1->value;
                *param_2 = *param_3;
                *param_3 = t4;
            }
        }
    }
}
// Ruling: PROGRESS.md VERDICTS (0x00045574); experiment: try/hsttry/sort3_00045574-46612
// improved by lane dsn8 (unreviewed): 68 -> 62 differing words
