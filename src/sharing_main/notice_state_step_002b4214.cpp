/* HSTMATCH module=hstp_trans_main.prx start=0x002b4214 end=0x002b4498 symbol=FUN_002b4214 lang=c++ */
/* Hand-written reconstruction: clone of the source of notice_state_step_002a4a84.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Four-state notice driver for a gameplay object.  State lives at +0x5d0 and the optional
   response value at +0x5d4; every transition ends in FUN_002b46a8(object, next_state).
   While a state is active the global device object (DAT_003e1a00) has three of its virtuals
   driven: v6(99980.0f), v5(0xffffff00) and v11().  The message shown in states 1 and 2 is the
   same four-way choice (no argument block, short block, long block, or a string owned by the
   caller object) formatted by FUN_001a4bcc into the buffer at +0xa58 and handed to
   FUN_0011fe78(..., 2, len, 0, 0).  Note the declared virtual ORDER matters: the three calls
   are vtable slots 5, 6 and 11, and only a real C++ virtual emits retail's jalr t9.
   Matching idioms: the 4th argument of FUN_001a4bcc is a POINTER type (char *), which is what
   makes MWCC compute it in the jal delay slot instead of hoisting the addiu.
*/

extern "C" {
extern void *DAT_003e1a00;
extern int DAT_003ea8ec;
extern int DAT_003eaaac;
extern char DAT_002d10d4[];
extern char DAT_002d10d0[];
extern char DAT_002d10cc[];
extern char DAT_003b8ec6;

extern int FUN_001204dc(int);
extern void FUN_00108f54(int, int);
extern int FUN_001205f0(int);
extern void FUN_002b46a8(int, int);
extern int FUN_001991d4(int);
extern int FUN_001a4bcc(int, char *, int, char *);
extern void FUN_0011fe78(int, int, int, int, int);

}

struct Obj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5(int);
    virtual void v6(float);
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
};

extern "C" void FUN_002b4214(int param_1)
{
    int r;
    int v;
    char *msg;
    int u;

    if (*(int *)(param_1 + 0x5d0) != 0) {
        ((Obj *)DAT_003e1a00)->v6(99980.0f);
        ((Obj *)DAT_003e1a00)->v5(0xffffff00);
        ((Obj *)DAT_003e1a00)->v11();
    }
    v = *(int *)(param_1 + 0x5d0);
    if (v == 1) {
        if (FUN_001204dc(DAT_003ea8ec) != 0) {
            FUN_00108f54(DAT_003ea8ec, 1);
            if (FUN_001205f0(DAT_003ea8ec) == 0) {
                FUN_002b46a8(param_1, 2);
            } else {
                r = FUN_001991d4(0x11);
                if (*(int *)(r + 4) == 0) {
                    msg = DAT_002d10d4;
                } else if (*(int *)(*(int *)(r + 8) + 8) < 0x15) {
                    msg = DAT_002d10d0;
                } else if (*(int *)(*(int *)(r + 0xc) + 0xf4) < 4) {
                    msg = DAT_002d10cc;
                } else {
                    msg = *(char **)(*(int *)(*(int *)(r + 0xc) + 0xf0) + 0xc);
                }
                u = FUN_001a4bcc(DAT_003eaaac, msg, 0x180, (char *)param_1 + 0xa58);
                FUN_0011fe78(DAT_003ea8ec, 2, u, 0, 0);
                FUN_002b46a8(param_1, 3);
            }
        }
    } else if (v == 2) {
        if (*(int *)(param_1 + 0x5d4) != 0) {
            if (*(int *)(param_1 + 0x5d4) < 0) {
                r = FUN_001991d4(0x11);
                if (*(int *)(r + 4) == 0) {
                    msg = DAT_002d10d4;
                } else if (*(int *)(*(int *)(r + 8) + 8) < 0x15) {
                    msg = DAT_002d10d0;
                } else if (*(int *)(*(int *)(r + 0xc) + 0xf4) < 4) {
                    msg = DAT_002d10cc;
                } else {
                    msg = *(char **)(*(int *)(*(int *)(r + 0xc) + 0xf0) + 0xc);
                }
                u = FUN_001a4bcc(DAT_003eaaac, msg, 0x180, (char *)param_1 + 0xa58);
                FUN_0011fe78(DAT_003ea8ec, 2, u, 0, 0);
                FUN_002b46a8(param_1, 3);
            } else {
                DAT_003b8ec6 = 1;
                FUN_002b46a8(param_1, 0);
            }
        }
    } else if (v == 3) {
        if (FUN_001204dc(DAT_003ea8ec) != 0) {
            FUN_00108f54(DAT_003ea8ec, 1);
            FUN_002b46a8(param_1, 0);
        }
    }
}
