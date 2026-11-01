/* HSTMATCH module=hstp_trans_main.prx start=0x001787ec end=0x00178cac symbol=FUN_001787ec lang=c++ */
/* Clone of src/eboot/filter_entries_0016c574.cpp (hstclone.py tag dsn67): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

#pragma opt_strength_reduction off

struct Tag {};
struct Alloc {};
struct Src;

struct Entry {
  int id;
  int score;
  bool fa;
  bool fb;
  bool fc;
  bool fd;
  bool fe;
  Src *src;
  Entry() {
    score = 0;
    id = 0;
    src = 0;
    fa = false;
    fb = false;
    fc = false;
    fd = false;
    fe = false;
  }
};

struct PtrVec;
extern "C" void FUN_00185ef0(PtrVec *, const Entry *const *, Tag);

struct PtrVec {
  const Entry **data;
  unsigned size;
  unsigned cap;
  const Entry *&at(unsigned i) { return data[i]; }
  void push_back(const Entry *const &x, Tag) {
    if (size < cap) {
      size++;
      data[size - 1] = x;
    } else {
      FUN_00185ef0(this, &x, Tag());
    }
  }
};

extern "C" {
extern void *DAT_003e1e48;
extern unsigned FUN_000dca08(void *, int);
extern void FUN_00185fe8(PtrVec *, unsigned);
extern unsigned char *FUN_00177478(int);
}

extern "C" void FUN_001787ec(PtrVec *dest, const PtrVec *src, int param_3, int) {
    Entry *e;
    FUN_000dca08(DAT_003e1e48, 5);
    if (dest->cap < src->size) {
        FUN_00185fe8(dest, src->size);
    }

    for (unsigned i = 0; i < src->size; i++) {
        const Entry *s7 = src->data[i];
        int mod = s7->id % 10;
        int div = s7->id / 10;
        unsigned char *p = FUN_00177478(*(volatile int *)&s7->id);
        bool bVar1 = *p - 4U < 2;

        bool ok = false;
        if (div == 0 || div == 50) {
            ok = true;
        } else {
            switch (param_3) {
            case 2:
                if (mod == 2) {
                    ok = true;
                }
                break;
            case 1:
                if (mod == 0) {
                } else if (mod == 1) {
                    if (bVar1) {
                        ok = true;
                    }
                } else if (mod == 2) {
                    bool found = false;
                    for (unsigned j = 0; j < src->size; j++) {
                        const Entry *other = src->data[j];
                        int other_mod = other->id % 10;
                        int other_div = other->id / 10;
                        unsigned char *p2 = FUN_00177478(*(volatile int *)&other->id);
                        if (*p2 - 4U < 2) {
                            if (div == other_div && other_mod >= param_3 && other_mod < mod) {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        ok = true;
                    }
                }
                break;
            case 0:
                if (mod == 0) {
                    if (bVar1) {
                        ok = true;
                    }
                } else if (mod == 1) {
                    if (bVar1) {
                        bool found = false;
                        for (unsigned j = 0; j < src->size; j++) {
                            const Entry *other = src->data[j];
                            int other_mod = other->id % 10;
                            int other_div = other->id / 10;
                            unsigned char *p2 = FUN_00177478(*(volatile int *)&other->id);
                            if (*p2 - 4U < 2) {
                                if (div == other_div && other_mod >= param_3 && other_mod < mod) {
                                    found = true;
                                    break;
                                }
                            }
                        }
                        if (!found) {
                            ok = true;
                        }
                    }
                } else {
                    bool found = false;
                    for (unsigned j = 0; j < src->size; j++) {
                        const Entry *other = src->data[j];
                        int other_mod = other->id % 10;
                        int other_div = other->id / 10;
                        unsigned char *p2 = FUN_00177478(*(volatile int *)&other->id);
                        if (*p2 - 4U < 2) {
                            if (div == other_div && other_mod >= param_3 && other_mod < mod) {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        ok = true;
                    }
                }
                break;
            }
        }

        if (ok) {
            Entry *m = new Entry;
            e = m;
            m->id = s7->id;
            m->score = s7->score;
            m->fa = s7->fa;
            m->fb = s7->fb;
            m->fc = s7->fc;
            m->fd = s7->fd;
            m->fe = s7->fe;
            m->src = s7->src;
            dest->push_back(e, Tag());
        }
    }
}
