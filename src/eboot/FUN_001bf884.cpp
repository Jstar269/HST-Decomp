/* HSTMATCH module=EBOOT.BIN start=0x001bf884 end=0x001bf948 symbol=FUN_001bf884 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Entry {
    char pad[0x38];
};

struct Sub {
    int pad;
    Entry items[15];

    Entry *GetEntry(int i) {
        if (i >= 0 && i < 15) {
            return &items[i];
        }
        return 0;
    }
};

struct Mgr {
    char pad[0x80];
    Sub sub;
};

struct Cell {
    unsigned char val;
    unsigned char pad;
};

struct Obj {
    char pad[0x34c];
    Cell cells[640];

    Cell *GetCell(int i) {
        if (i >= 0 && i < 640) {
            return &cells[i];
        }
        return 0;
    }
};

extern Mgr *DAT_003415a8;
extern "C" void FUN_001bbab8(Entry *);

extern "C" void FUN_001bf884(Obj *self, int idx, unsigned char val)
{
    Cell *c = self->GetCell(idx);
    if (c == 0) {
        return;
    }
    c->val = val;
    if ((val == 4 || val == 5) && DAT_003415a8 != 0) {
        for (int i = 0; i < 15; i++) {
            FUN_001bbab8(DAT_003415a8->sub.GetEntry(i));
        }
    }
}
