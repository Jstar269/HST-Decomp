/* HSTMATCH module=EBOOT.BIN start=0x000bc13c end=0x000bc2ac symbol=FUN_000bc13c lang=c++ helpers=FUN_000ba3ac@0x000ba3ac-0x000ba3d8 */

extern const char DAT_002d5368[];
extern const void *DAT_003073cc;

struct Tag {
    char c;
};

struct Name6 {
    char c[6];
    Name6() {}
};

struct Node {
    void *vtable;
    char pad[4];
    int param2;
    Name6 name;
    char pad2[2];
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

static inline unsigned Inc(Vector *v) { v->size += 1; return v->size - 1; }
struct Context {
    char pad[0x34];
    void *mutex;
    char pad2[0x14];
    Vector *vec;
};

extern "C" {
static void FUN_000ba3ac(...)
{
}
void FUN_002b7928(void *, int, int);
void FUN_002b7910(void *, int);
void *FUN_000104b0(int);
void *memcpy(void *, const void *, unsigned int);
void FUN_000be9bc(Vector *, Node **, Tag);

void FUN_000bc13c(Context *self, int param_2, const Name6 *param_3, int param_4, const void *param_5) {
    FUN_000ba3ac(DAT_002d5368);
    FUN_002b7928(self->mutex, 1, 0);
    if (self->vec == 0) {
        self->vec = new Vector();
    }
    Node *node = new Node();
    node->param2 = param_2;
    node->name = *param_3;

    node->dataLen = param_4;
    if (param_4 > 0) {
        node->data = FUN_000104b0(param_4);
        memcpy(node->data, param_5, param_4);
    }

    Vector *vec = self->vec;
    if (vec->size < vec->capacity) {
        vec->data[Inc(vec)] = node;
    } else {
        FUN_000be9bc(vec, &node, Tag());
    }
    FUN_002b7910(self->mutex, 1);
}
}

// near miss api-hstm-20261121: 92/92 words, 4 differ (was 28): only the order of the parameter copies at entry is off (retail
// `move s2,a2; move s1,a3; move s0,t0` then `move s3,a1` in the jal slot; ours a1 first, `move s0,t0` in the slot). Sibling 0xbc2ac
// is EXACT with the same tricks: the empty log stub 0xba3ac as a same-file static helper (helpers=), the 6-char name as a user-ctor
// struct assigned whole (`node->name = *p`: rolled lb/sb pairs and no reload of the address-taken node), and the push as
// `v->data[Inc(v)] = node` with `Inc` doing `size += 1; return size - 1;`. Tried for the copy order: local copy of param_2 before/after
// the call, renamed parameter assigned late, unsigned/void*/long type, a by-value one-int struct parameter (12 differ).
// improved by lane api-hstm-20261121 (unreviewed): 28 -> 4 differing words
