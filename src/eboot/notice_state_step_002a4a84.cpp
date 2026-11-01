/* HSTMATCH module=EBOOT.BIN start=0x002a4a84 end=0x002a4d08 symbol=FUN_002a4a84 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/*
   Four-state notice driver for a gameplay object.  State lives at +0x5d0 and the optional
   response value at +0x5d4; every transition ends in FUN_002a4f18(object, next_state).
   While a state is active the global device object (DAT_003386c0) has three of its virtuals
   driven: v6(99980.0f), v5(0xffffff00) and v11().  The message shown in states 1 and 2 is the
   same four-way choice (no argument block, short block, long block, or a string owned by the
   caller object) formatted by FUN_00198278 into the buffer at +0xa58 and handed to
   FUN_00115088(..., 2, len, 0, 0).  Note the declared virtual ORDER matters: the three calls
   are vtable slots 5, 6 and 11, and only a real C++ virtual emits retail's jalr t9.
   Matching idioms: the 4th argument of FUN_00198278 is a POINTER type (char *), which is what
   makes MWCC compute it in the jal delay slot instead of hoisting the addiu.
*/

extern "C" {
extern void *DAT_003386c0;
extern int DAT_003415ac;
extern int DAT_0034176c;
extern char DAT_002c0e20[];
extern char DAT_002c0e1c[];
extern char DAT_002c0e18[];
extern char DAT_0030fbfe;

extern int FUN_001155c0(int);
extern void FUN_00103628(int, int);
extern int FUN_001156d4(int);
extern void FUN_002a4f18(int, int);
extern int FUN_0018c9f4(int);
extern int FUN_00198278(int, char *, int, char *);
extern void FUN_00115088(int, int, int, int, int);

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

extern "C" void FUN_002a4a84(int param_1)
{
    int r;
    int v;
    char *msg;
    int u;

    if (*(int *)(param_1 + 0x5d0) != 0) {
        ((Obj *)DAT_003386c0)->v6(99980.0f);
        ((Obj *)DAT_003386c0)->v5(0xffffff00);
        ((Obj *)DAT_003386c0)->v11();
    }
    v = *(int *)(param_1 + 0x5d0);
    if (v == 1) {
        if (FUN_001155c0(DAT_003415ac) != 0) {
            FUN_00103628(DAT_003415ac, 1);
            if (FUN_001156d4(DAT_003415ac) == 0) {
                FUN_002a4f18(param_1, 2);
            } else {
                r = FUN_0018c9f4(0x11);
                if (*(int *)(r + 4) == 0) {
                    msg = DAT_002c0e20;
                } else if (*(int *)(*(int *)(r + 8) + 8) < 0x15) {
                    msg = DAT_002c0e1c;
                } else if (*(int *)(*(int *)(r + 0xc) + 0xf4) < 4) {
                    msg = DAT_002c0e18;
                } else {
                    msg = *(char **)(*(int *)(*(int *)(r + 0xc) + 0xf0) + 0xc);
                }
                u = FUN_00198278(DAT_0034176c, msg, 0x180, (char *)param_1 + 0xa58);
                FUN_00115088(DAT_003415ac, 2, u, 0, 0);
                FUN_002a4f18(param_1, 3);
            }
        }
    } else if (v == 2) {
        if (*(int *)(param_1 + 0x5d4) != 0) {
            if (*(int *)(param_1 + 0x5d4) < 0) {
                r = FUN_0018c9f4(0x11);
                if (*(int *)(r + 4) == 0) {
                    msg = DAT_002c0e20;
                } else if (*(int *)(*(int *)(r + 8) + 8) < 0x15) {
                    msg = DAT_002c0e1c;
                } else if (*(int *)(*(int *)(r + 0xc) + 0xf4) < 4) {
                    msg = DAT_002c0e18;
                } else {
                    msg = *(char **)(*(int *)(*(int *)(r + 0xc) + 0xf0) + 0xc);
                }
                u = FUN_00198278(DAT_0034176c, msg, 0x180, (char *)param_1 + 0xa58);
                FUN_00115088(DAT_003415ac, 2, u, 0, 0);
                FUN_002a4f18(param_1, 3);
            } else {
                DAT_0030fbfe = 1;
                FUN_002a4f18(param_1, 0);
            }
        }
    } else if (v == 3) {
        if (FUN_001155c0(DAT_003415ac) != 0) {
            FUN_00103628(DAT_003415ac, 1);
            FUN_002a4f18(param_1, 0);
        }
    }
}
