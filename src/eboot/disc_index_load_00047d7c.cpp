/* HSTMATCH module=EBOOT.BIN start=0x00047d7c end=0x000480a4 symbol=FUN_00047d7c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry {
    char *name;
    int start;
    int end;
    int blocks;
};

inline void *operator new(unsigned int, void *p) { return p; }

struct Tag {
};

struct EntryVec {
    Entry *data;
    unsigned size;
    unsigned cap;
};

extern "C" {
extern char DAT_002bc1e0[];
extern char DAT_002bc200[];
extern char DAT_002d0700[];
extern unsigned char DAT_0030ab8c;
extern EntryVec DAT_00311010;
void FUN_0001b880(void);
int FUN_002b7790(const char *, int, int);
long long FUN_002b77a0(int, long long, int);
int FUN_002b7720(int, void *, int);
int FUN_002b7738(int);
unsigned int FUN_002b7cf8(void);
void FUN_002b7958(int);
void FUN_00046564(const char *, ...);
void *FUN_00000a90(int);
void FUN_000009e4(void *);
char *FUN_00015ea0(char *, const char *);
int FUN_00014a40(const char *);
char *FUN_000149a8(char *, const char *);
int FUN_0000d62c(const char *);
void FUN_000489e0(EntryVec *, const Entry *, Tag);
}

inline void PushBack(EntryVec *v, const Entry &e)
{
    if (v->size < v->cap) {
        Entry *p = &v->data[v->size];
        new (p) Entry(e);
        v->size++;
    } else {
        FUN_000489e0(v, &e, Tag());
    }
}

extern "C" void FUN_00047d7c(void)
{
    char *buf;
    int fd;
    while ((fd = FUN_002b7790(DAT_002bc1e0, 1, 0)) < 0)
        FUN_0001b880();
    int size;
    while ((size = (int)FUN_002b77a0(fd, 0LL, 2)) < 0)
        FUN_0001b880();
    while (FUN_002b77a0(fd, 0LL, 0) < 0)
        FUN_0001b880();
    buf = (char *)FUN_00000a90(size + 1);
    buf[size] = 0;
    for (;;) {
        while ((FUN_002b7cf8() & 0x20) == 0) {
            FUN_00046564(DAT_002d0700);
            FUN_002b7958(1000000);
        }
        DAT_0030ab8c = 1;
        if (FUN_002b7720(fd, buf, size) == size) {
            DAT_0030ab8c = 0;
            break;
        }
        DAT_0030ab8c = 0;
        FUN_002b7958(1000000);
    }
    while (FUN_002b7738(fd) < 0)
        FUN_002b7958(1000000);
    char *tok = FUN_00015ea0(buf, DAT_002bc200);
    while (tok != 0) {
        Entry e;
        e.name = (char *)FUN_00000a90(FUN_00014a40(tok) + 1);
        FUN_000149a8(e.name, tok);
        FUN_00015ea0(0, DAT_002bc200);
        e.start = FUN_0000d62c(FUN_00015ea0(0, DAT_002bc200));
        e.blocks = (FUN_0000d62c(FUN_00015ea0(0, DAT_002bc200)) + 2047) / 2048;
        e.end = e.start + e.blocks - 1;
        PushBack(&DAT_00311010, e);
        FUN_00015ea0(0, DAT_002bc200);
        tok = FUN_00015ea0(0, DAT_002bc200);
    }
    FUN_000009e4(buf);
}
