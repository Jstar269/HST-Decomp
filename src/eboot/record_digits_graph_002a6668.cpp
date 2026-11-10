/* HSTMATCH module=EBOOT.BIN start=0x002a6668 end=0x002a69d0 symbol=FUN_002a6668 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Model {
    char pad[0x24];
    int f24, f28;
};

struct ItemDef {
    int layer;
    char *name;
    int value;
};

struct Quad {
    int x[4];
    int y[4];
};

struct Digits {
    int d[7];
};

struct Screen {
    char pad0[0x68];
    Model *digits[7];
    char pad84[0x8c - 0x84];
    Model *graph;
    char pad90[0x898 - 0x90];
    Quad quads[(0x1558 - 0x898) / 32];
    unsigned char show_graph;
    char pad1559[0x16f0 - 0x1559];
    int nquads;
    char pad16f4[0x1700 - 0x16f4];
    void *tex;
};

extern "C" {
extern char *DAT_003415a8;
extern Digits DAT_0030105c;
extern ItemDef DAT_00300d44[];
extern int DAT_00301078;
extern int DAT_0030107c;
int FUN_002af0c4(char *);
int FUN_002af0cc(char *);
void FUN_000ffb3c(Model *, int);
void FUN_00051490(void *tex, int a, int b, int w, int h, int c, int d);
void FUN_00052154(float, float, float, float, float, float, float, float, float, void *, int, int, int);
void FUN_00051b5c(float, float, float, float, float, float, void *, int, int);
void FUN_0005185c(void *tex);
}

extern "C" void FUN_002a6668(Screen *self)
{
    char *stats = DAT_003415a8 + 0x87128;
    int n = FUN_002af0c4(stats);
    Digits d = DAT_0030105c;
    d.d[4] = n >= 100 ? (n / 100) % 10 : -1;
    d.d[5] = n >= 10 ? (n / 10) % 10 : -1;
    d.d[6] = n % 10;
    if (self->show_graph == 0 || FUN_002af0cc(stats) > 100) {
        for (int i = 0; i < 7; i++)
            FUN_000ffb3c(self->digits[i], 1);
    } else {
        for (int i = 0; i < 7; i++) {
            if (d.d[i] >= 0) {
                FUN_000ffb3c(self->digits[i], 0);
                int v = DAT_00300d44[i + 1].value + d.d[i];
                Model *m = self->digits[i];
                m->f24 = v;
                m->f28 = v;
            } else {
                FUN_000ffb3c(self->digits[i], 1);
            }
        }
    }
    FUN_002af0cc(stats);
    if (self->show_graph != 0) {
        FUN_000ffb3c(self->graph, 1);
        FUN_00051490(self->tex, 0, 0, 480, 272, 0, 0);
        for (int i = 0; i < self->nquads - 1; i++) {
            Quad *q = &self->quads[i];
            FUN_00052154(q->x[0], q->y[0], 65280.0f, q->x[1], q->y[1], 65280.0f, q->x[2], q->y[2], 65280.0f,
                         self->tex, DAT_00301078, DAT_00301078, DAT_00301078);
            FUN_00052154(q->x[0], q->y[0], 65280.0f, q->x[2], q->y[2], 65280.0f, q->x[3], q->y[3], 65280.0f,
                         self->tex, DAT_00301078, DAT_00301078, DAT_00301078);
            FUN_00051b5c(q->x[0], q->y[0], 65280.0f, q->x[3], q->y[3], 65280.0f, self->tex, DAT_0030107c,
                         DAT_0030107c);
        }
        FUN_0005185c(self->tex);
    } else {
        FUN_000ffb3c(self->graph, 0);
    }
}
