/* HSTMATCH module=EBOOT.BIN start=0x000f31e0 end=0x000f355c symbol=FUN_000f31e0 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Widget {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void SetState(int);
};

struct Elem {
    unsigned int color;
    short x0;
    short y0;
    short x1;
    short y1;
    char pad0c[0x20 - 0xc];
    Widget *widget;
    int unk24;
};

struct ElemSpec {
    int layer;
    char *name;
    int value;
};

struct Game {
    char pad[0x45];
    unsigned char singles;
};

struct InPane {
    void *vtbl;
    char pad04[0x54];
    void *layers[4];
    Elem *elems[80];
    Widget *widgets[4];
    void *menu;
    char pad1bc[4];
    int arg;
    char pad1c4[0x1fc - 0x1c4];
    short cx[2];
    short cy[2];
    char pad204[0x21c - 0x204];
    int unk21c;
};

enum { LAYERS_OFF = 0x58, ELEMS_OFF = 0x68, WIDGETS_OFF = 0x1a8 };

extern "C" {
extern char DAT_003077f4[];
extern Game *DAT_00338b44;
extern char DAT_002d80c4[];
extern char DAT_002d80f8[];
extern float DAT_002d6c9c;
extern char *DAT_002d7b14[];
extern char *DAT_002d7ac8[];
extern ElemSpec DAT_002d7cf4[];
extern int DAT_002d80b4[];
void FUN_00102e38(void *);
void FUN_00104118(void *, char *, int);
void FUN_00103540(void *, float);
void FUN_001037c8(void *, int);
void *FUN_00000bcc(unsigned int);
void *FUN_000f051c(void *);
void *FUN_001039d8(void *, char *, int);
Elem *FUN_001026b8(void *, char *);
void FUN_000ffc3c(Elem *, int);
void FUN_000f3640(InPane *);
void FUN_000f4cac(InPane *);
void FUN_000f4cf0(InPane *);
}

static inline bool Doubles()
{
    return (DAT_00338b44->singles != 0) ^ 1;
}

extern "C" InPane *FUN_000f31e0(InPane *self, int arg)
{
    char *bytes = (char *)self;
    FUN_00102e38(self);
    self->vtbl = DAT_003077f4;
    self->menu = 0;
    self->unk21c = 0;
    if (Doubles())
        FUN_00104118(self, DAT_002d80c4, 0);
    else
        FUN_00104118(self, DAT_002d80f8, 0);
    FUN_00103540(self, DAT_002d6c9c);
    FUN_001037c8(self, -251);
    void *m = FUN_00000bcc(0x84);
    if (m != 0)
        m = FUN_000f051c(m);
    self->menu = m;
    {
        int i = 0;
        char **a = DAT_002d7b14;
        int off = 0;
        char **b = DAT_002d7ac8;
        do {
            if (Doubles())
                *(void **)(bytes + off + LAYERS_OFF) = FUN_001039d8(self, *a, 0);
            else
                *(void **)(bytes + off + LAYERS_OFF) = FUN_001039d8(self, *b, 0);
            i++;
            a++;
            off += 4;
            b++;
        } while (i < 4);
    }
    {
        int i = 0;
        ElemSpec *e = DAT_002d7cf4;
        int off = 0;
        do {
            Elem *h = FUN_001026b8(self->layers[e->layer], e->name);
            *(Elem **)(bytes + off + ELEMS_OFF) = h;
            if (h != 0)
                e->value = h->unk24;
            else
                e->value = 0;
            i++;
            e++;
            off += 4;
        } while (i < 80);
    }
    {
        int i = 0;
        int *p = DAT_002d80b4;
        int off = 0;
        do {
            if (self->elems[*p] != 0) {
                Widget *w = self->elems[*p]->widget;
                *(Widget **)(bytes + off + WIDGETS_OFF) = w;
                w->SetState(-16);
            }
            i++;
            p++;
            off += 4;
        } while (i < 4);
    }
    {
        int i = 0;
        int off = 0;
        while (i < (DAT_00338b44->singles == 0 ? 4 : 2)) {
            FUN_000ffc3c(*(Elem **)(bytes + off + 0xc0), -250);
            off += 4;
            i++;
        }
    }
    self->arg = arg;
    FUN_000f3640(self);
    self->cx[0] = (self->elems[62]->x0 + self->elems[62]->x1) / 2;
    self->cy[0] = (self->elems[62]->y0 + self->elems[62]->y1) / 2;
    self->cx[1] = (self->elems[63]->x0 + self->elems[63]->x1) / 2;
    self->cy[1] = (self->elems[63]->y0 + self->elems[63]->y1) / 2;
    self->elems[66]->color = 0xffffff;
    self->elems[67]->color = 0xffffff;
    self->elems[72]->color = 0xffffff;
    self->elems[73]->color = 0xffffff;
    self->elems[46]->color = 0x802a237f;
    self->elems[47]->color = 0x80004b7f;
    self->elems[48]->color = 0x802a237f;
    self->elems[49]->color = 0x80004b7f;
    self->elems[50]->color = 0x802a237f;
    self->elems[51]->color = 0x80004b7f;
    self->elems[52]->color = 0x802a237f;
    self->elems[53]->color = 0x80004b7f;
    FUN_000f4cac(self);
    FUN_000f4cf0(self);
    return self;
}
