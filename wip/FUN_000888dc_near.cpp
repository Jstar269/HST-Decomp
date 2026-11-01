/* HSTMATCH module=EBOOT.BIN start=0x000888dc end=0x000889ec symbol=FUN_000888dc lang=c++ */

struct Obj_000888dc {
    char pad0[0x3c];
    int field_3c;
    char pad1[0xc];
    bool field_4c;
    char pad2[0xc];
    bool field_59;
    char pad3[0x12];
    int field_6c;
    char pad4[0x2c];
    void *field_9c;
};

extern "C" {
extern unsigned char DAT_003386b8;
extern void *DAT_00338c20;
extern void *DAT_00338b44;

int FUN_000fb7b8(void *);
int FUN_001181c0(void *);
bool FUN_000def58(void *);
}

extern "C" bool FUN_000888dc(Obj_000888dc *self) {
    if (DAT_003386b8 != 0) {
        if (self->field_6c <= 0) {
            return false;
        }
    }

    int bVar2 = 0;
    bool v1;
    if (DAT_003386b8 != 0) {
        if (FUN_000fb7b8(DAT_00338c20) != 0) {
            v1 = false;
            goto LAB_000889c0;
        }
    }

    if (self->field_4c) {
        v1 = (bool)bVar2;
        goto LAB_000889c0;
    }

    if (self->field_3c == 6) {
        bVar2 = (FUN_001181c0(self->field_9c) >= 0) && !FUN_000def58(DAT_00338b44);
    } else {
        void *uVar5;
        if (self->field_3c == 7) {
            uVar5 = self->field_9c;
        } else {
            if (self->field_3c != 5) {
                goto LAB_default;
            }
            uVar5 = self->field_9c;
        }
        bVar2 = FUN_001181c0(uVar5) >= 0;
    }

LAB_default:
    v1 = (bool)bVar2;

LAB_000889c0:
    if (!v1) {
        return v1;
    }
    return !self->field_59;
}
// 36 differing words out of 68; first diff at +0x58.
// Matches perfectly 0x00 to +0x54 (prologue, DAT_003386b8 checks, FUN_000fb7b8 call).
// Differs in dispatch jump table for field_3c cases (6, 7, 5) and tail return conversion.
// Ruled out: switch statement (MWCC reorders cases 5,7,6 or 7,6,5, never 6,7,5); plain if-else without gotos.
// improved by lane dsn5 (unreviewed): 66 -> 36 differing words
