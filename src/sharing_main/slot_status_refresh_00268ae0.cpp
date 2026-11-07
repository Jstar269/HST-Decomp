/* HSTMATCH module=hstp_trans_main.prx start=0x00268ae0 end=0x00268f18 symbol=FUN_00268ae0 lang=c++ rodata=0x002dad20 */
/* Hand-written reconstruction: clone of the source of slot_status_refresh_00258630.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry {
    unsigned char used;
    char pad0[3];
    char name[0x25 - 4];
    unsigned char locked;
    unsigned char open;
    char pad1[0x2c - 0x27];
    int id;
    char pad2[0x39 - 0x30];
    unsigned char ready;
    char pad3[0x458 - 0x3a];
};

struct Info {
    char pad0;
    unsigned char online;
    char pad1[0x80 - 2];
    int mode;
    char pad2[0x1ba8 - 0x84];
    Entry ent[4];
};

struct Slot {
    unsigned char active;
    unsigned char b1;
    char pad0;
    unsigned char b3;
    signed char name[13];
    char pad1[0x25 - 0x11];
    unsigned char c1d;
    unsigned char c1e;
    unsigned char c1f;
    unsigned char c20;
    char pad2[0x2c - 0x29];
    int a;
    int b;
    float scale;
    unsigned char c34;
    unsigned char c35;
    char pad3[2];
    int c38;
    char pad4[0x458 - 0x40];

    void Reset()
    {
        active = 0;
        b3 = 0;
        b1 = 0;
        c1d = 0;
        c1e = 0;
        c1f = 0;
        c20 = 0;
        a = -1;
        b = -1;
        scale = 5.0f;
        c34 = 10;
        c35 = 0;
        c38 = 0;
        name[0] = -29;
        name[1] = -127;
        name[2] = -65;
        name[3] = -29;
        name[4] = -126;
        name[5] = -109;
        name[6] = -29;
        name[7] = -125;
        name[8] = -122;
        name[9] = -29;
        name[10] = -125;
        name[11] = -117;
        name[12] = 0x50;
    }
};

struct Self {
    char pad0[0x270];
    char sub[0x51c - 0x270];
    unsigned char host;
    char pad1[3];
    int sel3;
    char pad2[4];
    int sel1;
    char pad3[0x640 - 0x52c];
    Info *info;
    unsigned char two;
    char pad4[0x7f8 - 0x645];
    Slot slots[4];
};

extern "C" {
extern int DAT_003e1e04;
int FUN_00014a40(const char *);
void FUN_00266664(void *, int, int, int, int);
}

extern "C" void FUN_00268ae0(Self *self, int finish)
{
    int st = 0;
    for (int i = 0; i < (self->two ? 2 : 4); i++) {
        if (DAT_003e1e04 == 0) {
            if (self->info->mode == 1 && i == 3) {
                st = 6;
            } else {
                st = 3;
                self->slots[i].Reset();
            }
        } else {
            Info *info = self->info;
            if (info->ent[i].locked == 0) {
                if (info->mode == 1 && i == 3)
                    st = 6;
                else
                    st = 3;
            }
            if (info->ent[i].id >= 0 || info->ent[i].used != 0 || (info->online != 0 && FUN_00014a40(info->ent[i].name) != 0)) {
                unsigned char host = self->host;
                if (host != 0)
                    st = 0;
                else
                    st = 1;
                if (self->info->online != 0 && host == 0 && self->info->ent[i].used != 0)
                    st = 2;
            }
            Info *in2 = self->info;
            if (in2->ent[i].locked != 0 && in2->ent[i].open == 0)
                st = 7;
        }
        Info *info = self->info;
        if (info->mode == 1 && i == 3) {
            switch (self->sel1) {
            case 0:
            case 4:
            case 5:
            case 8:
                st = self->sel1;
                break;
            case 1:
            case 2:
                if (info->online != 0)
                    st = 2;
                else
                    st = 1;
                break;
            case 3:
                st = 6;
                break;
            case 7:
            case 9:
                st = 9;
                break;
            }
        }
        if (info->mode == 3 && (unsigned)(i - 2) < 2) {
            switch (self->sel3) {
            case 0:
            case 4:
            case 5:
            case 8:
                st = self->sel3;
                break;
            case 1:
            case 2:
                if (info->online != 0)
                    st = 2;
                else
                    st = 1;
                break;
            case 3:
                st = 6;
                break;
            case 7:
            case 9:
                st = 0;
                break;
            }
        }
        if (st == 3 || st == 7)
            self->slots[i].Reset();
        if (self->info->ent[i].ready != 0)
            st = 8;
        if (finish != 0) {
            switch (st) {
            default:
                st = 5;
                break;
            case 0:
            case 1:
            case 2:
            case 4:
                st = 4;
                break;
            }
        }
        FUN_00266664(self->sub, i, st, self->info->mode, self->two ? 2 : 4);
    }
}
