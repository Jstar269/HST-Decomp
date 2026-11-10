/* HSTMATCH module=EBOOT.BIN start=0x00162338 end=0x0016342c symbol=FUN_00162338 lang=c++ */
/* Hand-written reconstruction from the disassembly (same CSV reader/record recipe as ai_param_load_00234898.cpp; field names from the CSV column strings); exact match verified; the main table also loads the racket table (same class, DAT_0034150c) and the grade table (DAT_00341510, FUN_001607a4) when its path names character.csv; the new[1] arrays are spelled as explicit FUN_00000a90/FUN_000008d8 calls; names and types await human review. */
struct Tag {};

extern "C" {
extern char DAT_003070bc[];
extern char DAT_002de8b4[];
extern char DAT_002de908[];
extern char DAT_002dfb04[];
extern char DAT_002e0340[];
extern char DAT_002e0350[];
extern short DAT_002dfb00;
extern char DAT_002de8b8[];
extern char DAT_002de8c0[];
extern char DAT_002de8cc[];
extern char DAT_002de8d4[];
extern char DAT_002de8dc[];
extern char DAT_002deae0[];
extern char DAT_002deb08[];
extern char DAT_002deb20[];
extern char DAT_002dfb0c[];
extern char DAT_002dfb18[];
extern char DAT_002dfb24[];
extern char DAT_002dfb30[];
extern char DAT_002dfb38[];
extern char DAT_002dfb40[];
extern char DAT_002dfb50[];
extern char DAT_002dfb60[];
extern char DAT_002dfb74[];
extern char DAT_002dfb80[];
extern char DAT_002dfb94[];
extern char DAT_002dfb9c[];
extern char DAT_002dfba4[];
extern char DAT_002dfbb0[];
extern char DAT_002dfbc0[];
extern char DAT_002dfbcc[];
extern char DAT_002dfbd8[];
extern char DAT_002dfbe4[];
extern char DAT_002dfbf0[];
extern char DAT_002dfbf8[];
extern char DAT_002dfc04[];
extern char DAT_002dfc10[];
extern char DAT_002dfc1c[];
extern char DAT_002dfc28[];
extern char DAT_002dfc38[];
extern char DAT_002dfc48[];
extern char DAT_002dfc58[];
extern char DAT_002dfc6c[];
extern char DAT_002dfc78[];
extern char DAT_002dfc84[];
extern char DAT_002dfc90[];
extern char DAT_002dfca0[];
extern char DAT_002dfcac[];
extern char DAT_002dfcc0[];
extern char DAT_002dfcd4[];
extern char DAT_002dfce4[];
extern char DAT_002dfcf4[];
extern char DAT_002dfd04[];
extern char DAT_002dfd10[];
extern char DAT_002dfd20[];
extern char DAT_002dfd30[];
extern char DAT_002dfd40[];
extern char DAT_002dfd4c[];
extern char DAT_002dfd58[];
extern char DAT_002dfd60[];
extern char DAT_002dfd6c[];
extern char DAT_002dfd7c[];
extern char DAT_002dfd88[];
extern char DAT_002dfd94[];
extern char DAT_002dfda4[];
extern char DAT_002dfdb4[];
extern char DAT_002dfdc0[];
extern char DAT_002dfdcc[];
extern char DAT_002dfdd8[];
extern char DAT_002dfde8[];
extern char DAT_002dfdf4[];
extern char DAT_002dfe00[];
extern char DAT_002dfe0c[];
extern char DAT_002dfe1c[];
extern char DAT_002dfe2c[];
extern char DAT_002dfe38[];
extern char DAT_002dfe44[];
extern char DAT_002dfe54[];
extern char DAT_002dfe64[];
extern char DAT_002dfe78[];
extern char DAT_002dfe8c[];
extern char DAT_002dfe9c[];
extern char DAT_002dfeac[];
extern char DAT_002dfebc[];
extern char DAT_002dfed0[];
extern char DAT_002dfee4[];
extern char DAT_002dfef4[];
extern char DAT_002dff08[];
extern char DAT_002dff18[];
extern char DAT_002dff2c[];
extern char DAT_002dff40[];
extern char DAT_002dff58[];
extern char DAT_002dff70[];
extern char DAT_002dff84[];
extern char DAT_002dff94[];
extern char DAT_002dffa4[];
extern char DAT_002dffb8[];
extern char DAT_002dffcc[];
extern char DAT_002dffdc[];
extern char DAT_002dffe8[];
extern char DAT_002dfff4[];
extern char DAT_002e0004[];
extern char DAT_002e0014[];
extern char DAT_002e0020[];
extern char DAT_002e002c[];
extern char DAT_002e0038[];
extern char DAT_002e0048[];
extern char DAT_002e0058[];
extern char DAT_002e0064[];
extern char DAT_002e0074[];
extern char DAT_002e0080[];
extern char DAT_002e0090[];
extern char DAT_002e00a4[];
extern char DAT_002e00b0[];
extern char DAT_002e00c0[];
extern char DAT_002e00cc[];
extern char DAT_002e00d8[];
extern char DAT_002e00e8[];
extern char DAT_002e00f8[];
extern char DAT_002e0104[];
extern char DAT_002e0110[];
extern char DAT_002e011c[];
extern char DAT_002e0128[];
extern char DAT_002e0138[];
extern char DAT_002e0148[];
extern char DAT_002e0154[];
extern char DAT_002e0164[];
extern char DAT_002e0174[];
extern char DAT_002e0188[];
extern char DAT_002e019c[];
extern char DAT_002e01ac[];
extern char DAT_002e01bc[];
extern char DAT_002e01cc[];
extern char DAT_002e01e0[];
extern char DAT_002e01f4[];
extern char DAT_002e0204[];
extern char DAT_002e0214[];
extern char DAT_002e0224[];
extern char DAT_002e0238[];
extern char DAT_002e024c[];
extern char DAT_002e025c[];
extern char DAT_002e0268[];
extern char DAT_002e0274[];
extern char DAT_002e0280[];
extern char DAT_002e028c[];
extern char DAT_002e029c[];
extern char DAT_002e02ac[];
extern char DAT_002e02c0[];
extern char DAT_002e02d0[];
extern char DAT_002e02dc[];
extern char DAT_002e02e8[];
extern char DAT_002e02f8[];
extern char DAT_002e0304[];
extern char DAT_002e0310[];
extern char DAT_002e0320[];
extern char DAT_002e0330[];
void FUN_000649c0(void *);
void FUN_00064a08(void *, int);
void FUN_0008c230(void *);
void FUN_00066338(void *, int);
int FUN_000651b8(void *, const char *, int, int, char *);
int FUN_000650e0(void *);
int FUN_00065364(void *, const char *);
char *FUN_0006517c(void *, int, int);
int FUN_001639ec(void *, int, const char *);
float FUN_00163a4c(void *, int, const char *);
void *FUN_000114c0(void *, int, int);
char *FUN_000149a8(char *, const char *);
int FUN_0000fe64(int);
int FUN_0000d62c(const char *);
char *FUN_00014d64(const char *, const char *);
void *FUN_00000a90(unsigned int);
void *FUN_000008d8(void *, void *, void *, unsigned int, unsigned int);
void FUN_001621e0();
void FUN_00162200();
void FUN_00160790();
void FUN_001622d8();
void FUN_001607a4(void *);
}

