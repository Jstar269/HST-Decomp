/* HSTMATCH module=EBOOT.BIN start=0x0028fb70 end=0x0028ff38 symbol=FUN_0028fb70 lang=c++ helpers=FUN_0028fb44@0x0028fb44-0x0028fb70 */
/* Hand-written reconstruction from the disassembly (same-file static log stub FUN_0028fb44 defined in front and placed by helpers=; name from the "PsmfPlayer::FinalizeForSuspend" log string); exact match verified; names and types await human review. */
struct Sock {
    unsigned int vtable;
    char pad04[16];
    int handle;
    char pad18[4];
};

struct Player {
    char pad00[0x28];
    unsigned char ready;
    unsigned char started;
    unsigned char closing;
    char pad2b;
    void *work_buf;
    int unk30;
    Sock a;
    Sock b;
    Sock c;
    int pipe;
    char pad8c[0xd4 - 0x8c];
    void *bufs[2];
    char paddc[4];
    int unke0;
    void *unke4;
    char pade8[0x1ec - 0xe8];
    unsigned char wave;
};

extern "C" {
extern char DAT_002ff390[];
extern char DAT_002ff3bc[];
extern char DAT_002cabe4[];
extern char DAT_002cabbc[];
extern char DAT_002ff3e0[];
extern char DAT_002ff408[];
extern char DAT_002ff434[];
extern char DAT_002ff2a8[];
extern char DAT_002ff2d0[];
extern char DAT_002ff27c[];
extern char DAT_002ff300[];
extern char DAT_002ff320[];
extern char DAT_002ff34c[];
static void FUN_0028fb44(...) { }
int FUN_002b7ee0(Player *);
int FUN_002b7e78(Player *);
int FUN_002b7980(int, int);
int FUN_002b79a8(int);
int FUN_002b7908(int);
int FUN_002b7e90(Player *);
void FUN_002b7958(int);
int FUN_002b7ed8(Player *);
int FUN_002b7ec0(Player *);
int FUN_002b7a08(int);
int FUN_002b78d0(int);
int FUN_002b7d50(void);
int FUN_002b7cc0(int);
int FUN_0006165c(void);
void FUN_002903cc(void);
void FUN_000104e0(void *);
}

static inline void WaitEnd(Sock *s)
{
    if (s->handle >= 0) {
        int r = FUN_002b7980(s->handle, 0);
        if (r != 0)
            FUN_0028fb44(DAT_002cabe4, r);
    }
}

static inline void Delete(Sock *s)
{
    if (s->handle >= 0) {
        int r = FUN_002b79a8(s->handle);
        if (r != 0)
            FUN_0028fb44(DAT_002cabbc, r);
        s->handle = -1;
    }
}

extern "C" int FUN_0028fb70(Player *self)
{
    if (self->ready == 0)
        return 1;
    FUN_0028fb44(DAT_002ff390);
    self->closing = 1;
    int st = FUN_002b7ee0(self);
    bool active = st != (int)0x80616001 && st != 0 && st != 1;
    int r;
    if (active) {
        r = FUN_002b7e78(self);
        if (r != 0)
            FUN_0028fb44(DAT_002ff3bc, r);
    }
    WaitEnd(&self->c);
    Delete(&self->c);
    WaitEnd(&self->b);
    Delete(&self->b);
    for (;;) {
        if (FUN_002b7908(self->a.handle) >= 0)
            break;
        r = FUN_002b7e90(self);
        if (r != 0)
            FUN_0028fb44(DAT_002ff3e0, r);
        FUN_002b7958(100000);
    }
    WaitEnd(&self->a);
    Delete(&self->a);
    if (active) {
        r = FUN_002b7ed8(self);
        if (r != 0)
            FUN_0028fb44(DAT_002ff408, r);
        r = FUN_002b7e90(self);
        if (r != 0)
            FUN_0028fb44(DAT_002ff3e0, r);
        FUN_0028fb44(DAT_002ff434);
    }
    r = FUN_002b7ec0(self);
    if (r != 0)
        FUN_0028fb44(DAT_002ff2a8, r);
    r = FUN_002b7a08(0);
    if (r < 0)
        FUN_0028fb44(DAT_002ff2d0, r);
    r = FUN_002b78d0(self->pipe);
    if (r != 0)
        FUN_0028fb44(DAT_002ff27c, r);
    r = FUN_002b7d50();
    if (r != 0)
        FUN_0028fb44(DAT_002ff300, r);
    r = FUN_002b7cc0(0x303);
    if (r < 0)
        FUN_0028fb44(DAT_002ff320, r);
    r = FUN_0006165c();
    if (r != 0)
        FUN_0028fb44(DAT_002ff34c, r);
    if (self->wave != 0)
        FUN_002903cc();
    for (unsigned i = 0; i < 2; i++) {
        if (self->bufs[i] != 0) {
            FUN_000104e0(self->bufs[i]);
            self->bufs[i] = 0;
        }
    }
    if (self->unke4 != 0) {
        FUN_000104e0(self->unke4);
        self->unke4 = 0;
    }
    if (self->work_buf != 0) {
        FUN_000104e0(self->work_buf);
        self->work_buf = 0;
    }
    self->unke0 = 0;
    self->started = 0;
    self->ready = 0;
    self->closing = 0;
    return 0;
}
