/* HSTMATCH module=EBOOT.BIN start=0x001e1fb0 end=0x001e22cc symbol=FUN_001e1fb0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Prize {
    int kind;
    int id;
    int extra;
};

struct Save {
    char pad[0x12e0];
    Prize prizes[10];
    char pad1358[0x1368 - 0x1358];
    int f1368;
};

struct Board {
    char pad[0x170];
    int f170;
};

struct Result {
    char pad00[0x1958];
    void *layout;
    void *layout2;
    void *panes[3];
    char items[0x1aac - 0x196c];
    char item1[0x1ab8 - 0x1aac];
    char item3[0x1adc - 0x1ab8];
    void *prize_panes[(0x1b34 - 0x1adc) / 4];
    char anim1b34[0x1bc0 - 0x1b34];
    char anim1bc0[0x1c4c - 0x1bc0];
    char anim1c4c[0x1cd8 - 0x1c4c];
    char anim1cd8[0x1d64 - 0x1cd8];
    Board *board;
};

extern "C" {
extern char DAT_0030ebf4[];
extern char DAT_002ebe78[];
extern char DAT_002ebea8[];
extern char DAT_002ebecc[];
extern char *DAT_002ebc48[];
extern char DAT_002c8198[];
extern char DAT_002c83a8[];
extern char DAT_002c83cc[];
extern char DAT_002ebef8[];
extern char DAT_002ebf28[];
extern char DAT_002ebd24[];
extern char DAT_002ebf5c[];
extern char DAT_002ebf60[];
extern char DAT_002ebf68[];
void FUN_00104118(void *, void *, int);
void FUN_0001cb88(void *);
void FUN_0001cad0(void *, int, char *);
void FUN_00103540(void *, float);
void FUN_001037c8(void *, int);
void *FUN_00000bcc(unsigned int);
Board *FUN_0021d7c4(void *);
Save *FUN_001ee200(void);
void FUN_0021e2d4(Board *, int);
void FUN_00216e90(void **, void *, char **, int);
void FUN_00216f14(void *, void **, void *, int);
void FUN_00216fb0(void *, void **, void *, int);
void FUN_0021704c(void *, void **, void *, int);
void FUN_0001ca58(void *, char *, int);
void *FUN_000649c0(void *);
void FUN_00064bf4(void *, char *, int, char *);
void *FUN_0006517c(void *, int, int);
void *FUN_00014d64(void *, char *);
void FUN_000143b0(char *, char *, ...);
void FUN_000111c0(void *, char *, int);
void *FUN_00102e38(void *);
void FUN_001041dc(void *, void *, char *, int);
void FUN_00217778(void *, void *);
void FUN_00217704(void *, void *);
}

extern "C" void FUN_001e1fb0(Result *self)
{
    FUN_00104118(self->layout2, DAT_002ebe78, 0);
    FUN_0001cb88(DAT_0030ebf4);
    FUN_0001cad0(DAT_0030ebf4, 1, DAT_002ebea8);
    FUN_00104118(self->layout, DAT_002ebecc, 0);
    FUN_00103540(self->layout, 2900.0f);
    FUN_001037c8(self->layout, -256);
    void *b = FUN_00000bcc(0x210);
    if (b != 0)
        b = FUN_0021d7c4(b);
    self->board = (Board *)b;
    self->board->f170 = FUN_001ee200()->f1368;
    FUN_0021e2d4(self->board, 1);
    FUN_0001cb88(DAT_0030ebf4);
    FUN_00216e90(self->panes, self->layout, DAT_002ebc48, 3);
    FUN_00216f14(self->items, self->panes, DAT_002c8198, 16);
    FUN_00216fb0(self->item1, self->panes, DAT_002c83a8, 1);
    FUN_0021704c(self->item3, self->panes, DAT_002c83cc, 3);
    bool shown;
    int i;
    int n;
    void *res;
    Prize *p;
    p = FUN_001ee200()->prizes;
    shown = false;
    for (i = 0; i < 10; i++, p++) {
        if (p->kind == 5) {
            n = p->id;
            if (n == 0)
                n = 1;
            FUN_0001ca58(DAT_0030ebf4, DAT_002ebef8, n);
            res = FUN_00000bcc(16);
            if (res != 0)
                res = FUN_000649c0(res);
            FUN_00064bf4(res, DAT_002ebf28, 1, DAT_002ebd24);
            void *node = FUN_00014d64(FUN_0006517c(res, 1, 0), DAT_002ebf5c);
            char buf[8];
            FUN_000143b0(buf, DAT_002ebf60, n);
            FUN_000111c0(node, buf, 2);
            void *pane = FUN_00000bcc(0x54);
            if (pane != 0)
                pane = FUN_00102e38(pane);
            self->prize_panes[p->id] = pane;
            FUN_001041dc(self->prize_panes[p->id], res, DAT_002ebf68, 0);
            FUN_00103540(self->prize_panes[p->id], 2900.0f);
            FUN_001037c8(self->prize_panes[p->id], -256);
            if (!shown)
                FUN_00217778(self->anim1bc0, self->prize_panes[p->id]);
            shown = true;
            FUN_0001cb88(DAT_0030ebf4);
        }
    }
    FUN_00217704(self->anim1b34, self->panes[1]);
    FUN_00217704(self->anim1c4c, self->panes[0]);
    FUN_00217778(self->anim1cd8, self->board);
}