struct CharParam {
    short id;
    char name[16];
    signed char swingFore; /* 18 */
    signed char swingBack; /* 19 */
    signed char playstyle; /* 20 */
    float manualTargetUnitX; /* 24 */
    float volleyManualTargetUnitX; /* 28 */
    float smashManualTargetUnitX; /* 32 */
    float height; /* 36 */
    float leach; /* 40 */
    float distLimitStroke; /* 44 */
    float strongshotArea; /* 48 */
    float strongshotAreaMax; /* 52 */
    float ballhitArea; /* 56 */
    float heightLimitServe; /* 60 */
    float col_2dfb94; /* 64 */
    float power; /* 68 */
    float col_2dfb9c; /* 72 */
    float technique; /* 76 */
    float speed; /* 80 */
    float stamina; /* 84 */
    float impact; /* 88 */
    float powerShop; /* 92 */
    float techniqueShop; /* 96 */
    float speedShop; /* 100 */
    float staminaShop; /* 104 */
    float impactShop; /* 108 */
    float usaPower; /* 112 */
    float usaSpin; /* 116 */
    float kamePower; /* 120 */
    float kameSpin; /* 124 */
    float backPower; /* 128 */
    float backSpin; /* 132 */
    float backTechnique; /* 136 */
    float backVolleyPower; /* 140 */
    float backVolleySpin; /* 144 */
    float backVolleyTechnique; /* 148 */
    float bodyArea; /* 152 */
    float bodyPower; /* 156 */
    float bodySpin; /* 160 */
    float bodyTechnique; /* 164 */
    int bodyRigor; /* 168 */
    float snatchPowerLower; /* 172 */
    float snatchPowerUpper; /* 176 */
    float snatchSpinLower; /* 180 */
    float snatchSpinUpper; /* 184 */
    float snatchTechnique; /* 188 */
    short snatchRigor; /* 192 */
    short snatchMissRigor; /* 194 */
    float speedForward; /* 196 */
    float speedBackward; /* 200 */
    float speedSide; /* 204 */
    float topPower; /* 208 */
    float topSpin; /* 212 */
    float topSidespin; /* 216 */
    float topTechnique; /* 220 */
    int topRigor; /* 224 */
    float slicePower; /* 228 */
    float sliceSidespin; /* 232 */
    float sliceTechnique; /* 236 */
    int sliceRigor; /* 240 */
    float lobPower; /* 244 */
    float lobSidespin; /* 248 */
    float lobTechnique; /* 252 */
    int lobRigor; /* 256 */
    float volleyPower; /* 260 */
    float volleySpin; /* 264 */
    float volleySidespin; /* 268 */
    float volleyTechnique; /* 272 */
    float volleyArea; /* 276 */
    int volleyRigor; /* 280 */
    float volleyWeakPower; /* 284 */
    float volleyWeakSpin; /* 288 */
    float volleyWeakSidespin; /* 292 */
    float volleyWeakTechnique; /* 296 */
    int volleyWeakRigor; /* 300 */
    float lowvolleyPower; /* 304 */
    float lowvolleySpin; /* 308 */
    float lowvolleySidespin; /* 312 */
    float lowvolleyTechnique; /* 316 */
    float lowvolleyArea; /* 320 */
    float lowvolleyAreaNear; /* 324 */
    int lowvolleyRigor; /* 328 */
    float lowvolleyWeakPower; /* 332 */
    float lowvolleyWeakSpin; /* 336 */
    float lowvolleyWeakSidespin; /* 340 */
    float lowvolleyWeakTechnique; /* 344 */
    int lowvolleyWeakRigor; /* 348 */
    float highvolleyPower; /* 352 */
    float highvolleySpin; /* 356 */
    float highvolleySidespin; /* 360 */
    float highvolleyTechnique; /* 364 */
    int highvolleyRigor; /* 368 */
    float flatPower; /* 372 */
    float flatSpin; /* 376 */
    float flatSidespin; /* 380 */
    float flatTechnique; /* 384 */
    float flatArea; /* 388 */
    int flatRigor; /* 392 */
    float dropPower; /* 396 */
    float dropSidespin; /* 400 */
    float dropTechnique; /* 404 */
    int dropRigor; /* 408 */
    float airshotPower; /* 412 */
    float airshotSpin; /* 416 */
    float airshotSidespin; /* 420 */
    float airshotTechnique; /* 424 */
    float airshotArea; /* 428 */
    int airshotRigor; /* 432 */
    float risingPower; /* 436 */
    float risingSpin; /* 440 */
    float risingSidespin; /* 444 */
    float risingTechnique; /* 448 */
    float risingArea; /* 452 */
    int risingRigor; /* 456 */
    float smashPower; /* 460 */
    float smashSpin; /* 464 */
    float smashSidespin; /* 468 */
    float smashTechnique; /* 472 */
    int smashRigor; /* 476 */
    float spinservePower; /* 480 */
    float spinserveSpin; /* 484 */
    float spinserveSidespin; /* 488 */
    float spinserveTechnique; /* 492 */
    int spinserveRigor; /* 496 */
    float sliceservePower; /* 500 */
    float sliceserveSpin; /* 504 */
    float sliceserveSidespin; /* 508 */
    float sliceserveTechnique; /* 512 */
    int sliceserveRigor; /* 516 */
    float flatservePower; /* 520 */
    float flatserveSpin; /* 524 */
    float flatserveSidespin; /* 528 */
    float flatserveTechnique; /* 532 */
    int flatserveRigor; /* 536 */
    signed char topForte; /* 540 */
    signed char sliceForte; /* 541 */
    signed char lobForte; /* 542 */
    signed char volleyForte; /* 543 */
    signed char volleyWeakForte; /* 544 */
    signed char lowvolleyForte; /* 545 */
    signed char lowvolleyWeakForte; /* 546 */
    signed char highvolleyForte; /* 547 */
    signed char flatForte; /* 548 */
    signed char dropForte; /* 549 */
    signed char airshotForte; /* 550 */
    signed char risingForte; /* 551 */
    signed char smashForte; /* 552 */
    signed char spinserveForte; /* 553 */
    signed char sliceserveForte; /* 554 */
    signed char flatserveForte; /* 555 */
};

