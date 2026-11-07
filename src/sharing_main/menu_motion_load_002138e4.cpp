/* HSTMATCH module=hstp_trans_main.prx start=0x002138e4 end=0x00213b20 symbol=FUN_002138e4 lang=c++ */
/* Hand-written reconstruction: clone of the source of menu_motion_load_0020507c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; matching idiom: the container-grow call takes an empty Tag by value (move v0,zero; seb a2,v0 in the likely slot) and the 6th parameter of FUN_00094ce4 is unsigned char (orders the owner load before the lbu); names and types await human review. */
extern "C" void FUN_0001c5f4(int *, char *, int);
extern "C" void FUN_00214ea8(void *, int);
extern "C" void FUN_000143b0(char *, char *, ...);
extern "C" int FUN_00000bcc(int);
extern "C" int FUN_0005697c(int);
extern "C" void FUN_00056a58(int, char *, int);
struct Tag {};
extern "C" void FUN_000b75a4(void *, int *, Tag);
extern "C" void FUN_00094ce4(int *, int, char *, char *, int, unsigned char);
extern "C" void FUN_0001c73c(int *);
extern int DAT_003b7ebc;
extern char DAT_00397760[];
extern char *DAT_0039768c[];
extern char DAT_003976d0[];
extern char DAT_003976f4[];
extern char DAT_0039777c[];

struct Obj20507c {
    int *owner;
    char pad0[8];
    int *data;
    unsigned int count;
    unsigned int cap;
    char pad1[0x54];
    unsigned char f6c;
};

extern "C" void FUN_002138e4(struct Obj20507c *p, int param_2)
{
    int flag;
    int i;
    char **namep;
    int handle;
    char bufP[64];
    char bufN[256];
    char bufM[256];
    int local;

    flag = *(int *)((char *)p->owner + 0x50);
    FUN_0001c5f4(&DAT_003b7ebc, DAT_00397760, param_2);
    if (p->cap < 6)
        FUN_00214ea8(&p->data, 6);
    i = 0;
    namep = DAT_0039768c;
    do {
        FUN_000143b0(bufP, *namep, param_2);
        switch (i) {
        case 2:
        case 3:
        case 4:
        case 5:
            FUN_000143b0(bufN, DAT_003976d0, 99, bufP);
            break;
        default:
            FUN_000143b0(bufN, DAT_003976d0, param_2, bufP);
            break;
        }
        handle = FUN_00000bcc(0x10);
        if (handle != 0)
            handle = FUN_0005697c(handle);
        local = handle;
        FUN_00056a58(local, bufN, 0);
        if (p->count < p->cap) {
            handle = p->count + 1;
            p->count = handle;
            p->data[handle - 1] = local;
        } else {
            FUN_000b75a4(&p->data, &local, Tag());
        }
        if (flag != 0) {
            switch (i) {
            case 2:
            case 3:
            case 4:
            case 5:
                FUN_000143b0(bufN, DAT_003976f4, 99, bufP);
                break;
            default:
                FUN_000143b0(bufN, DAT_003976f4, param_2, bufP);
                break;
            }
            FUN_000143b0(bufM, DAT_0039777c, param_2);
            FUN_00094ce4(p->owner, i, bufN, bufM, 0, p->f6c);
        }
        i++;
        namep++;
    } while (i < 6);
    FUN_0001c73c(&DAT_003b7ebc);
}
