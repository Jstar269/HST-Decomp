/* HSTMATCH module=EBOOT.BIN start=0x00067688 end=0x00067730 symbol=FUN_00067688 lang=c++ */
/* Hand-written reconstruction from the disassembly (lane om1 near miss wip/clear_list_00067688.cpp, finished by lane pn4: the list object itself, &self->f3c, is FUN_000679cc's second argument and the walk advances by p->next only in the else arm); exact match verified; names and types await human review. */
/* Walks a list of 12-byte nodes: calls value slot 1 with the float at +0x48 and, when the value's byte at +0x10 is
   set, destroys the value (slot 0 with 1) and erases the node through FUN_000679cc, which returns the next node. */
struct Value {
    virtual void slot0(int n);
    virtual void slot1(float f);
    char pad[12];
    unsigned char f10;
};

struct Node {
    void *vptr;
    Node *next;
    Value *value;
};

struct Obj {
    char pad[0x3c];
    void *f3c;
    Node *sentinel;
    Node *head;
    float f48;
};

extern "C" void FUN_000679cc(int *out, void *list, int *p);

extern "C" void FUN_00067688(Obj *self)
{
    Value *v;
    Node *p = self->head;
    Node *end = (Node *)&self->sentinel;
    while (p != end) {
        v = p->value;
        v->slot1(self->f48);
        if (v->f10 != 0) {
            if (v != 0) {
                v->slot0(1);
            }
            int out[2];
            int c = (int)p;
            FUN_000679cc(out, &self->f3c, &c);
            p = (Node *)out[0];
        } else {
            p = p->next;
        }
    }
}
