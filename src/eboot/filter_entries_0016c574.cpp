/* HSTMATCH module=EBOOT.BIN start=0x0016c574 end=0x0016ca34 symbol=FUN_0016c574 lang=c++ */
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
extern "C" void FUN_001797fc(PtrVec *, const Entry *const *, Tag);

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
      FUN_001797fc(this, &x, Tag());
    }
  }
};

extern "C" {
extern void *DAT_00338b08;
extern unsigned FUN_000d8bfc(void *, int);
extern void FUN_001798f4(PtrVec *, unsigned);
extern unsigned char *FUN_0016b2b4(int);
}

extern "C" void FUN_0016c574(PtrVec *dest, const PtrVec *src, int param_3, int) {
    Entry *e;
    FUN_000d8bfc(DAT_00338b08, 5);
    if (dest->cap < src->size) {
        FUN_001798f4(dest, src->size);
    }

    for (unsigned i = 0; i < src->size; i++) {
        const Entry *s7 = src->data[i];
        int mod = s7->id % 10;
        int div = s7->id / 10;
        unsigned char *p = FUN_0016b2b4(*(volatile int *)&s7->id);
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
                        unsigned char *p2 = FUN_0016b2b4(*(volatile int *)&other->id);
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
                            unsigned char *p2 = FUN_0016b2b4(*(volatile int *)&other->id);
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
                        unsigned char *p2 = FUN_0016b2b4(*(volatile int *)&other->id);
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
