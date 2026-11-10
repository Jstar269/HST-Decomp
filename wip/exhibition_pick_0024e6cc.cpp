/* HSTMATCH module=EBOOT.BIN start=0x0024e6cc end=0x0024f714 symbol=FUN_0024e6cc lang=c++ data=0x002fbfbc helpers=FUN_0024e6a0@0x0024e6a0-0x0024e6cc */
/* Hand-written reconstruction from the disassembly; NEAR miss (1042/1042 words, 25 differ); names and types await human review. */

/* 108-byte exhibition record (copied whole into the global current-match record). */
struct ExhRec {
    ExhRec();
    char name[16];
    unsigned short unk10;
    char unk12[16];
    short unk22;
    short unk24;
    unsigned char unk26;
    unsigned char unk27;
    unsigned char unk28;
    unsigned char unk29;
    unsigned char unk2a;
    unsigned char unk2b;
    signed char unk2c;
    signed char unk2d;
    unsigned char unk2e;
    signed char unk2f;
    unsigned char unk30;
    unsigned char unk31;
    signed char unk32;
    unsigned char unk33;
    unsigned char unk34;
    short pc_id;
    short pc_sub;
    short partner_id;
    short partner_sub;
    char pc_name[16];
    char partner_name[16];
    struct { unsigned char a, b, c; } unk5e;
    struct { short a, b, c; } unk62;
    short unk68;
    short unk6a;
};

/* 156-byte character record. */
struct NpcRec {
    NpcRec();
    short unk00[6];
    unsigned char unk0c;
    unsigned char unk0d;
    unsigned char unk0e;
    char name[16];
    unsigned short id;
    unsigned short sub;
    unsigned char unk24;
    char unk25[6];
    unsigned char unk2b;
    unsigned char unk2c;
    char unk2d[16];
    short unk3e;
    short unk40;
    unsigned char unk42;
    unsigned char unk43;
    float unk44;
    float unk48;
    float unk4c;
    short unk50;
    short unk52;
    char unk54[16];
    unsigned char unk64;
    unsigned char unk65;
    unsigned char unk66;
    unsigned char unk67;
    char unk68[16];
    short unk78;
    short racket;
    unsigned char hand0;
    unsigned char unk7d[12];
    struct { short a, b, c; } unk8a;
    short unk90[5];
};

struct Kind { unsigned char kind; };

extern "C" {
extern unsigned DAT_00310fc8;
extern char *DAT_003415a8;
extern int DAT_002fbef4;
extern unsigned char DAT_002fbef8;
extern unsigned char DAT_002fbef9;
extern unsigned char DAT_002fbefa;
extern int DAT_002fbefc;
extern int DAT_0034a780;
extern int DAT_0034a784;
extern int DAT_0034a78c;
extern int DAT_0034a790;
extern const int DAT_002ca630[];
extern ExhRec DAT_00341d10;
extern unsigned char DAT_00341954;
extern unsigned char DAT_00341990;
extern short DAT_0034196a;
extern short DAT_003418fa;
extern unsigned char DAT_00341b94;
extern unsigned char DAT_00341bd0;
extern short DAT_00341baa;
extern short DAT_00341b3a;
unsigned char FUN_001c7158(void);
Kind *FUN_0016b21c(int);
void FUN_000143b0(char *, const char *, ...);
void FUN_000149a8(char *, const char *);
ExhRec *FUN_001933e8(const char *);
NpcRec *FUN_00193364(const char *);
NpcRec *FUN_00193474(int);
NpcRec *FUN_0024e30c(unsigned);
}

extern "C" static void FUN_0024e6a0(...)
{
}

static inline int Rand()
{
    DAT_00310fc8 = DAT_00310fc8 * 0x343fd + 0x269ec3;
    return (DAT_00310fc8 >> 16) & 0x7fff;
}

static inline bool IsKind45(int i)
{
    return (unsigned)(FUN_0016b21c(i)->kind - 4) < 2;
}

static inline bool Unusable(int i) { return !((unsigned)(FUN_0016b21c(i)->kind - 4) < 2); }
static inline bool Avail() { return FUN_001c7158() != 0; }
static inline bool Taken(int i) { bool r = DAT_0034a780 == i; if (!r) r = Unusable(i); return r; }
static inline bool Same(int i) { bool r = DAT_0034a780 == i; if (!r) r = DAT_0034a784 == i; return r; }
static inline bool Taken2(int i) { bool r = Same(i); if (!r) r = Unusable(i); return r; }
static inline bool Blocked() { bool r = DAT_002fbef4 == 3; if (r) r = !(FUN_001c7158() != 0); return r; }
static inline const int &Max(const int &a, const int &b) { return a < b ? b : a; }
static inline int Clamp(const int &v, const int &lo, const int &hi) { return v < lo ? lo : (hi < v ? hi : v); }

