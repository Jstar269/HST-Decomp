/* HSTMATCH module=EBOOT.BIN start=0x0012a698 end=0x0012aa1c symbol=FUN_0012a698 lang=c++ */
#define V(n) virtual void v##n();
struct VObj {
    V(0) V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9)
    V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29)
    V(30) V(31) V(32) V(33) V(34) V(35) V(36)
    virtual int Get9c();
};

struct Area {
    char pad00[0xf0];
    int id;
    char padf4[0x100 - 0xf4];
    float width;
    float pos;
    char pad108[0x110 - 0x108];
    int f110;
};

struct Walker : VObj {
    char pad04[0x58 - 4];
    Area *area;
    char pad5c[0x190 - 0x5c];
    int mode;
    char pad194[0x254 - 0x194];
    unsigned char npc;
};

extern "C" {
extern int DAT_0034179c;
extern void *DAT_0034a114;
extern VObj *DAT_0034a118;
extern int DAT_0033b538;
extern int DAT_0033b53c;
extern int DAT_0033b540;
extern int DAT_0033b544;
extern int DAT_0033b548;
extern int DAT_0033b54c;
extern float DAT_002da938;
extern float DAT_002da93c;
extern float DAT_002da940;
extern float DAT_002da944;
extern char DAT_002da948[];
extern char DAT_002da95c[];
extern char DAT_002da96c[];
int FUN_001b28f0(int *, const char *);
int FUN_001cd2a8(void *);
void FUN_001b2240(float, float, float, float, int *, int *, int, int, int, int);
}

static inline float Width(Area *a)
{
    return a ? a->width : 1.0f;
}

static inline float Pos(Area *a)
{
    return a ? a->pos : 0.0f;
}

extern "C" void FUN_0012a698(Walker *self)
{
    Area *area = self->area;
    if (area == 0)
        return;
    float w = Width(area);
    float p = Pos(area);
    int mode = self->mode;
    bool left = mode == 4;
    bool right = mode == 5;
    if (mode == 12) {
        int id = area->f110 < 0 ? area->id : -1;
        left = id == DAT_0033b538 || id == DAT_0033b540 || id == DAT_0033b53c;
        right = id == DAT_0033b54c || id == DAT_0033b548 || id == DAT_0033b544;
    }
    if (left) {
        if ((DAT_002da938 <= p && p < DAT_002da938 + w) || (DAT_002da93c <= p && p < DAT_002da93c + w))
        {
            if (self->npc != 0) {
                FUN_001b28f0(&DAT_0034179c, DAT_002da948);
            } else {
                int h = FUN_001b28f0(&DAT_0034179c, DAT_002da95c);
                float a = 6.0f;
                float b = 40.0f;
                int x = 0;
                if (FUN_001cd2a8(DAT_0034a114) == 0) {
                    x = DAT_0034a118->Get9c();
                    a = 2.0f;
                    b = 10.0f;
                }
                FUN_001b2240(0.0f, 1.0f, a, b, &DAT_0034179c, &h, self->Get9c(), 0, 0, x);
            }
        }
    } else if (right) {
        if ((DAT_002da940 <= p && p < DAT_002da940 + w) || (DAT_002da944 <= p && p < DAT_002da944 + w))
        {
            if (self->npc != 0) {
                FUN_001b28f0(&DAT_0034179c, DAT_002da96c);
            } else {
                int h = FUN_001b28f0(&DAT_0034179c, DAT_002da95c);
                float a = 6.0f;
                float b = 40.0f;
                int x = 0;
                if (FUN_001cd2a8(DAT_0034a114) == 0) {
                    x = DAT_0034a118->Get9c();
                    a = 2.0f;
                    b = 10.0f;
                }
                FUN_001b2240(0.0f, 1.0f, a, b, &DAT_0034179c, &h, self->Get9c(), 0, 0, x);
            }
        }
    }
}
// NEAR 18 (lane w4f1; the old far draft wip/FUN_0012a698_0012a698.c was 222/135 words): every instruction
// matches except the saved-register naming: retail keeps self in s1 and the FUN_001b2240 last argument x
// (0 or DAT_0034a118->Get9c()) in s0, ours the opposite. Ruled out: the stamp body as a static inline
// Step(self, str) vs written out twice, x declared first/last/at function top/as one shared local,
// void* x, Walker *const self, a member function (this), an if/else for x/a/b (35), assignment order (28).
// Window tests must be open-coded in the caller with the globals read directly (an inline taking the
// bounds loads them early; a bool helper materialises flags).
// near miss from lane w4f1 (unreviewed)