struct ShortVec {
    short *data;
    unsigned size;
    unsigned cap;
    void resize(unsigned n, const short &x);
    short &operator[](unsigned i) { return data[i]; }
};

extern "C" void FUN_0016578c(ShortVec *, unsigned, const short *, Tag);

inline void ShortVec::resize(unsigned n, const short &x)
{
    if (size < n) {
        FUN_0016578c(this, n - size, &x, Tag());
    } else if (n < size) {
        int k = size - n;
        size -= k;
    }
}

struct CharParamTable {
    CharParam *data;
    unsigned size;
    unsigned cap;
    ShortVec index;
    CharParam &operator[](unsigned i) { return data[i]; }
};

extern "C" {
void FUN_001655d0(CharParamTable *, unsigned, Tag);
void FUN_001655e0(CharParamTable *, unsigned);
void FUN_0016342c(CharParamTable *, const CharParam &);
extern CharParamTable *DAT_0034150c;
extern void *DAT_00341510;
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

extern "C" void FUN_00162338(CharParamTable *self, const char *path)
{
    FUN_001655d0(self, self->size, Tag());
    CsvTable csv;
    if (FUN_000651b8(&csv, path, 0, 1, DAT_002de8b4)) {
        int max_id = -1;
        int row_count = FUN_000650e0(&csv);
        if (self->cap < (unsigned)(row_count - 1)) {
            FUN_001655e0(self, row_count - 1);
        }
        for (int row = 1; row < row_count; row++) {
            CharParam rec;
            FUN_000114c0(&rec, 0, sizeof(CharParam));
            int col = FUN_00065364(&csv, DAT_002de908);
            if (col < 0)
                col = FUN_00065364(&csv, DAT_002dfb04);
            char *text = FUN_0006517c(&csv, row, col);
            FUN_000149a8(rec.name, text);
            while (!FUN_0000fe64(*text))
                text++;
            rec.id = FUN_0000d62c(text);
            rec.swingFore = FUN_001639ec(&csv, row, DAT_002dfb0c);
            rec.swingBack = FUN_001639ec(&csv, row, DAT_002dfb18);
            rec.playstyle = FUN_001639ec(&csv, row, DAT_002dfb24);
            rec.manualTargetUnitX = FUN_00163a4c(&csv, row, DAT_002deae0);
            rec.volleyManualTargetUnitX = FUN_00163a4c(&csv, row, DAT_002deb20);
            rec.smashManualTargetUnitX = FUN_00163a4c(&csv, row, DAT_002deb08);
            rec.height = FUN_00163a4c(&csv, row, DAT_002dfb30);
            rec.leach = FUN_00163a4c(&csv, row, DAT_002dfb38);
            rec.distLimitStroke = FUN_00163a4c(&csv, row, DAT_002dfb40);
            rec.strongshotArea = FUN_00163a4c(&csv, row, DAT_002dfb50);
            rec.strongshotAreaMax = FUN_00163a4c(&csv, row, DAT_002dfb60);
            rec.ballhitArea = FUN_00163a4c(&csv, row, DAT_002dfb74);
            rec.heightLimitServe = FUN_00163a4c(&csv, row, DAT_002dfb80);
            rec.col_2dfb94 = FUN_00163a4c(&csv, row, DAT_002dfb94);
            rec.power = FUN_00163a4c(&csv, row, DAT_002de8b8);
            rec.col_2dfb9c = FUN_00163a4c(&csv, row, DAT_002dfb9c);
            rec.technique = FUN_00163a4c(&csv, row, DAT_002de8c0);
            rec.speed = FUN_00163a4c(&csv, row, DAT_002de8cc);
            rec.stamina = FUN_00163a4c(&csv, row, DAT_002de8d4);
            rec.impact = FUN_00163a4c(&csv, row, DAT_002de8dc);
            rec.powerShop = FUN_00163a4c(&csv, row, DAT_002dfba4);
            rec.techniqueShop = FUN_00163a4c(&csv, row, DAT_002dfbb0);
            rec.speedShop = FUN_00163a4c(&csv, row, DAT_002dfbc0);
            rec.staminaShop = FUN_00163a4c(&csv, row, DAT_002dfbcc);
            rec.impactShop = FUN_00163a4c(&csv, row, DAT_002dfbd8);
            rec.usaPower = FUN_00163a4c(&csv, row, DAT_002dfbe4);
            rec.usaSpin = FUN_00163a4c(&csv, row, DAT_002dfbf0);
            rec.kamePower = FUN_00163a4c(&csv, row, DAT_002dfbf8);
            rec.kameSpin = FUN_00163a4c(&csv, row, DAT_002dfc04);
            rec.backPower = FUN_00163a4c(&csv, row, DAT_002dfc10);
            rec.backSpin = FUN_00163a4c(&csv, row, DAT_002dfc1c);
            rec.backTechnique = FUN_00163a4c(&csv, row, DAT_002dfc28);
            rec.backVolleyPower = FUN_00163a4c(&csv, row, DAT_002dfc38);
            rec.backVolleySpin = FUN_00163a4c(&csv, row, DAT_002dfc48);
            rec.backVolleyTechnique = FUN_00163a4c(&csv, row, DAT_002dfc58);
            rec.bodyArea = FUN_00163a4c(&csv, row, DAT_002dfc6c);
            rec.bodyPower = FUN_00163a4c(&csv, row, DAT_002dfc78);
            rec.bodySpin = FUN_00163a4c(&csv, row, DAT_002dfc84);
            rec.bodyTechnique = FUN_00163a4c(&csv, row, DAT_002dfc90);
            rec.bodyRigor = FUN_001639ec(&csv, row, DAT_002dfca0);
            rec.snatchPowerLower = FUN_00163a4c(&csv, row, DAT_002dfcac);
            rec.snatchPowerUpper = FUN_00163a4c(&csv, row, DAT_002dfcc0);
            rec.snatchSpinLower = FUN_00163a4c(&csv, row, DAT_002dfcd4);
            rec.snatchSpinUpper = FUN_00163a4c(&csv, row, DAT_002dfce4);
            rec.snatchTechnique = FUN_00163a4c(&csv, row, DAT_002dfcf4);
            rec.snatchRigor = FUN_001639ec(&csv, row, DAT_002dfd04);
            rec.snatchMissRigor = FUN_001639ec(&csv, row, DAT_002dfd10);
            rec.speedForward = FUN_00163a4c(&csv, row, DAT_002dfd20);
            rec.speedBackward = FUN_00163a4c(&csv, row, DAT_002dfd30);
            rec.speedSide = FUN_00163a4c(&csv, row, DAT_002dfd40);
            rec.topPower = FUN_00163a4c(&csv, row, DAT_002dfd4c);
            rec.topSpin = FUN_00163a4c(&csv, row, DAT_002dfd58);
            rec.topSidespin = FUN_00163a4c(&csv, row, DAT_002dfd60);
            rec.topTechnique = FUN_00163a4c(&csv, row, DAT_002dfd6c);
            rec.topRigor = FUN_001639ec(&csv, row, DAT_002dfd7c);
            rec.slicePower = FUN_00163a4c(&csv, row, DAT_002dfd88);
            rec.sliceSidespin = FUN_00163a4c(&csv, row, DAT_002dfd94);
            rec.sliceTechnique = FUN_00163a4c(&csv, row, DAT_002dfda4);
            rec.sliceRigor = FUN_001639ec(&csv, row, DAT_002dfdb4);
            rec.lobPower = FUN_00163a4c(&csv, row, DAT_002dfdc0);
            rec.lobSidespin = FUN_00163a4c(&csv, row, DAT_002dfdcc);
            rec.lobTechnique = FUN_00163a4c(&csv, row, DAT_002dfdd8);
            rec.lobRigor = FUN_001639ec(&csv, row, DAT_002dfde8);
            rec.volleyPower = FUN_00163a4c(&csv, row, DAT_002dfdf4);
            rec.volleySpin = FUN_00163a4c(&csv, row, DAT_002dfe00);
            rec.volleySidespin = FUN_00163a4c(&csv, row, DAT_002dfe0c);
            rec.volleyTechnique = FUN_00163a4c(&csv, row, DAT_002dfe1c);
            rec.volleyArea = FUN_00163a4c(&csv, row, DAT_002dfe2c);
            rec.volleyRigor = FUN_001639ec(&csv, row, DAT_002dfe38);
            rec.volleyWeakPower = FUN_00163a4c(&csv, row, DAT_002dfe44);
            rec.volleyWeakSpin = FUN_00163a4c(&csv, row, DAT_002dfe54);
            rec.volleyWeakSidespin = FUN_00163a4c(&csv, row, DAT_002dfe64);
            rec.volleyWeakTechnique = FUN_00163a4c(&csv, row, DAT_002dfe78);
            rec.volleyWeakRigor = FUN_001639ec(&csv, row, DAT_002dfe8c);
            rec.lowvolleyPower = FUN_00163a4c(&csv, row, DAT_002dfe9c);
            rec.lowvolleySpin = FUN_00163a4c(&csv, row, DAT_002dfeac);
            rec.lowvolleySidespin = FUN_00163a4c(&csv, row, DAT_002dfebc);
            rec.lowvolleyTechnique = FUN_00163a4c(&csv, row, DAT_002dfed0);
            rec.lowvolleyArea = FUN_00163a4c(&csv, row, DAT_002dfee4);
            rec.lowvolleyAreaNear = FUN_00163a4c(&csv, row, DAT_002dfef4);
            rec.lowvolleyRigor = FUN_001639ec(&csv, row, DAT_002dff08);
            rec.lowvolleyWeakPower = FUN_00163a4c(&csv, row, DAT_002dff18);
            rec.lowvolleyWeakSpin = FUN_00163a4c(&csv, row, DAT_002dff2c);
            rec.lowvolleyWeakSidespin = FUN_00163a4c(&csv, row, DAT_002dff40);
            rec.lowvolleyWeakTechnique = FUN_00163a4c(&csv, row, DAT_002dff58);
            rec.lowvolleyWeakRigor = FUN_001639ec(&csv, row, DAT_002dff70);
            rec.highvolleyPower = FUN_00163a4c(&csv, row, DAT_002dff84);
            rec.highvolleySpin = FUN_00163a4c(&csv, row, DAT_002dff94);
            rec.highvolleySidespin = FUN_00163a4c(&csv, row, DAT_002dffa4);
            rec.highvolleyTechnique = FUN_00163a4c(&csv, row, DAT_002dffb8);
            rec.highvolleyRigor = FUN_001639ec(&csv, row, DAT_002dffcc);
            rec.flatPower = FUN_00163a4c(&csv, row, DAT_002dffdc);
            rec.flatSpin = FUN_00163a4c(&csv, row, DAT_002dffe8);
            rec.flatSidespin = FUN_00163a4c(&csv, row, DAT_002dfff4);
            rec.flatTechnique = FUN_00163a4c(&csv, row, DAT_002e0004);
            rec.flatArea = FUN_00163a4c(&csv, row, DAT_002e0014);
            rec.flatRigor = FUN_001639ec(&csv, row, DAT_002e0020);
            rec.dropPower = FUN_00163a4c(&csv, row, DAT_002e002c);
            rec.dropSidespin = FUN_00163a4c(&csv, row, DAT_002e0038);
            rec.dropTechnique = FUN_00163a4c(&csv, row, DAT_002e0048);
            rec.dropRigor = FUN_001639ec(&csv, row, DAT_002e0058);
            rec.airshotPower = FUN_00163a4c(&csv, row, DAT_002e0064);
            rec.airshotSpin = FUN_00163a4c(&csv, row, DAT_002e0074);
            rec.airshotSidespin = FUN_00163a4c(&csv, row, DAT_002e0080);
            rec.airshotTechnique = FUN_00163a4c(&csv, row, DAT_002e0090);
            rec.airshotArea = FUN_00163a4c(&csv, row, DAT_002e00a4);
            rec.airshotRigor = FUN_001639ec(&csv, row, DAT_002e00b0);
            rec.risingPower = FUN_00163a4c(&csv, row, DAT_002e00c0);
            rec.risingSpin = FUN_00163a4c(&csv, row, DAT_002e00cc);
            rec.risingSidespin = FUN_00163a4c(&csv, row, DAT_002e00d8);
            rec.risingTechnique = FUN_00163a4c(&csv, row, DAT_002e00e8);
            rec.risingArea = FUN_00163a4c(&csv, row, DAT_002e00f8);
            rec.risingRigor = FUN_001639ec(&csv, row, DAT_002e0104);
            rec.smashPower = FUN_00163a4c(&csv, row, DAT_002e0110);
            rec.smashSpin = FUN_00163a4c(&csv, row, DAT_002e011c);
            rec.smashSidespin = FUN_00163a4c(&csv, row, DAT_002e0128);
            rec.smashTechnique = FUN_00163a4c(&csv, row, DAT_002e0138);
            rec.smashRigor = FUN_001639ec(&csv, row, DAT_002e0148);
            rec.spinservePower = FUN_00163a4c(&csv, row, DAT_002e0154);
            rec.spinserveSpin = FUN_00163a4c(&csv, row, DAT_002e0164);
            rec.spinserveSidespin = FUN_00163a4c(&csv, row, DAT_002e0174);
            rec.spinserveTechnique = FUN_00163a4c(&csv, row, DAT_002e0188);
            rec.spinserveRigor = FUN_001639ec(&csv, row, DAT_002e019c);
            rec.sliceservePower = FUN_00163a4c(&csv, row, DAT_002e01ac);
            rec.sliceserveSpin = FUN_00163a4c(&csv, row, DAT_002e01bc);
            rec.sliceserveSidespin = FUN_00163a4c(&csv, row, DAT_002e01cc);
            rec.sliceserveTechnique = FUN_00163a4c(&csv, row, DAT_002e01e0);
            rec.sliceserveRigor = FUN_001639ec(&csv, row, DAT_002e01f4);
            rec.flatservePower = FUN_00163a4c(&csv, row, DAT_002e0204);
            rec.flatserveSpin = FUN_00163a4c(&csv, row, DAT_002e0214);
            rec.flatserveSidespin = FUN_00163a4c(&csv, row, DAT_002e0224);
            rec.flatserveTechnique = FUN_00163a4c(&csv, row, DAT_002e0238);
            rec.flatserveRigor = FUN_001639ec(&csv, row, DAT_002e024c);
            rec.topForte = FUN_001639ec(&csv, row, DAT_002e025c);
            rec.sliceForte = FUN_001639ec(&csv, row, DAT_002e0268);
            rec.lobForte = FUN_001639ec(&csv, row, DAT_002e0274);
            rec.volleyForte = FUN_001639ec(&csv, row, DAT_002e0280);
            rec.volleyWeakForte = FUN_001639ec(&csv, row, DAT_002e028c);
            rec.lowvolleyForte = FUN_001639ec(&csv, row, DAT_002e029c);
            rec.lowvolleyWeakForte = FUN_001639ec(&csv, row, DAT_002e02ac);
            rec.highvolleyForte = FUN_001639ec(&csv, row, DAT_002e02c0);
            rec.flatForte = FUN_001639ec(&csv, row, DAT_002e02d0);
            rec.dropForte = FUN_001639ec(&csv, row, DAT_002e02dc);
            rec.airshotForte = FUN_001639ec(&csv, row, DAT_002e02e8);
            rec.risingForte = FUN_001639ec(&csv, row, DAT_002e02f8);
            rec.smashForte = FUN_001639ec(&csv, row, DAT_002e0304);
            rec.spinserveForte = FUN_001639ec(&csv, row, DAT_002e0310);
            rec.sliceserveForte = FUN_001639ec(&csv, row, DAT_002e0320);
            rec.flatserveForte = FUN_001639ec(&csv, row, DAT_002e0330);
            max_id = rec.id < max_id ? max_id : rec.id;
            FUN_0016342c(self, rec);
        }
        self->index.resize(max_id + 1, DAT_002dfb00);
        for (int i = 0; i < self->size; i++)
            self->index[(*self)[i].id] = i;
    }
    if (FUN_00014d64(path, DAT_002e0340) != 0) {
        DAT_0034150c = (CharParamTable *)FUN_000008d8(FUN_00000a90(0x28), (void *)FUN_001621e0, (void *)FUN_00162200, 0x18, 1);
        FUN_00162338(DAT_0034150c, DAT_002e0350);
        DAT_00341510 = FUN_000008d8(FUN_00000a90(0x1c), (void *)FUN_00160790, (void *)FUN_001622d8, 0xc, 1);
        FUN_001607a4(DAT_00341510);
    }
}
