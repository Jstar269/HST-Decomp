/* HSTMATCH module=EBOOT.BIN start=0x001caee4 end=0x001cb204 symbol=FUN_001caee4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. The node lists are 16-byte holders (an empty allocator slot in front of begin/count/cap) and are released with explicit calls on each exit path, as in retail. */

struct Vec4 {
    float x, y, z, w;
} __attribute__((aligned(16)));

struct Quat {
    float x, y, z, w;
};

struct Node {
    int id;
    char pad4[0x30 - 4];
    Quat rot;
    Vec4 pos;
};

struct NodeList {
    Node *begin;
    int count;
    int cap;
};

struct Scene {
    char pad[0x9ec];
    Node *nodes;
    int count;
};

struct Obj {
    char pad[0xa0];
    Vec4 posA;
    Vec4 posB;
    char padc0[0xd0 - 0xc0];
    int fd0;
};

struct Pred {};

struct Alloc {};

struct Holder {
    Alloc a;
    NodeList l;
};

extern Scene *DAT_0033b574;
extern Quat DAT_002bc430;

extern "C" {
void FUN_00022418(Vec4 *out, const Vec4 *v, const Quat *q);
void FUN_001ce440(NodeList *out, Node *first, Node *last, Pred p);
void FUN_001474d0(NodeList *v, int f);
}

static inline bool Same(Node *a, Node *b) { return a == b; }
static inline bool Differ(Node *a, Node *b) { return !Same(a, b); }

static inline void Set(Vec4 *d, Vec4 v) { *d = v; }

extern "C" int FUN_001caee4(Obj *o, int id, Vec4 *pos, Vec4 *out)
{
    Holder all;
    Holder near;
    Pred p1;
    Pred p2;
    Vec4 t1;
    Vec4 t2;
    Vec4 t3;
    Vec4 t4;
    if (id == 1000) {
        *pos = o->posA;
        FUN_00022418(&t1, pos, &DAT_002bc430);
        Set(out, t1);
        return 1;
    }
    if (id == 1001) {
        *pos = o->posB;
        FUN_00022418(&t2, pos, &DAT_002bc430);
        Set(out, t2);
        return 1;
    }
    all.l.begin = 0;
    all.l.count = 0;
    all.l.cap = 0;
    FUN_001ce440(&all.l, DAT_0033b574->nodes, DAT_0033b574->nodes + DAT_0033b574->count, p1);
    {
        Node *e = all.l.begin;
        Node *end = all.l.begin + all.l.count;
        for (; Differ(e, end); e++) {
            if (e->id == id) {
                *pos = e->pos;
                FUN_00022418(&t3, pos, &e->rot);
                Set(out, t3);
                FUN_001474d0(&all.l, 0);
                return 1;
            }
        }
    }
    if (-10 < id && id < 1) {
        int key = -o->fd0;
        near.l.begin = 0;
        near.l.count = 0;
        near.l.cap = 0;
        FUN_001ce440(&near.l, DAT_0033b574->nodes, DAT_0033b574->nodes + DAT_0033b574->count, p2);
        Node *e = near.l.begin;
        Node *end = near.l.begin + near.l.count;
        for (; Differ(e, end); e++) {
            if (e->id == key) {
                *pos = e->pos;
                FUN_00022418(&t4, pos, &e->rot);
                Set(out, t4);
                FUN_001474d0(&near.l, 0);
                FUN_001474d0(&all.l, 0);
                return 1;
            }
        }
        FUN_001474d0(&near.l, 0);
    }
    FUN_001474d0(&all.l, 0);
    return 0;
}
