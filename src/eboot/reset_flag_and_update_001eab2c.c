/* HSTMATCH module=EBOOT.BIN start=0x001eab2c end=0x001eab5c symbol=FUN_001eab2c */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    char pad[0x43c];
    unsigned char flag43c;
    char pad43d[3];
    int val440;
};

void FUN_001e995c(struct Obj *obj);

void FUN_001eab2c(struct Obj **param_1)
{
    struct Obj *obj = *param_1;
    if (obj->flag43c != 0) {
        obj->flag43c = 0;
        obj->val440 = 0;
        FUN_001e995c(obj);
    }
}
