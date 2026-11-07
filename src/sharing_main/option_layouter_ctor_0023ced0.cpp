/* HSTMATCH module=hstp_trans_main.prx start=0x0023ced0 end=0x0023d350 symbol=FUN_0023ced0 lang=c++ */
/* Hand-written reconstruction: clone of the source of option_layouter_ctor_0022cdd0.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (lane ol3 near miss reworked); exact match verified; names and types await human review. Matching idiom: byte-offset table loops written as (unsigned int)self + off + DISP with an (unsigned long) re-spelling after the call, so they keep self+off per pass with strength reduction on. */
enum { PANE_COUNT = 3, ITEM_COUNT = 70, SUB_COUNT = 12, TABLE_COUNT = 15 };

class VObj {
public:
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5(int);
    virtual void m6(float);
};
struct PaneItem {
    void *vt;
    short x, y;
    char pad8[0x18];
    VObj *child;
    int unk24;
};
struct ItemExtra {
    char pad0[0xd8];
    int unkD8;
    char padDC[0x114 - 0xdc];
    unsigned char unk114;
    char pad115[0x12c - 0x115];
    unsigned char unk12c;
};
struct Entry {
    int idx;   /* index into the pane list */
    int id;
    int out;
};
struct OptionLayouter {
    void *vt;
    char pad4[0x54];
    int unk58[18];
    unsigned char unkA0;
    int unkA4[5];
    unsigned char unkB8;
    int panes[PANE_COUNT];
    PaneItem *items[ITEM_COUNT];
    VObj *subs[SUB_COUNT];
    ItemExtra *extra;
    int unk214;
    int pad218;
    int unk21c;
    int unk220;
    int unk224[8];
    char pad244[0x290 - 0x244];
    char a290[0x344 - 0x290];
    char a344[0x404 - 0x344];
    int unk404;
    int unk408;
    int unk40c;
    int unk410;
    int unk414;
};
extern "C" {
extern char DAT_003b2b08[];
extern unsigned char DAT_0039bbf0;
extern int DAT_003f3648[TABLE_COUNT];
extern int DAT_002da0f8[TABLE_COUNT];
extern char DAT_0039bd44[];
extern float DAT_0039b498;
extern int DAT_0039b4b4[PANE_COUNT];
extern Entry DAT_0039b878[ITEM_COUNT];
extern int DAT_0039bbc0[SUB_COUNT];
extern int DAT_003e1a08;
extern int DAT_003f344c;
extern int DAT_003ea914;
extern int DAT_003f469c;
void FUN_0010873c(void *);
void FUN_000007f4(void *, void *, void *, int, int);
void FUN_000cb230();
void FUN_000540d8();
void FUN_00109a44(void *, void *, int);
void FUN_00108e6c(void *, float);
int FUN_00109304(void *, int, int);
PaneItem *FUN_00108124(int, int);
void FUN_00105570(void *, int);
void FUN_00111aa4(ItemExtra *, int, int);
void *FUN_0008f170(int);
void *FUN_001dbee0(int);
void FUN_001090f4(int, int);
void FUN_0023d4f0(OptionLayouter *, int);
void *FUN_00000bcc(unsigned);
int FUN_0016a154(void *);
int FUN_0019c784(void *);
void FUN_002b4168(void *);
void FUN_0001ef34(int, int);
}

