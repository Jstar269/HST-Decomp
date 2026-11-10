/* HSTMATCH module=EBOOT.BIN start=0x000d0a9c end=0x000d0cfc symbol=FUN_000d0a9c lang=c++ */
/* Hand-written reconstruction from the disassembly (finishing the lane ag4 near miss in wip/); exact match verified; names and types await human review. */

typedef unsigned char u8;

struct Vec4 {
    float x, y, z, w;
};

struct Matrix44 {
    float m[16];
};

struct ChildItem {
    float f10c;
    char pad[52 - 4];
};

struct Child {
    char pad[0x30];
    Matrix44 mat;
    char pad2[0x10c - 0x70];
    ChildItem items[1];
    char pad3[0x158 - (0x10c + 52)];
    int start;
    int end;
    bool IsActive() const
    {
        for (int i = start; i <= end; i++) {
            if (items[i].f10c != 0.0f)
                return true;
        }
        return false;
    }
};

struct Obj {
    char pad[0x3c];
    Child *child;
    char pad2[0x98 - 0x40];
    void *f98;
    void *f9c;
    Vec4 pos;
    u8 fb0;
};

extern "C" {
extern int DAT_002d5ab0;
extern char *DAT_003313c4;

void FUN_00022434(Vec4 *out, void *base, Vec4 *in);
void FUN_00022518(Vec4 *out, Vec4 *in);
void FUN_00022498(float angle, Vec4 *out, Vec4 *in);
void FUN_00022418(Vec4 *out, Vec4 *a, Vec4 *b);
void FUN_00055044(Child *obj, Matrix44 *mat);
void FUN_0005f1e8(void *obj, void *arg);
}

extern "C" void FUN_000d0a9c(Obj *self) {
    if ((DAT_002d5ab0 & 0x1000) != 0) {
        Child *child = self->child;
        if (child != 0) {
            Vec4 in_pos;
            Matrix44 mat;
            Vec4 t6;
            Vec4 t4;
            Vec4 t2;
            Vec4 t7;
            Vec4 t1;
            Vec4 t3;
            Vec4 t5;

            in_pos = self->pos;
            FUN_00022434(&t1, DAT_003313c4 + 0x1d0, &in_pos);
            t2 = t1;
            FUN_00022518(&t3, &t2);
            t3.w = 0.0f;
            t4 = t3;
            FUN_00022498(0.07f, &t5, &t4);
            t6 = t5;
            FUN_00022418(&t7, &t6, &in_pos);
            in_pos = t7;

            child = self->child;
            if (child != 0) {
                mat.m[0] = child->mat.m[0];
                mat.m[1] = child->mat.m[1];
                mat.m[2] = child->mat.m[2];
                mat.m[3] = child->mat.m[3];
                mat.m[4] = child->mat.m[4];
                mat.m[5] = child->mat.m[5];
                mat.m[6] = child->mat.m[6];
                mat.m[7] = child->mat.m[7];
                mat.m[8] = child->mat.m[8];
                mat.m[9] = child->mat.m[9];
                mat.m[10] = child->mat.m[10];
                mat.m[11] = child->mat.m[11];
                mat.m[12] = child->mat.m[12];
                mat.m[13] = child->mat.m[13];
                mat.m[14] = child->mat.m[14];
                mat.m[15] = child->mat.m[15];
                mat.m[12] = in_pos.x;
                mat.m[13] = in_pos.y;
                mat.m[14] = in_pos.z;
                mat.m[15] = in_pos.w;
                FUN_00055044(child, &mat);
            }
        }
    }

    Child *child = self->child;
    if (child != 0) {
        void *arg = self->f98;
        if (child->IsActive())
            FUN_0005f1e8(child, arg);
    }

    if (self->fb0 != 0) {
        FUN_0005f1e8(self->f9c, 0);
    }
}
