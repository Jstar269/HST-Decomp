/* HSTMATCH module=hstp_trans_main.prx start=0x001c7854 end=0x001c7a58 symbol=FUN_001c7854 lang=c++ */
/* Hand-written reconstruction: clone of the source of preview_camera_reset_001b9b0c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* Matching idioms: the camera is a class with a virtual destructor on a 4-byte non-polymorphic base (vptr at +4, deleting dtor via lw t9,8(t9)); DAT_003b7ebc is passed as an array to const char * parameters, which keeps retail's lui/addiu at both call sites; the success branch is `cam != 0 && F() != 0`. */
extern "C" {
void FUN_00000020(char *);
void FUN_00000028(void);
int FUN_0001342c(void);
int FUN_000143b0(char *, const char *, ...);
void FUN_0001c5f4(const char *, char *);
void *FUN_00000bcc(int);
void *FUN_00049194(void *, int);
void FUN_000492ac(void *, int, char *);
void FUN_0001c73c(const char *);
int FUN_00049380(float, void *, int, int);
void FUN_00059d68(void *, int);
extern int DAT_003e95e0;
extern unsigned char DAT_003e19f8;
extern char DAT_003b7ebc[];
extern char DAT_00391788;
extern char DAT_00391798;
extern char DAT_0039179c;
extern char DAT_003917c0;
extern char DAT_003917e8;
extern char *DAT_00391738[];
}

struct CamBase {
    int x;
};
struct Cam : CamBase {
    virtual ~Cam();
};

struct PrevCam {
    char pad0[0x3c];
    Cam *cam;
    unsigned char f40;
    unsigned char f41;
    char pad1[2];
    int f44;
};

extern "C" void FUN_001c7854(PrevCam *p, int eval)
{
    char camName[256];
    char path[256];
    char desc[256];
    char *names[20];
    char **src;
    char **dst;
    int idx;
    int n;
    int cnt;
    void *obj;
    void *mem;

    FUN_00000020(&DAT_00391788);
    idx = *(int *)(DAT_003e95e0 + 0x1364);
    if (eval != 0)
        n = 1;
    else
        n = FUN_0001342c() % 5 + 1;
    p->f41 = (unsigned char)eval;
    p->f44 = 0;
    src = DAT_00391738;
    dst = names;
    cnt = 10;
    do {
        dst[0] = src[0];
        dst[1] = src[1];
        src += 2;
        dst += 2;
    } while (--cnt > 0);
    FUN_000143b0(camName, &DAT_00391798, names[idx - 1]);
    FUN_000143b0(path, &DAT_0039179c, idx);
    if (DAT_003e19f8 == 0)
        FUN_000143b0(desc, &DAT_003917c0, camName, camName, n);
    else
        FUN_000143b0(desc, &DAT_003917e8, camName, camName, n);
    FUN_0001c5f4(DAT_003b7ebc, path);
    if (p->cam != 0) {
        delete p->cam;
        p->cam = 0;
    }
    mem = FUN_00000bcc(0x1f0);
    if (mem != 0)
        mem = FUN_00049194(mem, 1);
    p->cam = (Cam *)mem;
    FUN_000492ac(p->cam, 0, desc);
    FUN_0001c73c(DAT_003b7ebc);
    if (p->cam != 0 && FUN_00049380(65533.0f, p->cam, 0, 1) != 0) {
        p->f40 = 0;
        obj = *(void **)p->cam;
        FUN_00059d68(obj, -1);
        *(float *)((char *)obj + 0xd0) = *(float *)((char *)obj + 0xc4);
    } else {
        p->f40 = 1;
    }
    FUN_00000028();
}