static inline int Table()
{
    return (int)(DAT_003415a8 + 0x86c10);
}

static inline int Level(int table, int use_first_group, int index)
{
    unsigned char sel = (use_first_group != 0) ^ 1;
    int v = *(char *)(index * 6 + (sel * 0x60 + table) + 4) - 1;
    return Max(v, DAT_002fbefc);
}

static inline int CurLevel(int table)
{
    return Level(table, DAT_002fbef8, DAT_0034a780);
}

static inline bool InRange(short id)
{
    bool r;
    if (id >= 0 && id < 15) r = true; else r = false;
    return r;
}

extern "C" void FUN_0024e6cc(int a, int b, int c)
{
    int weights[5];
    int ids[5];
    char ex_name[256];
    char partner_name[256];
    char pc_name[256];

    if ((b != 0 || a != 0) && (DAT_002fbef9 != 0 || b != 0)) {
        if (a != 0) {
            DAT_0034a780 = -1;
            if (DAT_002fbef8 == 0) DAT_0034a784 = -1;
        } else if (DAT_002fbef4 == 3) {
            bool found = false;
            for (int i = 0; i < 15; i++) {
                if (i != DAT_0034a780 && Avail()) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                DAT_0034a780 = -1;
                if (DAT_002fbef8 == 0) DAT_0034a784 = -1;
            }
        }
        int i;
        bool bad;
        do {
            i = Rand() % 15;
            bad = Taken(i);
            if (!bad) bad = Blocked();
        } while (bad);
        DAT_0034a780 = i;
        if (DAT_002fbef8 == 0 && DAT_002fbefa == 0) {
            do {
                i = Rand() % 15;
                bad = Taken2(i);
                if (!bad) bad = Blocked();
            } while (bad);
            DAT_0034a784 = i;
        }
    }

    int total = 0;
    int lv = CurLevel(Table());
    switch (DAT_002fbef4) {
    case 0:
        DAT_0034a78c = 0;
        ids[0] = 4;
        ids[1] = 3;
        ids[2] = 2;
        ids[3] = 1;
        ids[4] = 0;
        break;
    case 1:
        DAT_0034a78c = 0;
        ids[0] = 9;
        ids[1] = 8;
        ids[2] = 7;
        ids[3] = 6;
        ids[4] = 5;
        break;
    case 2:
        DAT_0034a78c = 0;
        ids[0] = 14;
        ids[1] = 13;
        ids[2] = 12;
        ids[3] = 11;
        ids[4] = 10;
        break;
    case 3:
        DAT_0034a78c = 0;
        DAT_0034a790 = 15;
        break;
    }
    if (DAT_002fbef4 != 3) {
        int k = Clamp(lv, ids[4], ids[0]) - ids[4];
        for (int j = 0; j < 5; j++) {
            weights[j] = DAT_002ca630[k + j];
            total += weights[j];
        }
        int r = Rand() % total;
        for (int j = 0; j < 5; j++) {
            if (r < weights[j]) {
                DAT_0034a790 = ids[j];
                break;
            }
            r -= weights[j];
        }
    }

    NpcRec *partner = 0;
    if (DAT_002fbef9 != 0 || b != 0 || a != 0 || c != 0) {
        if (DAT_002fbef4 == 3 && DAT_002fbef8 != 0 && DAT_002fbef9 == 0) {
            FUN_000143b0(ex_name, "Exhibition_%03d", DAT_0034a780);
        } else {
            FUN_000143b0(ex_name, "Exhibition_%03d", DAT_0034a780 + (DAT_002fbef8 != 0 ? 100 : 500));
        }
        DAT_00341d10 = *FUN_001933e8(ex_name);
        if (DAT_002fbef8 == 0) {
            NpcRec *src;
            if (DAT_002fbefa != 0) {
                DAT_00341d10.unk34 = 2;
                src = FUN_0024e30c(0);
            } else {
                DAT_00341d10.unk34 = 2;
                src = FUN_00193474(DAT_0034a784 + (DAT_002fbef4 >= 3) * 15);
            }
            FUN_000143b0(partner_name, "Npc_999_%02d", DAT_0034a790);
            if (src != 0) {
                DAT_00341d10.partner_id = 999;
                DAT_00341d10.partner_sub = DAT_0034a790;
                FUN_000149a8(DAT_00341d10.partner_name, partner_name);
                partner = FUN_00193364(partner_name);
                short racket = partner->racket;
                FUN_0024e6a0("PartnerRacket=%d\n", racket);
                *partner = *src;
                partner->racket = racket;
                partner->unk00[5] = racket;
                FUN_000149a8(partner->name, partner_name);
            }
        } else {
            DAT_00341d10.partner_id = -1;
        }
    }

    FUN_000143b0(pc_name, "Npc_998_%02d", DAT_0034a790);
    NpcRec *pc;
    NpcRec *src;
    if (DAT_002fbef9 != 0) {
        if (partner != 0) {
            src = FUN_0024e30c(partner->id);
        } else {
            src = FUN_0024e30c(0);
        }
    } else {
        src = FUN_00193474(DAT_0034a780 + (DAT_002fbef4 >= 3) * 15);
    }
    DAT_00341d10.pc_id = 998;
    DAT_00341d10.pc_sub = DAT_0034a790;
    FUN_000149a8(DAT_00341d10.pc_name, pc_name);
    DAT_00341d10.unk33 = 2;
    pc = FUN_00193364(pc_name);
    short racket = pc->racket;
    FUN_0024e6a0("PcRacket=%d\n", racket);
    *pc = *src;
    pc->racket = racket;
    pc->unk00[5] = racket;
    FUN_000149a8(pc->name, pc_name);

    int hand_a[4] = {0, 9, 19, 29};
    int hand_b[4] = {0, 0, 0, 4};
    int hand_c[16] = {0, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9};
    int ha = hand_a[DAT_002fbef4];
    int hb = hand_b[DAT_002fbef4];
    FUN_0024e6a0("hand0=%d\n", pc->hand0);
    pc->unk7d[9] = DAT_0034a790;
    pc->unk64 = DAT_0034a790;
    if (c != 0) DAT_00341954 = pc->unk7d[9];
    FUN_0024e6a0("hand0=%d\n", pc->hand0);
    if (InRange(pc->unk00[0])) {
        pc->unk7d[11] = ha;
    } else {
        pc->unk7d[11] = hb;
    }
    DAT_00341990 = pc->unk7d[11];
    FUN_0024e6a0("hand0=%d\n", pc->hand0);
    pc->unk7d[10] = hand_c[DAT_0034a790];
    DAT_003418fa = DAT_0034196a = pc->racket;
    FUN_0024e6a0("hand0=%d\n", pc->hand0);
    if (partner != 0) {
        partner->unk7d[9] = pc->unk7d[9];
        partner->unk64 = pc->unk7d[9];
        if (c != 0) DAT_00341b94 = partner->unk7d[9];
        if (InRange(partner->unk00[0])) {
            partner->unk7d[11] = ha;
        } else {
            partner->unk7d[11] = hb;
        }
        DAT_00341bd0 = partner->unk7d[11];
        partner->unk7d[10] = pc->unk7d[10];
        DAT_00341b3a = DAT_00341baa = partner->racket;
    }
}
// own1: 1042/1042 words, 25 differ (first +0x140); mwccpsp 3.0.1 b219 -O4,p C++ (static helper FUN_0024e6a0 placed via helpers=).
// Residual A (~10 words, 0x24e80c-0x24e878, first pick loop): retail keeps the Taken() result in v1 and `bad` in v0; ours swaps them (andi/bnez registers).
//   Ruled out: separate/reordered bad and i declarations, six Taken() spellings, `i == G780` order, while/do-while condition forms. The second loop (Taken2) and Blocked() match.
// Residual B (~15 words, 0x24e984-0x24e9d4, inlined FUN_0024e268 Level): retail computes the table pointer (DAT_003415a8+0x86c10) first into t0, then G780 (a2), `move a3,zero`, then DAT_002fbef8 (a1);
//   ours loads DAT_002fbef8 first and adds the table late. Single-expression `index*6 + (sel*0x60 + table) + 4` (this file) beat the += form of the standalone FUN_0024e268 (33 -> 25).
//   Ruled out: tbl local before/after `total = 0`, total placement, int/char*/void* table types, uchar/const params, const-ref index (broken), body reorder, struct-array indexing (26),
//   real helper functions with auto_inline / inline_max_auto_size 24-48 (main body unchanged). No pragma tried beyond those in MWCC-NOTES.
// near miss from lane own1 (unreviewed)
