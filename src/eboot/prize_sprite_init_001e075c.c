/* HSTMATCH module=EBOOT.BIN start=0x001e075c end=0x001e09dc symbol=FUN_001e075c lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef struct Layout Layout;
typedef struct Panel Panel;

typedef struct PrizeSprite {
    char pad0[0x1f4];
    Layout *layout_1f4;
    char pad1[0x810 - 0x1f8];
    Layout *layout_810;
    char pad2[0x175c - 0x814];
    Layout *layout_175c;
    char pad3[0x1954 - 0x1760];
    Panel *panel_1954;
    Layout *layout_1958;
    Layout *layout_195c;
    char pad4[0x1d68 - 0x1960];
    Layout *layout_1d68;
    char pad5[0x21e4 - 0x1d6c];
    Layout *layout_21e4;
    char pad6[0x235c - 0x21e8];
    Layout *layout_235c;
    char pad7[0x250c - 0x2360];
    Layout *layout_250c;
} PrizeSprite;

typedef struct GameState {
    char pad[0x597];
    unsigned char flag_597;
    char pad1;
    unsigned char flag_599;
} GameState;

extern char DAT_002ebaec[];
extern char DAT_002ebaf8[];
extern char DAT_0030ebf4[];
extern int DAT_0034a678;

extern void FUN_00000020(char *);
extern void FUN_00000028(void);
extern void *FUN_00000bcc(int);
extern Layout *FUN_00102e38(void *);
extern Panel *FUN_002226b4(void *, int);
extern GameState *FUN_001ee200(void);
extern void FUN_001e1fb0(PrizeSprite *);
extern void FUN_001e1964(PrizeSprite *);
extern void FUN_001e1d70(PrizeSprite *);
extern void FUN_001e1e24(PrizeSprite *);
extern void FUN_001e1ed8(PrizeSprite *);
extern void FUN_001e23ac(PrizeSprite *);
extern void FUN_001e22cc(PrizeSprite *);
extern void FUN_001e2528(PrizeSprite *);
extern void FUN_0001cad0(char *, int, char *);
extern void FUN_0001cb88(char *);

#define NEW_LAYOUT(dst) do { void *m = FUN_00000bcc(0x54); if (m) m = FUN_00102e38(m); (dst) = m; } while (0)

void FUN_001e075c(PrizeSprite *self)
{
    int mode;
    int bit2;

    FUN_00000020(DAT_002ebaec);
    if (self->layout_1f4 == 0) {
        mode = 1;
        switch (DAT_0034a678) {
        case 0:
            mode = 3;
            break;
        case 0x31:
            mode = 2;
            break;
        }
        NEW_LAYOUT(self->layout_1f4);
        if (FUN_001ee200()->flag_599) {
            NEW_LAYOUT(self->layout_810);
        }
        if (FUN_001ee200()->flag_597) {
            NEW_LAYOUT(self->layout_175c);
        }
        NEW_LAYOUT(self->layout_1d68);
        NEW_LAYOUT(self->layout_1958);
        NEW_LAYOUT(self->layout_195c);
        bit2 = mode & 2;
        if (bit2) {
            NEW_LAYOUT(self->layout_235c);
        }
        mode = mode & 1;
        if (mode) {
            NEW_LAYOUT(self->layout_21e4);
        }
        NEW_LAYOUT(self->layout_250c);
        FUN_001e1fb0(self);
        FUN_0001cad0(DAT_0030ebf4, 1, DAT_002ebaf8);
        FUN_001e1964(self);
        if (FUN_001ee200()->flag_599) {
            FUN_001e1d70(self);
        }
        if (FUN_001ee200()->flag_597) {
            FUN_001e1e24(self);
        }
        FUN_001e1ed8(self);
        {
            void *m = FUN_00000bcc(0xcc);
            if (m) m = FUN_002226b4(m, 1);
            self->panel_1954 = m;
        }
        if (bit2) {
            FUN_001e23ac(self);
        }
        if (mode) {
            FUN_001e22cc(self);
        }
        FUN_001e2528(self);
        FUN_0001cb88(DAT_0030ebf4);
    }
    FUN_00000028();
}
