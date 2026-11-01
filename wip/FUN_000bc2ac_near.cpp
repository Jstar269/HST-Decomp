/* HSTMATCH module=EBOOT.BIN start=0x000bc2ac end=0x000bc434 symbol=FUN_000bc2ac lang=c++ */

extern const char DAT_002d5378[];
extern const void *DAT_003073cc;

struct Tag {
    char c;
};

struct Node {
    void *vtable;
    int param2;
    int param3;
    char name[8];
    int dataLen;
    void *data;
    Node() {
        vtable = (void *)&DAT_003073cc;
        data = 0;
    }
};

struct Vector {
    Node **data;
    unsigned int size;
    unsigned int capacity;
    Vector() {
        data = 0;
        size = 0;
        capacity = 0;
    }
};

struct Context {
    char pad[0x34];
    void *mutex;
    char pad2[0x10];
    Vector *vec;
};

extern "C" {
void FUN_000ba3ac(const char *, ...);
void FUN_002b7928(void *, int, int);
void FUN_002b7910(void *, int);
void *FUN_000104b0(int);
void *memcpy(void *, const void *, unsigned int);
void FUN_000be9bc(Vector *, Node **, Tag);

void FUN_000bc2ac(Context *self, int param_2, int param_3, const char *param_4, int param_5, const void *param_6) {
    FUN_000ba3ac(DAT_002d5378, param_3);
    FUN_002b7928(self->mutex, 1, 0);
    if (self->vec == 0) {
        self->vec = new Vector();
    }
    Node *node = new Node();
    node->param2 = param_2;
    node->param3 = param_3;
    char *dst = (char *)node + 12;
    int count = 3;
    do {
        char c = *param_4++;
        char d = *param_4++;
        count--;
        *dst++ = c;
        *dst++ = d;
    } while (count > 0);

    node->dataLen = param_5;
    if (param_5 > 0) {
        node->data = FUN_000104b0(param_5);
        memcpy(node->data, param_6, param_5);
    }

    Vector *vec = self->vec;
    if (vec->size < vec->capacity) {
        int idx = vec->size++;
        vec->data[idx] = node;
    } else {
        FUN_000be9bc(vec, &node, Tag());
    }
    FUN_002b7910(self->mutex, 1);
}
}
// Near miss: 98/98 words, 31 differ; first at +0x20.
// What differs:
// 1. Prologue +0x20 band: MWCC homes caller arguments and emits addiu a0,a0,21368 before jal 0xba3ac,
//    putting move a1, s3 into the jal delay slot, whereas retail puts move s0, t1 in the delay slot (MWCC-NOTES line 4160 schedule tie).
// 2. Caller-saved register allocation for node across the copy loop: retail keeps node in a2 across the loop,
//    using it directly in the blez delay slot (sw s1, 20(a2)), whereas MWCC reloads node from 44(sp) into v0.
// Ruled out:
// - Register-resident alias for node (loses 3 words, identical to 0x000bc13c).
// - Interleaved byte copy (unrolls into 101 words; *p++ read-pair-then-store-pair exactly matched retail's rolled loop).
// near miss from lane dsn5 (unreviewed)
