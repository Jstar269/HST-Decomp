/* HSTMATCH module=EBOOT.BIN start=0x0023de6c end=0x0023e3c4 symbol=FUN_0023de6c lang=c++ */
/* Hand-written reconstruction: clone of the source of record_page_update_0024e004.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Record page refresh: prints the selected record's date ("%d/%d/%d" or "%d.%d.%d" by DAT_003417a4) into a label,
   fills ten list rows from the id list (alternating sprite frames, text via FUN_001fea10) and marks the selected
   cell of the sprite list. Inline helpers: the bounds-checked record lookup (nested ifs, one shared `return 0`,
   like FUN_00177e58) and an unsigned-index sprite accessor (retail recomputes `j*4` per pass). The sub-objects are
   taken into local pointers (retail keeps their addresses in saved registers); the row index is computed after
   the game getter call (retail leaves that jal slot empty). EBOOT twin: 0x0023de6c. */
struct Widget;
struct ListWidget;
struct Sprite;
extern "C" {
void FUN_0010d4a8(Widget *w);
void FUN_000ffb3c(Sprite *s, int on);
const char *FUN_001fea10(int cat, int sub, int n);
int FUN_000143b0(char *buf, const char *fmt, ...);
void FUN_00218ae4(int *row, int *col, int id);
void FUN_0010b980(void *sel, int idx, int f);
void FUN_00110a78(void *msg, const char *s, int f);
}

struct Widget {
    virtual void v0();
    virtual void Show(int hide);
    char pad4[0xf8 - 4];
    const char *str;   /* 0xf8 */
    char padfc[0x138 - 0xfc];
    int visible;       /* 0x138 */
    void Set(const char *s, int on)
    {
        str = s;
        visible = on;
        FUN_0010d4a8(this);
    }
};

struct SpriteVec {
    Sprite **data;
    int count;
    int Size() const { return count; }
    Sprite *&operator[](unsigned i) { return data[i]; }
};

struct ListWidget {
    virtual void v0();
    virtual void Show(int hide);
    char pad4[0x10 - 4];
    SpriteVec sprites; /* 0x10 */
};

struct Sprite {
    char pad0[0x24];
    int frame;         /* 0x24 */
    int baseFrame;     /* 0x28 */
};

struct Item {
    Sprite *spr;
    char pad4[0x10];
};

struct Label {
    Sprite *spr;
    Widget *text;      /* +4 */
    int f8;
};

struct ListLabel {
    Sprite *spr;
    ListWidget *list;  /* +4 */
    int f8;
};

struct Bar {
    char pad0[0xe0];
    float pos;         /* 0xe0 */
};

struct Page {
    char pad0[0x2c];
    Item items[203];   /* 0x2c */
    Label labels[35];  /* 0x1008 */
    ListLabel cal;     /* 0x11ac */
    void *sel;         /* 0x11b8 */
    Bar *bar;          /* 0x11bc */
    char date[0x100];  /* 0x11c0 */
    char msg[0x490];   /* 0x12c0 */
    void *box;         /* 0x1750 */
};

struct Rec {
    unsigned char kind;
    char pad1[3];
    unsigned short year;   /* 4 */
    unsigned short month;  /* 6 */
    unsigned short day;    /* 8 */
    char pada[0x14 - 0xa];
};

struct Block {
    char pad0[0x1058];
    Rec recs[40];
};

struct IdList {
    int *data;
};

struct Game {
    char pad0[0x1808];
    int f1808;
    int cur;      /* 0x180c */
    int top;      /* 0x1810 */
    int mode;     /* 0x1814 */
    char pad1818[0x1830 - 0x1818];
    int count;    /* 0x1830 */
};

extern "C" {
Game *FUN_00238d58(void);
IdList *FUN_0023a830(Game *g);
Block *FUN_0016bb88(void);
extern int DAT_003417a4;
extern char DAT_002f54f4[];
extern char DAT_002f5500[];
}

