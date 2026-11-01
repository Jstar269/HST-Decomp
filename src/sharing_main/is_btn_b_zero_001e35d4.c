/* HSTMATCH module=hstp_trans_main.prx start=0x001e35d4 end=0x001e35f4 symbol=FUN_001e35d4 */
/* Clone of src/eboot/is_btn_b_zero_001d57d0.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Btn {
    signed char a : 2;
    signed char b : 2;
    signed char c : 2;
    signed char d : 2;
};

struct Pad {
    char pad[0x8b];
    struct Btn k8b;
};

extern struct Pad *DAT_003751d4;

int FUN_001e35d4(void) {
    return (DAT_003751d4->k8b.b != 0) ^ 1;
}
