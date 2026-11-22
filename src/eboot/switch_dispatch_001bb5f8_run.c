/* HSTMATCH module=EBOOT.BIN start=0x001bb5f8 end=0x001bb6b0 symbol=FUN_001bb5f8 lang=c99 rodata=0x002c6260 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Run of FUN_001bb5f8 (maps x to 0..3, tail-calls the next function) and FUN_001bb640 (code lookup); the retail range 0x1bb5f8-0x1bb6b0 holds both. Matching idiom: `case 1: v = 0;` (retail's case 1 body is a dead block and its test goes straight to the join). */
extern int DAT_003417a4;
extern void FUN_002b7e70(int, int);
void FUN_001bb640(int kind);

void FUN_001bb5f8(int x)
{
    int v = 0;

    switch (x) {
    case 1: v = 0; break;
    case 2: v = 2; break;
    case 3: v = 3; break;
    default: break;
    }
    FUN_001bb640(v);
}

void FUN_001bb640(int kind)
{
    int code;
    DAT_003417a4 = kind;
    code = 1;
    switch (kind) {
    case 0:
    case 1:
        code = 1;
        break;
    case 2:
        code = 2;
        break;
    case 3:
        code = 3;
        break;
    case 4:
        code = 4;
        break;
    case 5:
        code = 5;
        break;
    case 6:
    case 7:
    case 8:
        break;
    case 9:
        code = 9;
        break;
    case 10:
        code = 10;
        break;
    }
    FUN_002b7e70(code, 1);
}
