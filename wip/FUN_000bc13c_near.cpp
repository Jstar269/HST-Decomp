/* HSTMATCH module=EBOOT.BIN start=0x000bc13c end=0x000bc2ac symbol=FUN_000bc13c lang=c++ */

extern const char DAT_002d5368[];
extern const void *DAT_003073cc;

struct Tag {
    char c;
};

struct Node {
    void *vtable;
    char pad[4];
    int param2;
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
    char pad2[0x14];
    Vector *vec;
};

extern "C" {
void FUN_000ba3ac(const char *);
void FUN_002b7928(void *, int, int);
void FUN_002b7910(void *, int);
void *FUN_000104b0(int);
void *memcpy(void *, const void *, unsigned int);
void FUN_000be9bc(Vector *, Node **, Tag);

void FUN_000bc13c(Context *self, int param_2, const char *param_3, int param_4, const void *param_5) {
    FUN_000ba3ac(DAT_002d5368);
    FUN_002b7928(self->mutex, 1, 0);
    if (self->vec == 0) {
        self->vec = new Vector();
    }
    Node *node = new Node();
    node->param2 = param_2;
    char *dst = (char *)node + 12;
    int count = 3;
    do {
        char c = *param_3++;
        char d = *param_3++;
        count--;
        *dst++ = c;
        *dst++ = d;
    } while (count > 0);

    node->dataLen = param_4;
    if (param_4 > 0) {
        node->data = FUN_000104b0(param_4);
        memcpy(node->data, param_5, param_4);
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
// Near miss: 92/92 words, 28 differ; first at +0x1c.
// What differs:
// 1. Prologue +0x1c band: MWCC homes s3=a1 before jal 0xba3ac and puts addiu a0,a0,21352 into the jal delay slot,
//    whereas retail homes s3=a1 in the jal delay slot and emits addiu a0 beforehand (documented prologue scheduler tie).
// 2. Register allocation for node across the copy loop: retail loads node into a2 at 0xbc1dc and retains a2 across
//    the loop into the blez delay slot (sw s1, 20(a2)), whereas MWCC spills node to 44(sp) and reloads into v0.
// Ruled out:
// - Register-resident alias for node (loses 3 words, regressing to 89/92).
// - Unrolled array indexing vs pointer increments (*p++ completely matched the inner 6-byte copy loop).
// - vec->size += 1 (emits addiu v0, v0, -1, increasing differ count from 28 to 29).
// near miss from lane dsn5 (unreviewed)
