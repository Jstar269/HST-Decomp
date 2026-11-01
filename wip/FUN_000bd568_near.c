/* HSTMATCH module=EBOOT.BIN start=0x000bd568 end=0x000bd6a4 symbol=FUN_000bd568 lang=c99 */
void *memcpy(void *dst, const void *src, unsigned int n);
int FUN_000bdb14(void *, void *, void *, unsigned short, void *, int, int, int);
void FUN_000ba3ac(const void *, ...);
extern char DAT_002d54e4;

typedef struct Item {
    char pad0[4];
    void *buf;    // 0x04
    char pad1[4];
    int size;     // 0x0c
} Item;

typedef struct Entry {
    char data[6];
    unsigned char flag; // +6 = 0x5e relative to 0x58
} Entry;

typedef struct Header {
    int type;     // +0 (44(sp))
    int size;     // +4 (48(sp))
    float f1;     // +8 (52(sp))
    float f2;     // +12 (56(sp))
    int idx;      // +16 (60(sp))
} Header;

typedef struct Context {
    char pad0[0x10];
    unsigned short field_10; // 0x10
    char pad1[0x46];
    Entry entries[16];       // 0x58: 16 * 7 = 112 bytes (0x58..0xc8)
    char pad2[0xb4];         // 0xc8..0x17c: 0x17c - 0xc8 = 0xb4
    void *field_17c;         // 0x17c
    char pad3[4];
    void *field_184;         // 0x184
    char pad4[0x10];
    int idx;                 // 0x198
    char pad5[0x40];         // 0x19c..0x1dc: 0x40 = 64
    Item *items[1];          // 0x1dc
    char pad6[0x27c - 0x1e0];
    float f1;                // 0x27c
    float f2;                // 0x280
} Context;

int FUN_000bd568(Context *self)
{
    int idx = self->idx;
    Item *item = self->items[idx];
    if (!item) {
        return 0;
    }

    Header hdr;
    hdr.type = 5;
    int s2 = item->size + 20;
    hdr.idx = idx;
    hdr.size = s2;
    hdr.f1 = self->f1;
    hdr.f2 = self->f2;

    void *s0 = self->field_184;
    memcpy(s0, &hdr, 20);

    Item *item2 = self->items[self->idx];
    memcpy((char *)s0 + 20, item2->buf, item2->size);

    for (int i = 0; i < 16; i++) {
        if (self->entries[i].flag != 0) {
            if (i != self->idx) {
                int ret = FUN_000bdb14(self, self->field_17c, &self->entries[i],
                                       self->field_10, self->field_184, s2, 0, 1);
                if (ret < 0 && ret != (int)0x80410001 && ret != (int)0x80410701) {
                    FUN_000ba3ac(&DAT_002d54e4, ret);
                }
            }
        }
    }

    return 1;
}

// 5 differing words out of 79 words (improved from 67 differing words).
// Differs in:
// 1. Float loads at 0xbd5c0..0xbd5c8: retail loads 636 into $f1 and 640 into $f0, while ours uses $f0 for both sequentially.
// 2. Argument evaluation order for FUN_000bdb14: retail loads lhu a3, 16(s3) before lw a1, 380(s3).
// near miss from lane dsn5 (unreviewed)