static inline Rec *GetRec(int id)
{
    Block *b = FUN_0016bb88();
    if (id >= 0) {
        if (id < 40)
            return &b->recs[id];
    }
    return 0;
}

static inline void FlagAllBut(ListWidget *l, int sel)
{
    for (int j = 0; j < l->sprites.Size(); j++)
        FUN_000ffb3c(l->sprites[j], j != sel);
}

extern "C" void FUN_0023de6c(Page *self, unsigned char flag, int first)
{
    int cur = FUN_00238d58()->cur;
    first = first + FUN_00238d58()->top;
    int f1808 = FUN_00238d58()->f1808;
    IdList *list = FUN_0023a830(FUN_00238d58());
    int id = list->data[cur];
    Rec *rec = GetRec(id);
    if (flag)
        FUN_0010b980(self->sel, f1808, 0);
    self->bar->pos = (float)first;
    Label *hdr = &self->labels[12];
    if (FUN_00238d58()->mode != 0)
        hdr->text->Set(FUN_001fea10(0x13, 8, 2), 1);
    else
        hdr->text->Set(FUN_001fea10(0x13, 8, 0), 1);
    for (int i = 0; i < 10; i++) {
        Game *g = FUN_00238d58();
        int n = first + i;
        if (n <= g->count - 1) {
            int id2 = list->data[n];
            Rec *r = GetRec(id2);
            Label *lb = &self->labels[2 + i];
            Item *it = &self->items[174 + i];
            if (n % 2 == 0)
                it->spr->frame = it->spr->baseFrame;
            else
                it->spr->frame = it->spr->baseFrame + 1;
            if (r->kind == 4 || r->kind == 5)
                lb->text->Set(FUN_001fea10(9, 4, id2), 0);
            else
                lb->text->Set(FUN_001fea10(0x13, 0, 2), 0);
        }
    }
    Label *date = &self->labels[13];
    ListLabel *cal = &self->cal;
    Item *mark = &self->items[195];
    if (flag) {
        if (rec->kind == 4 || rec->kind == 5) {
            switch (DAT_003417a4) {
            case 0:
                FUN_000143b0(self->date, DAT_002f54f4, rec->month, rec->day, rec->year);
                break;
            case 4:
                FUN_000143b0(self->date, DAT_002f5500, rec->day, rec->month, rec->year);
                break;
            default:
                FUN_000143b0(self->date, DAT_002f54f4, rec->day, rec->month, rec->year);
                break;
            }
            date->text->Show(0);
            date->text->Set(self->date, 1);
            cal->list->Show(0);
            int row, col;
            FUN_00218ae4(&row, &col, id);
            FlagAllBut(cal->list, row);
            Sprite *s = cal->list->sprites[row];
            s->frame = s->baseFrame + col;
            FUN_000ffb3c(mark->spr, 1);
        } else {
            date->text->Show(0);
            date->text->Set(FUN_001fea10(0x13, 0, 4), 1);
            FUN_000ffb3c(cal->spr, 1);
            FUN_000ffb3c(mark->spr, 0);
        }
    } else {
        date->text->Show(1);
        FUN_000ffb3c(cal->spr, 1);
        FUN_000ffb3c(mark->spr, 1);
    }
    if (flag && self->box) {
        if (rec->kind == 4 || rec->kind == 5) {
            if (id == 6) {
                const char *a = FUN_001fea10(0x12, 0x11, id);
                const char *b = FUN_001fea10(0xe, 0, 0x10);
                FUN_000143b0(self->msg, a, b);
                FUN_00110a78(self->box, self->msg, 0);
            } else {
                FUN_00110a78(self->box, FUN_001fea10(0x12, 0x11, id), 0);
            }
        } else {
            FUN_00110a78(self->box, FUN_001fea10(0x12, 0x12, 0), 0);
        }
    }
}
