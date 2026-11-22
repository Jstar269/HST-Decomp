/* HSTMATCH module=EBOOT.BIN start=0x00286af8 end=0x00286bc4 symbol=FUN_00286af8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

extern "C" char DAT_002fe9dc[];
extern "C" void FUN_00286acc(...);
extern "C" void FUN_00286db0();
extern "C" bool FUN_0014c9b8(void *, int, float);
extern "C" void FUN_00286e08(void *);

struct Sub8 {
    char pad[4];
    int field_4;
};

struct Queue {
    char pad[4];
    int *head;
    char pad2[12];
    int *curr;

    bool IsValid() { return head != 0; }
    bool IsAtStart() { return curr == head + 1; }
    bool HasMore() {
        return !(!IsValid() || IsAtStart());
    }
};

struct Obj {
    char pad_0[8];
    Sub8 *field_8;
    Queue *queue;
    char pad_10[0x46];
    unsigned char field_56;
};

extern "C" bool FUN_00286af8(Obj *self)
{
    if (self->field_8->field_4 == 0) {
        FUN_00286acc(DAT_002fe9dc);
        return false;
    }
    FUN_00286db0();
    bool r = FUN_0014c9b8(self->field_8, 0, 1.0f);
    if (r) {
        FUN_00286e08(self);
    }
    Queue *q = self->queue;
    bool flag = false;
    if (q->head != 0) {
        if (q->HasMore()) {
            flag = true;
        }
    }
    if (flag) {
        self->field_56 = 1;
    }
    return r;
}
