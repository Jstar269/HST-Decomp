/* HSTMATCH module=EBOOT.BIN start=0x001cb980 end=0x001cbccc symbol=FUN_001cb980 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#include "hst/types.h"
#include "hst/panel_data.h"

struct Prim {
    virtual void d0(); virtual void d1(); virtual void d2(); virtual void d3(); virtual void d4();
    virtual void d5(); virtual void d6(); virtual void d7(); virtual void d8(); virtual void d9();
    virtual void d10(); virtual void d11(); virtual void d12(); virtual void d13(); virtual void d14();
    virtual void d15(); virtual void d16(); virtual void d17(); virtual void d18(); virtual void d19();
    virtual void d20(); virtual void d21(); virtual void d22(); virtual void d23(); virtual void d24();
    virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28(); virtual void d29();
    virtual void d30();
    virtual void Show(int, int);
    char pad04[0xe0 - 4];
};

struct Obj {
    virtual void d0(); virtual void d1(); virtual void d2(); virtual void d3(); virtual void d4();
    virtual void d5(); virtual void d6(); virtual void d7(); virtual void d8(); virtual void d9();
    virtual void d10();
    virtual int Kind();
    int regs;
    char pad08[0x30 - 8];
    int name_id;
    int text_id;
};

struct Actor : Prim, Obj {
};

struct Owner {
    char pad00[0x64];
    char **names;
    char pad68[0x15c - 0x68];
    PanelData *panel;
};

extern "C" {
extern char DAT_002ea308[];
extern char DAT_002eaa7c[];
extern int DAT_002eaaac;
extern char *DAT_002c6430[];
extern unsigned int DAT_0034a10c;
int *FUN_0014c384(int, int);
void FUN_000143b0(char *, char *, ...);
void FUN_000149a8(char *, char *);
int FUN_00229470(void *, TextBank *, char *);
int FUN_00014934(char *, char *);
int FUN_0022be84(int, int, int, int);
TextBank *FUN_0018c9f4(int);
u8 *FUN_002294f4(void *, TextBank *, char *, int);
void FUN_0018a950(u32, char *);
void FUN_001cd88c(PanelData *, int, int);
int FUN_002297d8(void *, char *, int);
void FUN_0012ad54(float, Actor *, int, int, int, int, int);
void FUN_000935b0(float, Actor *);
}

static inline const int &Max(const int &a, const int &b)
{
    return a < b ? b : a;
}

static inline char *Lookup(TextBank *bank, int idx)
{
    if (bank->loaded == 0)
        return DAT_002c0e20;
    if (bank->header->groupCount < 1)
        return DAT_002c0e1c;
    if (idx < 0 || idx >= bank->groups[0].count)
        return DAT_002c0e18;
    return (char *)bank->groups[0].strings[idx];
}

extern "C" void FUN_001cb980(Owner *self, Obj *obj, char *key, int mode)
{
    char buf[256];
    if (obj == 0)
        return;
    if (obj->Kind() == 3) {
        char *name = self->names[obj->name_id];
        name = name != 0 ? name : DAT_002ea308;
        FUN_000143b0(buf, DAT_002eaa7c, name, key, *FUN_0014c384(obj->regs, 8));
    } else {
        FUN_000149a8(buf, key);
    }
    int idx = 0;
    int n = FUN_00229470(self->panel->map34, self->panel->bank, buf);
    int count = Max(n, DAT_002eaaac);
    int kind = 21;
    for (unsigned i = 0; i < 19; i++) {
        if (FUN_00014934(key, DAT_002c6430[i]) == 0) {
            kind = i + 2;
            break;
        }
    }
    int id = obj->text_id;
    switch (mode) {
    case 0:
        DAT_0034a10c = DAT_0034a10c * 0x343fd + 0x269ec3;
        idx = (int)((DAT_0034a10c >> 16) & 0x7fff) % count;
        break;
    case 1:
        idx = FUN_0022be84(id, kind, count, 0);
        break;
    case 2:
        idx = FUN_0022be84(id, kind, count, 1);
        break;
    }
    char *text = Lookup(FUN_0018c9f4(7), id);
    PanelData *panel = self->panel;
    u8 *found = FUN_002294f4(panel->map34, panel->bank, buf, idx);
    if (found != 0) {
        panel->unk63 = 0;
        panel->unk64 = 0;
        panel->text = found;
        panel->unk6c = 0;
        panel->unk70 = 1;
        FUN_0018a950(panel->unk20, text);
        FUN_001cd88c(panel, 2, 1);
    }
    panel->unk74 = -1;
    panel->unk78 = -1;
    int r = FUN_002297d8(panel->map34, buf, idx);
    if (r >= 0) {
        Actor *a = static_cast<Actor *>(obj);
        FUN_0012ad54(-1.0f, a, r, 0, 0, 1, 1);
        FUN_000935b0(0.0f, a);
    } else {
        static_cast<Actor *>(obj)->Show(0, 1);
    }
    self->panel->unk5c = obj;
}
