/* HSTMATCH module=EBOOT.BIN start=0x000bc2ac end=0x000bc434 symbol=FUN_000bc2ac lang=c++ helpers=FUN_000ba3ac@0x000ba3ac-0x000ba3d8 */
/* Hand-written reconstruction from the disassembly (restructured from the near miss wip/FUN_000bc2ac_near.cpp; the empty variadic log stub 0xba3ac is a same-file static helper placed by helpers=); exact match verified; matching idioms: the 6-char name is a user-ctor struct assigned whole (rolled lb/sb pair loop, node slot not reloaded) and the push is `v->data[Inc(v)] = node` with an inline Inc that does `size += 1; return size - 1;`; names and types await human review. */

extern const char DAT_002d5378[];
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
    int param2;
    int param3;
    Name6 name;
    char pad[2];
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
    char pad2[0x10];
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

void FUN_000bc2ac(Context *self, int param_2, int param_3, const Name6 *param_4, int param_5, const void *param_6) {
    FUN_000ba3ac(DAT_002d5378, param_3);
    FUN_002b7928(self->mutex, 1, 0);
    if (self->vec == 0) {
        self->vec = new Vector();
    }
    Node *node = new Node();
    node->param2 = param_2;
    node->param3 = param_3;
    node->name = *param_4;

    node->dataLen = param_5;
    if (param_5 > 0) {
        node->data = FUN_000104b0(param_5);
        memcpy(node->data, param_6, param_5);
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
