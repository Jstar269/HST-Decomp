/* HSTMATCH module=hstp_trans_main.prx start=0x00279cb8 end=0x0027a248 symbol=FUN_00279cb8 lang=c++ */
/* Hand-written reconstruction: clone of the source of handle_state_msg_00268d3c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
#pragma exceptions off
#pragma opt_strength_reduction off

typedef unsigned char u8;
typedef unsigned int u32;

struct SubItem {
    u8 pad[208];
    float f_d0;
};

struct Item {
    float rect_0[4];    // 0..15
    float rect_16[4];   // 16..31
    int field_32;       // 32
    SubItem *sub_36;    // 36
};

class MainObj {
public:
    u8 pad0[60];
    int state;          // 60 (0x3c)
    u8 byte_64;         // 64 (0x40)
    u8 pad65[7];        // 65
    int field_72;       // 72 (0x48)
    u8 pad76[4];        // 76
    Item **items_80;    // 80 (0x50)
    u32 count_84;       // 84 (0x54)
    u8 pad88[13];       // 88
    u8 byte_101;        // 101 (0x65)

    void transition(int target_state);
    void copy_rects();
    int FUN_00279cb8(int msg);
};

struct DeviceState {
    u8 pad[97];
    u8 byte_97;         // 97 (0x61)
    u8 pad98[34];       // 98
    u8 byte_132;        // 132 (0x84)
    u8 byte_133;        // 133 (0x85)
};

extern "C" DeviceState *DAT_003e1e84;
extern "C" int *DAT_003e95e0;

extern "C" void FUN_0005ff00(SubItem *, float);
extern "C" int FUN_000e31ec(DeviceState *);
extern "C" int FUN_000e31a0(DeviceState *);

inline void MainObj::transition(int target_state) {
    if (this->state == target_state) return;
    this->state = target_state;
    this->byte_64 = 1;
    for (u32 i = 0; i < this->count_84; i++) {
        SubItem *sub = this->items_80[i]->sub_36;
        if (sub->f_d0 == 0.0f) continue;
        FUN_0005ff00(sub, 0.0f);
        *(int *)&sub->f_d0 = 0;
    }
}

inline void MainObj::copy_rects() {
    for (u32 i = 0; i < this->count_84; i++) {
        Item *item = this->items_80[i];
        item->rect_16[0] = item->rect_0[0];
        item->rect_16[1] = item->rect_0[1];
        item->rect_16[2] = item->rect_0[2];
        item->rect_16[3] = item->rect_0[3];
        this->items_80[i]->field_32 = 0;
    }
}

extern "C" int FUN_00279cb8(MainObj *self, int msg) {
    switch (msg) {
    case 4:
        self->transition(2);
        break;

    case 5:
        self->transition(1);
        break;

    case 19:
        break;

    case 16:
    case 18:
        self->transition(1);
        self->byte_101 = 1;
        break;

    case 8:
        if (*(int *)((char *)DAT_003e95e0 + 0x1364) == 6) {
            self->byte_101 = 1;
            goto case_8_done;
        }
        self->byte_101 = 0;
    case_8_done:
        self->copy_rects();
        self->transition(1);
        break;

    case 20:
        if (FUN_000e31ec(DAT_003e1e84) != 0) {
            self->transition(1);
        } else if (DAT_003e1e84->byte_132 != 0 || DAT_003e1e84->byte_97 != 0 || FUN_000e31a0(DAT_003e1e84) != 0 || DAT_003e1e84->byte_133 != 0) {
            self->field_72 = 0;
            self->transition(3);
        } else {
            self->transition(2);
        }
        break;

    case 22:
    case 14:
        self->copy_rects();
        self->transition(1);
        break;
    }
    return 0;
}
