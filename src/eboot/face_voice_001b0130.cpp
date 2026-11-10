/* HSTMATCH module=EBOOT.BIN start=0x001b0130 end=0x001b043c symbol=FUN_001b0130 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct VoiceKey {
    char *key;
    int base;
    int base_doubles;
    int step;
    int num_off;
    int wide_off;
};

struct Game {
    char pad[0x45];
    unsigned char singles;
};

struct Face {
    int sounds[0x768 / 4];
    int voice[(0x798 - 0x768) / 4];
    unsigned char wide[4];
    int sel;
    int sel_alt;
};

extern "C" {
extern VoiceKey DAT_002e95c0[];
extern Game *DAT_00338b44;
extern char DAT_0030ebf4[];
extern char DAT_002e95f0[];
extern char DAT_002e9614[];
extern char DAT_002e9618[];
extern char DAT_002c619c[];
extern char DAT_002c61a0[];
void FUN_001b052c(Face *);
unsigned int FUN_00014a40(const char *);
void FUN_000149a8(char *, const char *);
int FUN_001b1744(int);
void FUN_0001ca58(void *, char *, ...);
int FUN_00014a58(const char *, const char *, unsigned int);
void FUN_000111c0(char *, const char *, int);
int FUN_0000d62c(const char *);
void FUN_001b0ef8(Face *, int, int, int, int);
void FUN_0001cb88(void *);
}

static inline void Clear(char *p, int n)
{
    if (p != 0) {
        do {
            *p++ = 0;
        } while (--n != 0);
    }
}

static inline bool In(int v, int lo, int hi)
{
    return lo <= v && v < hi;
}

static inline void Copy(char *dst, const char *b, const char *e)
{
    FUN_000111c0(dst, b, e - b);
}

extern "C" void FUN_001b0130(Face *self, int idx, char *name, int var)
{
    int *slot = &((int *)((char *)self + 0x768))[idx];
    unsigned char wide;
    int id;
    unsigned int i;
    VoiceKey *e;
    if (*slot != 0)
        FUN_001b052c(self);
    if (name == 0 || FUN_00014a40(name) < 2)
        return;
    if (!(var >= 0 && var < 10))
        var = 0;
    char buf[64];
    FUN_000149a8(buf, name);
    if (buf[FUN_00014a40(buf) - 1] == '*') {
        char c = var + 'a';
        buf[FUN_00014a40(buf) - 1] = c;
    }
    int sel = self->sel;
    if (sel < 0)
        sel = self->sel_alt;
    char *suffix;
    if (FUN_001b1744(sel) != 0)
        suffix = DAT_002c61a0;
    else
        suffix = DAT_002c619c;
    char *prefix;
    if (DAT_00338b44 != 0 && DAT_00338b44->singles == 0)
        prefix = DAT_002e9614;
    else
        prefix = DAT_002e9618;
    FUN_0001ca58(DAT_0030ebf4, DAT_002e95f0, prefix, buf, suffix);
    id = 474;
    wide = 0;
    i = 0;
    e = DAT_002e95c0;
    for (; i < 2; i++, e++) {
        if (FUN_00014a58(name, e->key, FUN_00014a40(e->key)) == 0) {
            char num[4];
            Clear(num, 4);
            Copy(num, name + e->num_off, name + e->num_off + 2);
            int base;
            if (DAT_00338b44 != 0 && DAT_00338b44->singles == 0)
                base = e->base_doubles;
            else
                base = e->base;
            id = base + e->step * FUN_0000d62c(num);
            id += var;
            if (i == 1) {
                if (name[e->wide_off] == 'w')
                    id += 30;
                wide = 1;
            }
            break;
        }
    }
    if (!(id >= 195 && id <= 224) && !(id >= 225 && id <= 254) && !(id >= 255 && id <= 300) && !(id >= 301 && id <= 346))
        id = 225;
    FUN_001b0ef8(self, id, 0, 0, 0);
    *slot = self->sounds[id];
    self->wide[idx] = wide;
    FUN_0001cb88(DAT_0030ebf4);
}
