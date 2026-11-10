/* HSTMATCH module=EBOOT.BIN start=0x000bd014 end=0x000bd378 symbol=FUN_000bd014 lang=c++ */

struct Hdr {
    int type;
    int len;
    int f8;
    int fC;
};

struct Msg4 {
    int type;
    int len;
    int f8;
    int fC;
    Msg4(int t) : type(t) {}
};

struct Msg : Msg4 {
    int idx;
    Msg(int t) : Msg4(t) {}
};

struct Rx {
    int f0;
    void *f4;
    void *f8;
    int size;
    unsigned char done;
};

struct Handler {
    void (*fn)(void *, unsigned);
};

struct Net {
    char pad0[0x10];
    unsigned short port;
    char pad12[0x28 - 0x12];
    int state;
    char pad2c[0x17c - 0x2c];
    int link;
    char *buf;
    char pad184[0x190 - 0x184];
    int mode;
    int prev_mode;
    char pad198[0x1dc - 0x198];
    Rx *rx[16];
    unsigned char seen[0x26c - 0x21c];
    void *queue;
    Handler *handler;
    char pad274[0x27c - 0x274];
    int id_lo;
    int id_hi;
};

struct Glob {
    char pad[0x38];
    char f38[4];
};

extern Glob *DAT_00338ac4;
extern char DAT_002d53ec[];
extern char DAT_002d5400[];
extern char DAT_002d5440[];

extern "C" {
unsigned FUN_001268c4(void *);
void FUN_001268b8(void *, void *, int);
void FUN_00126884(void *, char *, int *);
void FUN_00011090(void *, const void *, int);
void FUN_000bc99c(Net *, char *);
void FUN_000ba3ac(const char *, ...);
unsigned char FUN_000bda40(Glob *, char *, int, int, int);
}

static inline int Accept(Net *s, const Msg4 &m)
{
    int r = s->id_hi == m.fC;
    if (r) r = s->id_lo == m.f8;
    return r;
}

static inline void SetState(Net *s, int st)
{
    s->prev_mode = s->mode;
    s->mode = 0;
    s->state = st;
    switch (s->state) {
    case 4:
    case 8:
        s->link = FUN_000bda40(DAT_00338ac4, DAT_00338ac4->f38, s->port, 1444, 0);
        break;
    case 10:
        break;
    }
}

extern "C" void FUN_000bd014(Net *s)
{
    if (s->link < 0)
        return;
    for (;;) {
        if (s->queue == 0)
            return;
        unsigned n = FUN_001268c4(s->queue);
        if (n < 16)
            return;
        int len;
        Hdr hdr;
        FUN_001268b8(s->queue, &hdr, 16);
        if ((int)n < hdr.len)
            return;
        len = hdr.len;
        FUN_00126884(s->queue, s->buf, &len);
        switch (hdr.type) {
        case 0:
            if (s->state == 8) {
                Msg m(0);
                FUN_00011090(&m, s->buf, 20);
                if (Accept(s, m))
                    FUN_000bc99c(s, s->buf);
            }
            break;
        case 3: {
            Msg m(3);
            FUN_00011090(&m, s->buf, 20);
            if (Accept(s, m))
                s->seen[m.idx] = 1;
            break;
        }
        case 4: {
            Msg4 m(4);
            FUN_00011090(&m, s->buf, 16);
            if (Accept(s, m)) {
                FUN_000ba3ac(DAT_002d53ec);
                SetState(s, 11);
            }
            break;
        }
        case 5: {
            Msg m(5);
            char *buf = s->buf;
            FUN_00011090(&m, buf, 20);
            Rx *r = s->rx[m.idx];
            if (r != 0 && Accept(s, m)) {
                if (r->f8 != 0) {
                    if (m.len - 20 == r->size) {
                        FUN_00011090(r->f8, buf + 20, m.len - 20);
                        s->rx[m.idx]->done = 1;
                    } else
                        FUN_000ba3ac(DAT_002d5400, m.len - 20, r->size);
                } else if (r->f4 != 0) {
                    if (m.len - 20 == r->size) {
                        FUN_00011090(r->f4, buf + 20, m.len - 20);
                        s->rx[m.idx]->done = 1;
                    } else
                        FUN_000ba3ac(DAT_002d5400, m.len - 20, r->size);
                }
            }
            break;
        }
        default:
            if (s->handler != 0)
                s->handler->fn(s->buf, hdr.len);
            FUN_000ba3ac(DAT_002d5440);
            break;
        }
    }
}
// NEAR 9 words (lane w4f2, 2026-11-10).
// Remaining diffs: case 5 (6 words, twice 3) - retail loads r->size into v1 before m.len into a1
//   (reusing r's register); ours loads m.len into v0 first. Default case (3 words) - retail keeps
//   s->handler in v1 (loaded in the switch-dispatch delay slot), ours uses v0.
// Ruled out: swapped compare, sizeof(Msg), int/unsigned size, size/len locals, Fill() inline helper,
//   assignment-in-condition, handler local / member Call() / (*fn) form, fn(char*,int) vs fn(...).
// Key find: Accept is (hi==fC) && (lo==f8) written as int r=...; if (r) r=...; return r;
// near miss from lane w4f2 (unreviewed)
