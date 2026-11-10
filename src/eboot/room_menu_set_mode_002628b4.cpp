/* HSTMATCH module=EBOOT.BIN start=0x002628b4 end=0x00262c14 symbol=FUN_002628b4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Menu {
    virtual void v1();
    virtual void SetActive(int);
    virtual void v2();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void Refresh();
    char pad04[0xdc - 4];
    int cursor;
    char pade0[0xe8 - 0xe0];
    unsigned char fe8;
};

struct Slot {
    int ids[4];
    char pad10[0x458 - 0x10];
};

struct Room {
    char pad[0x80];
    int f80;
    char pad84[0x4f8 - 0x84];
    int f4f8;
    char pad4fc[0x940 - 0x4fc];
    Slot slots[8];
};

struct Self {
    char pad[0x3e];
    char name[0x178 - 0x3e];
    unsigned char ready;
    char pad179[0x198 - 0x179];
    int id;
};

struct Rec {
    char pad[0x24];
    char *msg24;
    char *msg28;
};

struct Count {
    char pad[8];
    int n;
};

struct Lim {
    char pad[0xc0];
    Rec *rec;
    int n;
};

struct Query {
    int f0;
    int found;
    Count *count;
    Lim *lim;
};

struct Screen {
    char pad[0x58];
    Menu *menu;
    char pad5c[0x520 - 0x5c];
    int kinds[4];
    char pad530[0x640 - 0x530];
    Room *room;
    char pad644[0x1854 - 0x644];
    int mode;
    int count;
};

extern "C" {
extern char DAT_0034179c[];
extern void *DAT_003415ac;
extern Self *DAT_00338ac4;
extern char DAT_002c0e20[];
extern char DAT_002c0e1c[];
extern char DAT_002c0e18[];
void FUN_001b1c44(void *, int);
Query *FUN_0018c9f4(int);
void FUN_00114168(void *, int, char *, int, int);
void FUN_00103628(void *);
int FUN_000bbba4(Self *, char *);
void FUN_0010b8cc(Menu *, int, int);
void FUN_0010b6d4(Menu *, int, int);
}

static inline bool Other(int x, int id)
{
    return !(x == id);
}

extern "C" void FUN_002628b4(Screen *self, int mode)
{
    self->mode = mode;
    self->count = 0;
    switch (self->mode) {
    case 0: {
        FUN_00103628(DAT_003415ac);
        bool solo = true;
        if (DAT_00338ac4 != 0 && DAT_00338ac4->ready == 0)
            solo = false;
        if (!solo) {
            int n = 4;
            if (self->room->f80 == 1)
                n = 3;
            for (int i = 0; i < n; i++) {
                Room *room = self->room;
                bool hit = false;
                int a = FUN_000bbba4(DAT_00338ac4, DAT_00338ac4->name);
                bool taken = false;
                int b = FUN_000bbba4(DAT_00338ac4, DAT_00338ac4->name);
                if (Other(room->slots[b].ids[i], DAT_00338ac4->id)) {
                    if (room->slots[a].ids[i] >= 0)
                        taken = true;
                }
                bool free = !taken;
                if (free && self->kinds[i] == 3)
                    hit = true;
                if (hit) {
                    FUN_0010b8cc(self->menu, i, 2);
                    break;
                }
            }
        }
        self->menu->SetActive(0);
        self->menu->Refresh();
        self->menu->fe8 = 0;
        if (self->room->f80 == 1)
            FUN_0010b6d4(self->menu, 0, 2);
        break;
    }
    case 1: {
        self->room->f4f8 = self->menu->cursor;
        self->menu->SetActive(1);
        Query *q = FUN_0018c9f4(17);
        char *msg;
        if (q->found == 0)
            msg = DAT_002c0e20;
        else if (q->count->n < 17)
            msg = DAT_002c0e1c;
        else if (q->lim->n < 10)
            msg = DAT_002c0e18;
        else
            msg = q->lim->rec->msg24;
        FUN_00114168(DAT_003415ac, 0, msg, 0, 0);
        break;
    }
    case 2: {
        FUN_001b1c44(DAT_0034179c, 5);
        self->room->f4f8 = -1;
        self->menu->SetActive(1);
        Query *q = FUN_0018c9f4(17);
        char *msg;
        if (q->found == 0)
            msg = DAT_002c0e20;
        else if (q->count->n < 17)
            msg = DAT_002c0e1c;
        else if (q->lim->n < 11)
            msg = DAT_002c0e18;
        else
            msg = q->lim->rec->msg28;
        FUN_00114168(DAT_003415ac, 2, msg, 0, 0);
        break;
    }
    }
}
