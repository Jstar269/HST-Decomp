/* HSTMATCH module=EBOOT.BIN start=0x00288828 end=0x002889cc symbol=FUN_00288828 lang=c++ helpers=FUN_00286acc@0x00286acc-0x00286af8 */
/* Hand-written reconstruction from the disassembly (lane fm8 near miss wip/create_npc_from_pc_00288828.cpp, finished by lane pn4); exact match verified; the empty variadic log stub FUN_00286acc is a same-file static helper placed by helpers= (it fixes the v1/a0 colouring of the table test); names and types await human review. */
struct Game { char pad[0x4c]; int *npc; };
extern "C" Game *DAT_0034a114;
extern "C" char DAT_002feac8[];
extern "C" char DAT_002feaa0[];
extern "C" char DAT_002fea64[];
extern "C" char DAT_002fe9c8[];
extern "C" char DAT_002fea94[];
extern "C" int FUN_00000bcc(unsigned);
extern "C" int FUN_001268d8(int);
extern "C" void FUN_001c6908(int, int, void *);
extern "C" void FUN_000143b0(void *, char *, int, int);
extern "C" void FUN_0014c028(int, int);
extern "C" int FUN_00155f34(int, void *);
extern "C" int FUN_00014a40(void *);
extern "C" int FUN_00000a90(unsigned);
extern "C" void FUN_000149a8(int, void *);
extern "C" int FUN_0014c274(int, char *);
extern "C" void FUN_000943d0(int, int);
extern "C" void FUN_002869cc(int, char *, int);

extern "C" {
static void FUN_00286acc(...) { }
}

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void start(int, int, int, int);
};

extern "C" void FUN_00288828(int param_1, int param_2, int param_3)
{
    char buf[256];
    int p;
    int q;

    if (DAT_0034a114->npc[param_1] != 0) {
        FUN_00286acc(DAT_002feaa0, param_1, param_2);
        return;
    }
    {
        FUN_00286acc(DAT_002feac8, param_1, param_2);
        p = FUN_00000bcc(0x420);
        if (p != 0)
            p = FUN_001268d8(p);
        *(int *)(p + 0x110) = param_1;
        *(int *)(p + 0x114) = param_3;
        *(int *)(p + 0x118) = param_2;
        ((Sub *)(p + 0xe0))->start(2, param_2, 0, 0);
        q = p;
        if (p != 0)
            q = p + 0xe0;
        FUN_001c6908((int)DAT_0034a114, param_1, (void *)q);
        FUN_000143b0(buf, DAT_002fea64, *(int *)(DAT_0034a114->pad + 0xcc) + 1, param_1);
        if (*(int *)(p + 0xe4) != 0) {
            FUN_0014c028(*(int *)(p + 0xe4), 1);
            *(int *)(p + 0xe4) = 0;
        }
        *(int *)(p + 0xe4) = FUN_00155f34(*(int *)(DAT_0034a114->pad + 0x3c), buf);
        q = FUN_00014a40(buf);
        *(int *)(p + 0xf0) = FUN_00000a90(q + 1);
        FUN_000149a8(*(int *)(p + 0xf0), buf);
        if (*(int *)(p + 0xe4) != 0)
            *(int *)(p + 0x11c) = FUN_0014c274(*(int *)(p + 0xe4), DAT_002fe9c8);
        FUN_000943d0(p, 5);
        FUN_002869cc(p + 0xe0, DAT_002fea94, 0);
    }
}
