/* HSTMATCH module=EBOOT.BIN start=0x0015f44c end=0x0015f8f4 symbol=FUN_0015f44c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Vec4 {
    float x, y, z, w;
    Vec4() {}
    Vec4(const Vec4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

struct Node {
    virtual void _0() = 0;
    virtual void _1() = 0;
    virtual void _2() = 0;
    virtual bool Method3() = 0;
    virtual bool Method4() = 0;
    virtual void _5(Vec4 *a, Vec4 *b) = 0;
    virtual Vec4 GetTransform() = 0;

    Node *parent;           // 4
    int pad8;               // 8
    unsigned int index;     // 12 (0x0c)
    unsigned char flag;     // 16 (0x10)
    char pad17[3];
    Node **children;        // 20 (0x14)
    unsigned int count;     // 24 (0x18)

    Node *GetChild(unsigned int idx) {
        return (idx < count) ? children[idx] : 0;
    }
};

struct Model {
    char pad[0x30];
    char trackA[16];        // 48 (0x30)
    char trackB[16];        // 64 (0x40)
    float time;             // 80 (0x50)
    char pad84[80];         // 84..164
    unsigned char f164;     // 164 (0xa4)
    unsigned char f165;     // 165 (0xa5)
    char pad166[2];         // 166..168
    Node *root_node;        // 168 (0xa8)
};

struct Pad {
    char pad0[136];
    signed char f88;     // 136 = 0x88
    char pad89[2];
    signed char f8b;     // 139 = 0x8b
};

extern int DAT_00341110;
extern int DAT_00341114;
extern int DAT_00341118;
extern int DAT_0034111c;

extern "C" {
int FUN_0001eae4(Pad *pad, void *data, int mask);
void FUN_00022498(Vec4 *out, void *trackA, float time);
void FUN_00022418(Vec4 *out, void *trackB, Vec4 *in);
void FUN_00160058(Vec4 *out, Model *model);
}

extern "C" int FUN_0015f44c(Model *self, Pad *pad) {
    if (((int)pad->f8b << 24 >> 30) == 1) {
        Vec4 auStack;
        Vec4 local_20;
        Vec4 local_30;
        Vec4 local_10;
        Vec4 local_30_ret;
        Vec4 local_40;

        Node *curr = self->root_node;
        Node *target = 0;

        if (curr->Method3()) {
            do {
                target = curr;
                if (curr->flag != 0) {
                    break;
                }
                curr = curr->GetChild(curr->index);
            } while (curr->Method3());
        }

        if (FUN_0001eae4(pad, &DAT_00341110, 16)) {
            unsigned int uVar6 = target->count;
            if (uVar6 != 0) {
                target->index = ((target->index + uVar6) - 1) % uVar6;
            }
        }

        if (FUN_0001eae4(pad, &DAT_00341114, 64)) {
            if (target->count != 0) {
                target->index = (target->index + 1U) % (unsigned int)target->count;
            }
        }

        if (FUN_0001eae4(pad, &DAT_00341118, 128)) {
            curr = self->root_node;
            Node *t2 = 0;
            if (curr->Method3()) {
                do {
                    t2 = curr;
                    if (curr->flag != 0) {
                        break;
                    }
                    curr = curr->GetChild(curr->index);
                } while (curr->Method3());
            }
            if (t2 != 0 && t2->parent != 0 && t2->parent->Method3()) {
                t2->parent->flag = 1;
                t2->flag = 0;
            }
        }

        if (FUN_0001eae4(pad, &DAT_0034111c, 32)) {
            curr = self->root_node;
            Node *t2 = 0;
            if (curr->Method3()) {
                do {
                    t2 = curr;
                    if (curr->flag != 0) {
                        break;
                    }
                    curr = curr->GetChild(curr->index);
                } while (curr->Method3());
            }
            if (t2 != 0) {
                curr = t2->GetChild(t2->index);
                if (curr->Method3()) {
                    t2->flag = 0;
                    curr->flag = 1;
                }
            }
        }

        if (((int)pad->f8b << 30 >> 30) == 0) {
            Node *child = target->GetChild(target->index);
            if (child != 0) {
                local_20 = *(Vec4 *)self->trackB;
                FUN_00022498(&local_40, self->trackA, self->time);
                local_10 = local_40;
                FUN_00022418(&local_30_ret, self->trackB, &local_10);
                local_30 = local_30_ret;
                Node *t = target->GetChild(target->index);
                t->_5(&local_20, &local_30);
            }
        }

        int c1 = ((int)pad->f88 << 24 >> 30);
        int s0 = (c1 == 0);
        bool bVar2 = true;
        if (c1 != 1) {
            if (s0) {
                goto LAB_check2;
            }
            bVar2 = false;
        }

    LAB_check2:
        int c2 = ((int)pad->f8b << 26 >> 30);
        int s4 = (c2 == 0);
        bool bVar3 = true;
        if (c2 != 1 && !s4) {
            bVar3 = false;
        }

        if (bVar2 || bVar3) {
            FUN_00160058(&auStack, self);
            if (bVar2) {
                if (s0) {
                    self->f165 ^= 1;
                    if (self->f165 != 0) {
                        self->f164 = 0;
                    }
                }
            } else if (bVar3 && s4) {
                self->f164 ^= 1;
                if (self->f164 != 0) {
                    self->f165 = 0;
                }
            }
            return 1;
        }
    }

    return 0;
}
