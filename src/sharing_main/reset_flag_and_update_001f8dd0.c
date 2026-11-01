/* HSTMATCH module=hstp_trans_main.prx start=0x001f8dd0 end=0x001f8e00 symbol=FUN_001f8dd0 */
/* Clone of src/eboot/reset_flag_and_update_001eab2c.c (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Obj {
    char pad[0x43c];
    unsigned char flag43c;
    char pad43d[3];
    int val440;
};

void FUN_001f7c00(struct Obj *obj);

void FUN_001f8dd0(struct Obj **param_1)
{
    struct Obj *obj = *param_1;
    if (obj->flag43c != 0) {
        obj->flag43c = 0;
        obj->val440 = 0;
        FUN_001f7c00(obj);
    }
}
