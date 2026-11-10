/* HSTMATCH module=EBOOT.BIN start=0x001b1810 end=0x001b1b20 symbol=FUN_001b1810 lang=c++ */
/* Hand-written reconstruction from the disassembly (CSV reader recipe of ai_param_load_00234898.cpp); exact match verified; the 1.0f fills are written as count-first do/while loops (a plain for loop is unrolled), and the movie volume goes through a float& to the table slot; names and types await human review. */

#define SOUND_COUNT 474
#define MOVIE_COUNT 4

extern "C" {
extern char DAT_003070bc[];
extern char DAT_002e980c[];
extern char DAT_002e9824[];
extern char DAT_002e983c[];
extern char DAT_002e9840[];
extern char DAT_002e984c[];
extern char DAT_002e9854[];
extern char DAT_002e9864[];
extern char DAT_002e9888[];
extern char DAT_002e98bc[];
extern char DAT_002e98d4[];
extern char DAT_002e9900[];
extern const char *DAT_002c5a34[SOUND_COUNT];
extern float DAT_002e515c[SOUND_COUNT];
extern float DAT_002e58c4[MOVIE_COUNT];
void FUN_000649c0(void *);
void FUN_00064a08(void *, int);
void FUN_0008c230(void *);
void FUN_00066338(void *, int);
int FUN_000651b8(void *, const char *, int, int, char *);
int FUN_000650e0(void *);
void FUN_000652e8(void *);
float FUN_000655f8(void *, int, const char *);
const char *FUN_0006569c(void *, int, const char *);
int FUN_00014934(const char *, const char *);
void FUN_001b1b20(const char *fmt, ...);
}

struct Str {
    int w[4];
    Str() { FUN_0008c230(this); }
    ~Str() { FUN_00066338(this, 0); }
};

struct Base16 {
    int w[4];
    Base16()
    {
        FUN_000649c0(this);
        *(char **)this = DAT_003070bc;
    }
    ~Base16() { FUN_00064a08(this, 0); }
};

struct CsvTable : Base16 {
    Str a;
    Str b;
    ~CsvTable() { *(char **)this = DAT_003070bc; }
};

extern "C" void FUN_001b1810(void)
{
    FUN_001b1b20(DAT_002e980c);
    {
        unsigned i = 0;
        float *p = DAT_002e515c;
        do {
            i++;
            *p = 1.0f;
            p++;
        } while (i < SOUND_COUNT);
    }
    CsvTable csv;
    if (FUN_000651b8(&csv, DAT_002e9824, 0, 1, DAT_002e983c)) {
        for (int row = 1; row < FUN_000650e0(&csv); row++) {
            int found = 0;
            const char *name = FUN_0006569c(&csv, row, DAT_002e9840);
            if (name != 0) {
                for (unsigned i = 0; i < SOUND_COUNT; i++) {
                    if (name != 0 && FUN_00014934(name, DAT_002c5a34[i]) == 0) {
                        float v = FUN_000655f8(&csv, row, DAT_002e984c);
                        DAT_002e515c[i] = v;
                        FUN_001b1b20(DAT_002e9854, name, v);
                        found = 1;
                    }
                }
            }
            if (!found)
                FUN_001b1b20(DAT_002e9864, name);
        }
    } else
        FUN_001b1b20(DAT_002e9888);
    {
        unsigned i = 0;
        float *p = DAT_002e58c4;
        do {
            i++;
            *p = 1.0f;
            p++;
        } while (i < MOVIE_COUNT);
    }
    FUN_000652e8(&csv);
    if (FUN_000651b8(&csv, DAT_002e98bc, 0, 1, DAT_002e983c)) {
        for (int row = 1; row < FUN_000650e0(&csv); row++) {
            unsigned idx = row - 1;
            const char *name = FUN_0006569c(&csv, row, DAT_002e9840);
            if (idx >= MOVIE_COUNT) {
                FUN_001b1b20(DAT_002e98d4, name ? name : DAT_002e9900, MOVIE_COUNT);
                break;
            }
            float &dst = DAT_002e58c4[idx];
            dst = FUN_000655f8(&csv, row, DAT_002e984c);
            FUN_001b1b20(DAT_002e9854, name, dst);
        }
    }
}
