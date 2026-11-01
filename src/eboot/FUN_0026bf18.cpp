/* HSTMATCH module=EBOOT.BIN start=0x0026bf18 end=0x0026c470 symbol=FUN_0026bf18 lang=c++ rodata=0x002ca930 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

#pragma exceptions off

typedef unsigned char u8;
typedef unsigned int u32;

class SubBase {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual int slot10();
};

class Sub1 : public SubBase {
public:
    u8 pad[264];
};

class Sub2 : public SubBase {
public:
    u8 pad[1500];
};

class Sub3 : public SubBase {
public:
    u8 pad[968];
};

class Sub4 : public SubBase {
public:
    u8 pad[136];
};

class Sub5 : public SubBase {
public:
    u8 pad[108];
};

class MainObj {
public:
    int *sub0;          // 0
    Sub1 sub1;          // 4
    Sub2 sub2;          // 272
    Sub3 sub3;          // 1776
    Sub4 sub4;          // 2748
    Sub5 sub5;          // 2888
    void *active_sub;   // 3000
    u8 byte_3004;       // 3004
    u8 pad3005[11];     // 3005
    int state;          // 3016
    int prev_state;     // 3020
    int field_3024;     // 3024
    int field_3028;     // 3028
    u8 byte_3032;       // 3032
};

extern "C" int DAT_002cfc8c;
extern "C" u8 DAT_003417be;
extern "C" int DAT_0034a120;

extern "C" void FUN_0001dfe8(int, int);
extern "C" void FUN_0001e23c(int, int, int);
extern "C" void FUN_00103628(void *, ...);
extern "C" void FUN_0025d580(void *);
extern "C" void FUN_0025f1c4(void *);
extern "C" void FUN_0025f298(void *);
extern "C" void FUN_001d9c64(int);
extern "C" int FUN_001d9cc4(int);
extern "C" void FUN_002a1a64(int, int);
extern "C" void FUN_0026bf18(MainObj *self, int new_state);

void FUN_0026bf18(MainObj *self, int new_state) {
    FUN_0001dfe8(DAT_002cfc8c, 0x2000);
    FUN_0001e23c(DAT_002cfc8c, 0x2000, 1);
    FUN_0001dfe8(DAT_002cfc8c, 0x4000);
    FUN_0001e23c(DAT_002cfc8c, 0x4000, 1);

    self->prev_state = self->state;
    self->state = new_state;
    self->field_3024 = 0;
    self->field_3028 = 40;

    switch (self->state) {
    default:
        DAT_003417be = 0;
        self->byte_3004 = 0;
        self->state = 0;
        FUN_00103628(&self->sub1, 1);
        FUN_00103628(&self->sub2, 1);
        FUN_0025d580(&self->sub3);
        FUN_00103628(&self->sub4, 1);
        FUN_00103628(&self->sub5, 1);
        break;

    case 1:
        DAT_003417be = 0;
        self->sub1.slot6();
        self->sub2.slot7();
        self->sub3.slot7();
        self->sub4.slot7();
        self->sub5.slot7();
        self->active_sub = &self->sub1;
        break;

    case 2:
        self->sub1.slot7();
        if (self->prev_state == 4) {
            FUN_0025f1c4(&self->sub2);
        } else if (self->prev_state == 5) {
            FUN_0025f298(&self->sub2);
        } else {
            self->sub2.slot6();
        }
        self->sub3.slot7();
        self->sub4.slot7();
        self->sub5.slot7();
        self->active_sub = &self->sub2;
        break;

    case 3:
        self->sub1.slot7();
        self->sub2.slot7();
        self->sub3.slot6();
        self->sub4.slot7();
        self->sub5.slot7();
        self->active_sub = &self->sub3;
        break;

    case 4:
        self->sub1.slot7();
        self->sub2.slot7();
        self->sub3.slot7();
        self->sub4.slot6();
        self->sub5.slot7();
        self->active_sub = &self->sub4;
        break;

    case 5:
        self->sub1.slot7();
        self->sub2.slot7();
        self->sub3.slot7();
        self->sub4.slot7();
        self->sub5.slot6();
        self->active_sub = &self->sub5;
        break;

    case 6: {
        self->byte_3032 = 0;
        bool flag = true;
        switch (self->prev_state) {
        case 1:
            if (*(int *)((char *)self + 96) == 4) {
                switch (self->sub1.slot10()) {
                case 0:
                    break;
                case 1:
                    FUN_001d9c64(DAT_0034a120);
                    break;
                case 2:
                    FUN_001d9c64(DAT_0034a120);
                    flag = false;
                    break;
                default:
                    break;
                }
            } else if (*(int *)((char *)self + 96) == 10) {
                FUN_001d9c64(DAT_0034a120);
            } else {
                switch (self->sub1.slot10()) {
                case 0:
                    FUN_00103628(&self->sub1, 1);
                    FUN_00103628(&self->sub2, 1);
                    FUN_0025d580(&self->sub3);
                    FUN_00103628(&self->sub4, 1);
                    FUN_00103628(&self->sub5, 1);
                    FUN_0026bf18(self, 2);
                    break;
                case 1:
                    self->byte_3032 = 1;
                    break;
                case 2:
                    FUN_0026bf18(self, 5);
                    break;
                }
            }
            break;

        case 2:
            if (self->sub0[23] == 1) {
                FUN_0026bf18(self, 4);
            } else {
                FUN_001d9c64(DAT_0034a120);
            }
            flag = false;
            break;

        case 3:
        case 4:
            FUN_001d9c64(DAT_0034a120);
            flag = false;
            break;

        case 5:
            FUN_00103628(&self->sub1);
            FUN_00103628(&self->sub2, 1);
            FUN_0025d580(&self->sub3);
            FUN_00103628(&self->sub4, 1);
            FUN_00103628(&self->sub5, 1);
            self->state = 5;
            FUN_0026bf18(self, 2);
            break;

        case 0:
        case 6:
        default:
            FUN_00103628(&self->sub1, 1);
            FUN_00103628(&self->sub2, 1);
            FUN_0025d580(&self->sub3);
            FUN_00103628(&self->sub4, 1);
            FUN_00103628(&self->sub5, 1);
            break;

        case 7:
            break;
        }

        if (flag && self->state == 6) {
            int uVar2 = FUN_001d9cc4(DAT_0034a120);
            FUN_002a1a64(uVar2, 0x18);
        }
        break;
    }
    }
}
