/* HSTMATCH module=hstp_trans_main.prx start=0x00190fd0 end=0x00191248 symbol=FUN_00190fd0 lang=c++ */
/* Clone of src/eboot/item_select_update_00184884.cpp (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

typedef int (*Func)(int);
struct Mgr {
    char pad[0x43];
    unsigned char f43;
    char pad2[0x548 - 0x44];
    Func func548;
    int pad3;
    Func func550;
};
extern "C" Mgr *FUN_00177e98();

inline int helper2(int x) {
    Mgr *mgr = FUN_00177e98();
    if (x != 0x7fff && x != 0x7ffe) return mgr->func548(x);
    return 0;
}

struct Item {
    char pad[0x24];
    int f24;
    int f28;
};

struct ArrElem {
    Item *item;
    char pad[0x14 - 4];
};

struct Sub774 {
    void *f0;
    int pad[3];
    int f10;
};

struct Sub794 {
    char pad[68];
    Item *item;
};

struct Self {
    char pad_00[4];
    int f4;
    char pad_08[0x58 - 8];
    void *f58;
    char pad_5c[0x6c - 0x5c];
    void *f6c;
    char pad_70[0x760 - 0x70];
    void *f760;
    char pad_764[0x774 - 0x764];
    Sub774 s774;
    char pad_784[0x794 - 0x774 - sizeof(Sub774)];
    Sub794 s794;
    char pad_798[0x828 - 0x794 - sizeof(Sub794)];
    void *f828;
    char pad_82c[0x1180 - 0x82c];
    void *f1180;
};

extern "C" {
int FUN_00179388(int);
void FUN_00111b58(void *, int, int);
void FUN_00105470(void *, int);
void FUN_001055a4(void *, int);
void FUN_0018fe90(Self *, int, int, int);
void FUN_00190fd0(Self *self, unsigned int p2, int param_3);
}

void FUN_00190fd0(Self *self, unsigned int p2, int param_3) {
    int param_2 = (int)p2;
    int i;
    int sel;
    Mgr *m = FUN_00177e98();
    if (param_2 != 0x7fff && param_2 != 0x7ffe) sel = m->func550(param_2);
    else sel = 0;

    if (param_3 != 0) {
        FUN_00111b58(self->f1180, sel, 0);
    } else {
        FUN_00111b58(self->f1180, 4, 0);
    }

    Sub774 *s1 = &self->s774;
    void **s0 = &self->f760;
    void **s2 = &self->f828;
    Mgr *mgr2 = FUN_00177e98();

    if (mgr2->f43 != 0) {
        FUN_00105470(s1->f0, 1);
        FUN_00105470(*s0, 1);
        FUN_00105470(*s2, 1);
    } else {
        FUN_00105470(s1->f0, 0);
        FUN_00105470(*s0, 0);
        FUN_00105470(*s2, 0);
        if (param_3 != 0) {
            FUN_001055a4(s1->f0, (int)0x80808080);
        } else {
            FUN_001055a4(s1->f0, s1->f10);
        }
    }

    int s1_lim = FUN_00179388(param_2);
    i = 0;
    int s2_off = 0;
    do {
        Sub794 *sub = (Sub794 *)((char *)self + (s2_off + 1940));
        Item **p = &sub->item;
        if (i < s1_lim) {
            if (i == sel) {
                FUN_00105470(*p, 0);
                (*p)->f24 = (*p)->f28;
            } else {
                FUN_00105470(*p, 1);
            }
        } else {
            FUN_00105470(*p, 0);
            (*p)->f24 = (*p)->f28 + 1;
        }
        i++;
        s2_off += 20;
    } while (i < 4);

    void **v0 = &self->f58;
    void **s0_ptr = &self->f6c;
    if (param_3 != 0) {
        FUN_00105470(*v0, 0);
        FUN_00105470(*s0_ptr, 0);
    } else {
        FUN_00105470(*v0, 1);
        FUN_00105470(*s0_ptr, 1);
    }

    if (self->f4 != 0) {
        FUN_0018fe90(self, helper2(param_2), 1, param_3);
    }
}
