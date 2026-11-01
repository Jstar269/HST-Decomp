/* HSTMATCH module=EBOOT.BIN start=0x001d57d0 end=0x001d57f0 symbol=FUN_001d57d0 */
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

extern struct Pad *DAT_002cfc8c;

int FUN_001d57d0(void) {
    return (DAT_002cfc8c->k8b.b != 0) ^ 1;
}
