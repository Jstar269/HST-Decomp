/* HSTMATCH module=EBOOT.BIN start=0x0014e00c end=0x0014e11c symbol=FUN_0014e00c lang=c++ */
extern char DAT_003082d4[];
extern char DAT_0030840c[];

struct VC { virtual ~VC(); };

struct Node {
    int f0;
    Node *next;
    VC *obj;
};

struct List {
    int f0;
    Node sent;
};

struct Self {
    void *vptr;
    int f4;
    List list;
};

struct It { Node *p; };
struct Tag {};

extern "C" void FUN_0014f540(It *out, List *l, It *first, It *last);

extern "C" Self *FUN_0014e00c(Self *self, short flag, Tag)
{
    if (self != 0) {
        self->vptr = DAT_003082d4;
        Node *end = &self->list.sent;
        for (Node *n = self->list.sent.next; n != end; n = n->next) {
            if (n->obj != 0)
                delete n->obj;
        }
        {
            It out;
            It first;
            It last;
            last.p = &self->list.sent;
            first.p = self->list.sent.next;
            FUN_0014f540(&out, &self->list, &first, &last);
        }
        if (&self->list != 0 && &self->list != 0 && &self->list != 0) {
            It out;
            It first;
            It last;
            last.p = &self->list.sent;
            first.p = self->list.sent.next;
            FUN_0014f540(&out, &self->list, &first, &last);
        }
        if (self != 0)
            self->vptr = DAT_0030840c;
        if (flag > 0)
            delete self;
    }
    return self;
}
// api-hste1 NEAR 57 differ (was 67): typed intrusive-list destructor (Node{f0,next,obj}, sentinel at +12, list at +8, vtable-slot-0 `delete n->obj` in the walk without clearing the
// field, two FUN_0014f540(&out,&list,&first,&last) erases with iterator temps, three nested `&self->list != 0` tests before the second, `if (self) self->vptr = DAT_0030840c`,
// `if (flag > 0) delete self`). Shape and colours match; the frame is 64 vs retail 80 (retail iterator temps out/first/last at sp+64/60/56 and 52/48/44 with 12 more unused
// bytes above them); an extra trailing empty Tag parameter did not change the frame. Walk loop colours s2/s3 are swapped (retail s2 = node, s3 = end sentinel).
// improved by lane api-hste1-20261121 (unreviewed): 67 -> 57 differing words
