/* HSTMATCH module=hstp_trans_main.prx start=0x0014cd64 end=0x0014cee8 symbol=FUN_0014cd64 lang=c++ */
/* Clone of src/eboot/load_item_table_00141718.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Tag {
    char c;
};

typedef struct {
    int id;
    int pad[3];
    float v1[4];
    float v2[4];
} __attribute__((aligned(16))) Item;

struct Vector {
    Item *data;
    unsigned int size;
    unsigned int capacity;
};

struct Context {
    char pad[0x8e0];
    Vector vec;
    char path[256];
};

extern const char DAT_00382d98[];
extern "C" {
void FUN_000143b0(char *dst, const char *fmt, ...);
void *FUN_0004cd8c(const char *path, int *outSize, int align, int mode);
void FUN_00152c5c(Vector *vec, unsigned int count, Tag tag);
void FUN_00152c18(Vector *vec, const Item *item, Tag tag);
void FUN_000104e0(void *ptr);

void FUN_0014cd64(Context *self, const char *arg2) {
    FUN_000143b0(self->path, DAT_00382d98, arg2);
    int fileSize = 0;
    int *fileData = (int *)FUN_0004cd8c(self->path, &fileSize, 16, 1);
    if (fileData != 0) {
        FUN_00152c5c(&self->vec, self->vec.size, Tag());
        int count = fileData[0];
        int *src = fileData + 1;
        int i = 0;
        if (count > 0) {
            do {
                Item item;
                item.id = src[0];
                item.v1[0] = ((float *)src)[1];
                item.v1[1] = ((float *)src)[2];
                item.v1[2] = ((float *)src)[3];
                item.v1[3] = 1.0f;
                item.v2[0] = ((float *)src)[4];
                item.v2[1] = ((float *)src)[5];
                item.v2[2] = ((float *)src)[6];
                src += 7;

                if (self->vec.size < self->vec.capacity) {
                    Item *dest = &self->vec.data[self->vec.size];
                    if (dest != 0) {
                        dest->id = item.id;
                        dest->v1[0] = item.v1[0];
                        dest->v1[1] = item.v1[1];
                        dest->v1[2] = item.v1[2];
                        dest->v1[3] = item.v1[3];
                        dest->v2[0] = item.v2[0];
                        dest->v2[1] = item.v2[1];
                        dest->v2[2] = item.v2[2];
                    }
                    self->vec.size++;
                } else {
                    FUN_00152c18(&self->vec, &item, Tag());
                }
                i++;
            } while (i < count);
        }
        FUN_000104e0(fileData);
    }
}
}