/* Constructs the option screen layout: builds its panes and items, fetches their widgets and sets their initial state. */
extern "C" OptionLayouter *FUN_0023ced0(OptionLayouter *self, int reload)
{
    FUN_0010873c(self);
    self->vt = DAT_003b2b08;
    self->unk58[0] = 0;
    self->unk58[1] = 0;
    self->unk58[2] = 0;
    self->unk58[3] = 0;
    self->unk58[4] = 0;
    self->unk58[5] = 0;
    self->unk58[6] = 0;
    self->unk58[7] = 0;
    self->unk58[8] = 0;
    self->unk58[9] = 0;
    self->unk58[10] = 0;
    self->unk58[11] = 0;
    self->unk58[12] = 0;
    self->unk58[13] = 0;
    self->unk58[14] = 0;
    self->unk58[15] = 0;
    self->unk58[16] = 0;
    self->unk58[17] = 0;
    self->unkB8 = 0;
    self->unk21c = 0;
    self->unk220 = 0;
    FUN_000007f4(self->a290, (void *)FUN_000cb230, (void *)FUN_000540d8, 12, 15);
    FUN_000007f4(self->a344, (void *)FUN_000cb230, (void *)FUN_000540d8, 12, 15);
    if (DAT_0039bbf0) {
        for (int i = 0; i < TABLE_COUNT; i++)
            DAT_003f3648[i] = DAT_002da0f8[i];
        DAT_0039bbf0 = 0;
    }
    FUN_00109a44(self, DAT_0039bd44, 0);
    FUN_00108e6c(self, DAT_0039b498);
    self->unk224[0] = 0;
    self->unk224[1] = 0;
    self->unk224[2] = 0;
    self->unk224[3] = 0;
    self->unk224[4] = 0;
    self->unk224[5] = 0;
    self->unk224[6] = 0;
    self->unk224[7] = 0;
    self->unk414 = 0;
    self->unkA4[0] = -1;
    self->unkA4[1] = -1;
    self->unkA4[2] = -1;
    self->unkA4[3] = -1;
    self->unkA4[4] = -1;
    self->unkA0 = 1;
    int pane_index = 0;
    int *pane_kinds = DAT_0039b4b4;
    int pane_off = 0;
    do {
        VObj *pane;
        *(VObj **)((unsigned int)self + pane_off + 188) = pane = (VObj *)FUN_00109304(self, *pane_kinds, 0);
        pane->m5(-2);
        pane_index++;
        pane_kinds++;
        pane_off += 4;
    } while (pane_index < PANE_COUNT);
    int item_index = 0;
    Entry *entry = DAT_0039b878;
    int item_off = 0;
    do {
        PaneItem *item = FUN_00108124(self->panes[entry->idx], entry->id);
        *(PaneItem **)((unsigned int)self + item_off + 200) = item;
        if (item != 0) {
            FUN_00105570(item, -256);
            entry->out = (*(PaneItem **)((unsigned long)self + item_off + 200))->unk24;
        }
        item_index++;
        entry++;
        item_off += 4;
    } while (item_index < ITEM_COUNT);
    int sub_index = 0;
    int *sub_item_ids = DAT_0039bbc0;
    int sub_off = 0;
    do {
        VObj **slot = (VObj **)((unsigned int)self + sub_off + 480);
        PaneItem *item = self->items[*sub_item_ids];
        *slot = item->child;
        (*slot)->m5(-16);
        sub_index++;
        sub_item_ids++;
        sub_off += 4;
    } while (sub_index < SUB_COUNT);
    self->unk404 = self->items[67]->y;
    self->unk408 = self->items[68]->y;
    self->unk40c = self->items[13]->x;
    self->unk410 = self->items[19]->x;
    ItemExtra *extra_obj = (ItemExtra *)self->items[69]->child;
    self->extra = extra_obj;
    extra_obj->unkD8 = 0;
    extra_obj->unk114 = 1;
    FUN_00111aa4(self->extra, self->unk224[0], 2);
    self->extra->unk12c = 0;
    ((VObj *)self->extra)->m6(100.0f + DAT_0039b498);
    FUN_00105570(self->items[69], -8);
    self->unk214 = 0;
    if (DAT_003e1a08 != 0) {
        void *p = FUN_0008f170(DAT_003e1a08);
        self->unk214 = (int)p;
        FUN_00108e6c(p, 100000.0f);
    } else if (DAT_003f344c != 0) {
        void *p = FUN_001dbee0(DAT_003f344c);
        self->unk214 = (int)p;
        FUN_00108e6c(p, 100000.0f);
        FUN_001090f4(self->unk214, -1);
    }
    FUN_0023d4f0(self, reload);
    void *m = FUN_00000bcc(68);
    if (m != 0)
        self->unk21c = FUN_0016a154(m);
    else
        self->unk21c = (int)m;
    if (reload == 0) {
        if (DAT_003ea914 == 0) {
            void *q = FUN_00000bcc(64);
            if (q != 0)
                self->unk220 = FUN_0019c784(q);
            else
                self->unk220 = (int)q;
            FUN_0001ef34(DAT_003e1a08, DAT_003ea914);
        }
        if (DAT_003f469c == 0) {
            void *q = FUN_00000bcc(0xd58);
            if (q != 0)
                FUN_002b4168(q);
            FUN_0001ef34(DAT_003e1a08, DAT_003f469c);
        }
    }
    ((VObj *)self)->m1();
    return self;
}
