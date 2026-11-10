/* HSTMATCH module=hstp_trans_main.prx start=0x0016e8c4 end=0x0016f9b8 symbol=FUN_0016e8c4 lang=c++ */
/* Clone of src/eboot/char_param_load_00162338.cpp (hstclone.py tag c085007): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly (same CSV reader/record recipe as ai_param_load_00234898.cpp; field names from the CSV column strings); exact match verified; the main table also loads the racket table (same class, DAT_003ea84c) and the grade table (DAT_003ea850, FUN_0016cd30) when its path names character.csv; the new[1] arrays are spelled as explicit FUN_00000a90/FUN_000008d8 calls; names and types await human review. */
struct Tag {};

extern "C" {
extern char DAT_003b0388[];
extern char DAT_00385e70[];
extern char DAT_00385ec4[];
extern char DAT_003870c0[];
extern char DAT_003878fc[];
extern char DAT_0038790c[];
extern short DAT_003870bc;
extern char DAT_00385e74[];
extern char DAT_00385e7c[];
extern char DAT_00385e88[];
extern char DAT_00385e90[];
extern char DAT_00385e98[];
extern char DAT_0038609c[];
extern char DAT_003860c4[];
extern char DAT_003860dc[];
extern char DAT_003870c8[];
extern char DAT_003870d4[];
extern char DAT_003870e0[];
extern char DAT_003870ec[];
extern char DAT_003870f4[];
extern char DAT_003870fc[];
extern char DAT_0038710c[];
extern char DAT_0038711c[];
extern char DAT_00387130[];
extern char DAT_0038713c[];
extern char DAT_00387150[];
extern char DAT_00387158[];
extern char DAT_00387160[];
extern char DAT_0038716c[];
extern char DAT_0038717c[];
extern char DAT_00387188[];
extern char DAT_00387194[];
extern char DAT_003871a0[];
extern char DAT_003871ac[];
extern char DAT_003871b4[];
extern char DAT_003871c0[];
extern char DAT_003871cc[];
extern char DAT_003871d8[];
extern char DAT_003871e4[];
extern char DAT_003871f4[];
extern char DAT_00387204[];
extern char DAT_00387214[];
extern char DAT_00387228[];
extern char DAT_00387234[];
extern char DAT_00387240[];
extern char DAT_0038724c[];
extern char DAT_0038725c[];
extern char DAT_00387268[];
extern char DAT_0038727c[];
extern char DAT_00387290[];
extern char DAT_003872a0[];
extern char DAT_003872b0[];
extern char DAT_003872c0[];
extern char DAT_003872cc[];
extern char DAT_003872dc[];
extern char DAT_003872ec[];
extern char DAT_003872fc[];
extern char DAT_00387308[];
extern char DAT_00387314[];
extern char DAT_0038731c[];
extern char DAT_00387328[];
extern char DAT_00387338[];
extern char DAT_00387344[];
extern char DAT_00387350[];
extern char DAT_00387360[];
extern char DAT_00387370[];
extern char DAT_0038737c[];
extern char DAT_00387388[];
extern char DAT_00387394[];
extern char DAT_003873a4[];
extern char DAT_003873b0[];
extern char DAT_003873bc[];
extern char DAT_003873c8[];
extern char DAT_003873d8[];
extern char DAT_003873e8[];
extern char DAT_003873f4[];
extern char DAT_00387400[];
extern char DAT_00387410[];
extern char DAT_00387420[];
extern char DAT_00387434[];
extern char DAT_00387448[];
extern char DAT_00387458[];
extern char DAT_00387468[];
extern char DAT_00387478[];
extern char DAT_0038748c[];
extern char DAT_003874a0[];
extern char DAT_003874b0[];
extern char DAT_003874c4[];
extern char DAT_003874d4[];
extern char DAT_003874e8[];
extern char DAT_003874fc[];
extern char DAT_00387514[];
extern char DAT_0038752c[];
extern char DAT_00387540[];
extern char DAT_00387550[];
extern char DAT_00387560[];
extern char DAT_00387574[];
extern char DAT_00387588[];
extern char DAT_00387598[];
extern char DAT_003875a4[];
extern char DAT_003875b0[];
extern char DAT_003875c0[];
extern char DAT_003875d0[];
extern char DAT_003875dc[];
extern char DAT_003875e8[];
extern char DAT_003875f4[];
extern char DAT_00387604[];
extern char DAT_00387614[];
extern char DAT_00387620[];
extern char DAT_00387630[];
extern char DAT_0038763c[];
extern char DAT_0038764c[];
extern char DAT_00387660[];
extern char DAT_0038766c[];
extern char DAT_0038767c[];
extern char DAT_00387688[];
extern char DAT_00387694[];
extern char DAT_003876a4[];
extern char DAT_003876b4[];
extern char DAT_003876c0[];
extern char DAT_003876cc[];
extern char DAT_003876d8[];
extern char DAT_003876e4[];
extern char DAT_003876f4[];
extern char DAT_00387704[];
extern char DAT_00387710[];
extern char DAT_00387720[];
extern char DAT_00387730[];
extern char DAT_00387744[];
extern char DAT_00387758[];
extern char DAT_00387768[];
extern char DAT_00387778[];
extern char DAT_00387788[];
extern char DAT_0038779c[];
extern char DAT_003877b0[];
extern char DAT_003877c0[];
extern char DAT_003877d0[];
extern char DAT_003877e0[];
extern char DAT_003877f4[];
extern char DAT_00387808[];
extern char DAT_00387818[];
extern char DAT_00387824[];
extern char DAT_00387830[];
extern char DAT_0038783c[];
extern char DAT_00387848[];
extern char DAT_00387858[];
extern char DAT_00387868[];
extern char DAT_0038787c[];
extern char DAT_0038788c[];
extern char DAT_00387898[];
extern char DAT_003878a4[];
extern char DAT_003878b4[];
extern char DAT_003878c0[];
extern char DAT_003878cc[];
extern char DAT_003878dc[];
extern char DAT_003878ec[];
void FUN_00065884(void *);
void FUN_000658cc(void *, int);
void FUN_0008d97c(void *);
void FUN_0006778c(void *, int);
int FUN_00066260(void *, const char *, int, int, char *);
int FUN_00065fa4(void *);
int FUN_00066400(void *, const char *);
char *FUN_000661a8(void *, int, int);
int FUN_0016ff78(void *, int, const char *);
float FUN_0016ffd8(void *, int, const char *);
void *FUN_000114c0(void *, int, int);
char *FUN_000149a8(char *, const char *);
int FUN_0000fe64(int);
int FUN_0000d644(const char *);
char *FUN_00014d64(const char *, const char *);
void *FUN_00000a90(unsigned int);
void *FUN_000008d8(void *, void *, void *, unsigned int, unsigned int);
void FUN_0016e76c();
void FUN_0016e78c();
void FUN_0016cd1c();
void FUN_0016e864();
void FUN_0016cd30(void *);
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

extern "C" void FUN_0017197c(ShortVec *, unsigned, const short *, Tag);

inline void ShortVec::resize(unsigned n, const short &x)
{
    if (size < n) {
        FUN_0017197c(this, n - size, &x, Tag());
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
void FUN_001717c0(CharParamTable *, unsigned, Tag);
void FUN_001717d0(CharParamTable *, unsigned);
void FUN_0016f9b8(CharParamTable *, const CharParam &);
extern CharParamTable *DAT_003ea84c;
extern void *DAT_003ea850;
}

struct Str {
    int w[4];
    Str() { FUN_0008d97c(this); }
    ~Str() { FUN_0006778c(this, 0); }
};

struct Base16 {
    int w[4];
    Base16()
    {
        FUN_00065884(this);
        *(char **)this = DAT_003b0388;
    }
    ~Base16() { FUN_000658cc(this, 0); }
};

struct CsvTable : Base16 {
    Str a;
    Str b;
    ~CsvTable() { *(char **)this = DAT_003b0388; }
};

extern "C" void FUN_0016e8c4(CharParamTable *self, const char *path)
{
    FUN_001717c0(self, self->size, Tag());
    CsvTable csv;
    if (FUN_00066260(&csv, path, 0, 1, DAT_00385e70)) {
        int max_id = -1;
        int row_count = FUN_00065fa4(&csv);
        if (self->cap < (unsigned)(row_count - 1)) {
            FUN_001717d0(self, row_count - 1);
        }
        for (int row = 1; row < row_count; row++) {
            CharParam rec;
            FUN_000114c0(&rec, 0, sizeof(CharParam));
            int col = FUN_00066400(&csv, DAT_00385ec4);
            if (col < 0)
                col = FUN_00066400(&csv, DAT_003870c0);
            char *text = FUN_000661a8(&csv, row, col);
            FUN_000149a8(rec.name, text);
            while (!FUN_0000fe64(*text))
                text++;
            rec.id = FUN_0000d644(text);
            rec.swingFore = FUN_0016ff78(&csv, row, DAT_003870c8);
            rec.swingBack = FUN_0016ff78(&csv, row, DAT_003870d4);
            rec.playstyle = FUN_0016ff78(&csv, row, DAT_003870e0);
            rec.manualTargetUnitX = FUN_0016ffd8(&csv, row, DAT_0038609c);
            rec.volleyManualTargetUnitX = FUN_0016ffd8(&csv, row, DAT_003860dc);
            rec.smashManualTargetUnitX = FUN_0016ffd8(&csv, row, DAT_003860c4);
            rec.height = FUN_0016ffd8(&csv, row, DAT_003870ec);
            rec.leach = FUN_0016ffd8(&csv, row, DAT_003870f4);
            rec.distLimitStroke = FUN_0016ffd8(&csv, row, DAT_003870fc);
            rec.strongshotArea = FUN_0016ffd8(&csv, row, DAT_0038710c);
            rec.strongshotAreaMax = FUN_0016ffd8(&csv, row, DAT_0038711c);
            rec.ballhitArea = FUN_0016ffd8(&csv, row, DAT_00387130);
            rec.heightLimitServe = FUN_0016ffd8(&csv, row, DAT_0038713c);
            rec.col_2dfb94 = FUN_0016ffd8(&csv, row, DAT_00387150);
            rec.power = FUN_0016ffd8(&csv, row, DAT_00385e74);
            rec.col_2dfb9c = FUN_0016ffd8(&csv, row, DAT_00387158);
            rec.technique = FUN_0016ffd8(&csv, row, DAT_00385e7c);
            rec.speed = FUN_0016ffd8(&csv, row, DAT_00385e88);
            rec.stamina = FUN_0016ffd8(&csv, row, DAT_00385e90);
            rec.impact = FUN_0016ffd8(&csv, row, DAT_00385e98);
            rec.powerShop = FUN_0016ffd8(&csv, row, DAT_00387160);
            rec.techniqueShop = FUN_0016ffd8(&csv, row, DAT_0038716c);
            rec.speedShop = FUN_0016ffd8(&csv, row, DAT_0038717c);
            rec.staminaShop = FUN_0016ffd8(&csv, row, DAT_00387188);
            rec.impactShop = FUN_0016ffd8(&csv, row, DAT_00387194);
            rec.usaPower = FUN_0016ffd8(&csv, row, DAT_003871a0);
            rec.usaSpin = FUN_0016ffd8(&csv, row, DAT_003871ac);
            rec.kamePower = FUN_0016ffd8(&csv, row, DAT_003871b4);
            rec.kameSpin = FUN_0016ffd8(&csv, row, DAT_003871c0);
            rec.backPower = FUN_0016ffd8(&csv, row, DAT_003871cc);
            rec.backSpin = FUN_0016ffd8(&csv, row, DAT_003871d8);
            rec.backTechnique = FUN_0016ffd8(&csv, row, DAT_003871e4);
            rec.backVolleyPower = FUN_0016ffd8(&csv, row, DAT_003871f4);
            rec.backVolleySpin = FUN_0016ffd8(&csv, row, DAT_00387204);
            rec.backVolleyTechnique = FUN_0016ffd8(&csv, row, DAT_00387214);
            rec.bodyArea = FUN_0016ffd8(&csv, row, DAT_00387228);
            rec.bodyPower = FUN_0016ffd8(&csv, row, DAT_00387234);
            rec.bodySpin = FUN_0016ffd8(&csv, row, DAT_00387240);
            rec.bodyTechnique = FUN_0016ffd8(&csv, row, DAT_0038724c);
            rec.bodyRigor = FUN_0016ff78(&csv, row, DAT_0038725c);
            rec.snatchPowerLower = FUN_0016ffd8(&csv, row, DAT_00387268);
            rec.snatchPowerUpper = FUN_0016ffd8(&csv, row, DAT_0038727c);
            rec.snatchSpinLower = FUN_0016ffd8(&csv, row, DAT_00387290);
            rec.snatchSpinUpper = FUN_0016ffd8(&csv, row, DAT_003872a0);
            rec.snatchTechnique = FUN_0016ffd8(&csv, row, DAT_003872b0);
            rec.snatchRigor = FUN_0016ff78(&csv, row, DAT_003872c0);
            rec.snatchMissRigor = FUN_0016ff78(&csv, row, DAT_003872cc);
            rec.speedForward = FUN_0016ffd8(&csv, row, DAT_003872dc);
            rec.speedBackward = FUN_0016ffd8(&csv, row, DAT_003872ec);
            rec.speedSide = FUN_0016ffd8(&csv, row, DAT_003872fc);
            rec.topPower = FUN_0016ffd8(&csv, row, DAT_00387308);
            rec.topSpin = FUN_0016ffd8(&csv, row, DAT_00387314);
            rec.topSidespin = FUN_0016ffd8(&csv, row, DAT_0038731c);
            rec.topTechnique = FUN_0016ffd8(&csv, row, DAT_00387328);
            rec.topRigor = FUN_0016ff78(&csv, row, DAT_00387338);
            rec.slicePower = FUN_0016ffd8(&csv, row, DAT_00387344);
            rec.sliceSidespin = FUN_0016ffd8(&csv, row, DAT_00387350);
            rec.sliceTechnique = FUN_0016ffd8(&csv, row, DAT_00387360);
            rec.sliceRigor = FUN_0016ff78(&csv, row, DAT_00387370);
            rec.lobPower = FUN_0016ffd8(&csv, row, DAT_0038737c);
            rec.lobSidespin = FUN_0016ffd8(&csv, row, DAT_00387388);
            rec.lobTechnique = FUN_0016ffd8(&csv, row, DAT_00387394);
            rec.lobRigor = FUN_0016ff78(&csv, row, DAT_003873a4);
            rec.volleyPower = FUN_0016ffd8(&csv, row, DAT_003873b0);
            rec.volleySpin = FUN_0016ffd8(&csv, row, DAT_003873bc);
            rec.volleySidespin = FUN_0016ffd8(&csv, row, DAT_003873c8);
            rec.volleyTechnique = FUN_0016ffd8(&csv, row, DAT_003873d8);
            rec.volleyArea = FUN_0016ffd8(&csv, row, DAT_003873e8);
            rec.volleyRigor = FUN_0016ff78(&csv, row, DAT_003873f4);
            rec.volleyWeakPower = FUN_0016ffd8(&csv, row, DAT_00387400);
            rec.volleyWeakSpin = FUN_0016ffd8(&csv, row, DAT_00387410);
            rec.volleyWeakSidespin = FUN_0016ffd8(&csv, row, DAT_00387420);
            rec.volleyWeakTechnique = FUN_0016ffd8(&csv, row, DAT_00387434);
            rec.volleyWeakRigor = FUN_0016ff78(&csv, row, DAT_00387448);
            rec.lowvolleyPower = FUN_0016ffd8(&csv, row, DAT_00387458);
            rec.lowvolleySpin = FUN_0016ffd8(&csv, row, DAT_00387468);
            rec.lowvolleySidespin = FUN_0016ffd8(&csv, row, DAT_00387478);
            rec.lowvolleyTechnique = FUN_0016ffd8(&csv, row, DAT_0038748c);
            rec.lowvolleyArea = FUN_0016ffd8(&csv, row, DAT_003874a0);
            rec.lowvolleyAreaNear = FUN_0016ffd8(&csv, row, DAT_003874b0);
            rec.lowvolleyRigor = FUN_0016ff78(&csv, row, DAT_003874c4);
            rec.lowvolleyWeakPower = FUN_0016ffd8(&csv, row, DAT_003874d4);
            rec.lowvolleyWeakSpin = FUN_0016ffd8(&csv, row, DAT_003874e8);
            rec.lowvolleyWeakSidespin = FUN_0016ffd8(&csv, row, DAT_003874fc);
            rec.lowvolleyWeakTechnique = FUN_0016ffd8(&csv, row, DAT_00387514);
            rec.lowvolleyWeakRigor = FUN_0016ff78(&csv, row, DAT_0038752c);
            rec.highvolleyPower = FUN_0016ffd8(&csv, row, DAT_00387540);
            rec.highvolleySpin = FUN_0016ffd8(&csv, row, DAT_00387550);
            rec.highvolleySidespin = FUN_0016ffd8(&csv, row, DAT_00387560);
            rec.highvolleyTechnique = FUN_0016ffd8(&csv, row, DAT_00387574);
            rec.highvolleyRigor = FUN_0016ff78(&csv, row, DAT_00387588);
            rec.flatPower = FUN_0016ffd8(&csv, row, DAT_00387598);
            rec.flatSpin = FUN_0016ffd8(&csv, row, DAT_003875a4);
            rec.flatSidespin = FUN_0016ffd8(&csv, row, DAT_003875b0);
            rec.flatTechnique = FUN_0016ffd8(&csv, row, DAT_003875c0);
            rec.flatArea = FUN_0016ffd8(&csv, row, DAT_003875d0);
            rec.flatRigor = FUN_0016ff78(&csv, row, DAT_003875dc);
            rec.dropPower = FUN_0016ffd8(&csv, row, DAT_003875e8);
            rec.dropSidespin = FUN_0016ffd8(&csv, row, DAT_003875f4);
            rec.dropTechnique = FUN_0016ffd8(&csv, row, DAT_00387604);
            rec.dropRigor = FUN_0016ff78(&csv, row, DAT_00387614);
            rec.airshotPower = FUN_0016ffd8(&csv, row, DAT_00387620);
            rec.airshotSpin = FUN_0016ffd8(&csv, row, DAT_00387630);
            rec.airshotSidespin = FUN_0016ffd8(&csv, row, DAT_0038763c);
            rec.airshotTechnique = FUN_0016ffd8(&csv, row, DAT_0038764c);
            rec.airshotArea = FUN_0016ffd8(&csv, row, DAT_00387660);
            rec.airshotRigor = FUN_0016ff78(&csv, row, DAT_0038766c);
            rec.risingPower = FUN_0016ffd8(&csv, row, DAT_0038767c);
            rec.risingSpin = FUN_0016ffd8(&csv, row, DAT_00387688);
            rec.risingSidespin = FUN_0016ffd8(&csv, row, DAT_00387694);
            rec.risingTechnique = FUN_0016ffd8(&csv, row, DAT_003876a4);
            rec.risingArea = FUN_0016ffd8(&csv, row, DAT_003876b4);
            rec.risingRigor = FUN_0016ff78(&csv, row, DAT_003876c0);
            rec.smashPower = FUN_0016ffd8(&csv, row, DAT_003876cc);
            rec.smashSpin = FUN_0016ffd8(&csv, row, DAT_003876d8);
            rec.smashSidespin = FUN_0016ffd8(&csv, row, DAT_003876e4);
            rec.smashTechnique = FUN_0016ffd8(&csv, row, DAT_003876f4);
            rec.smashRigor = FUN_0016ff78(&csv, row, DAT_00387704);
            rec.spinservePower = FUN_0016ffd8(&csv, row, DAT_00387710);
            rec.spinserveSpin = FUN_0016ffd8(&csv, row, DAT_00387720);
            rec.spinserveSidespin = FUN_0016ffd8(&csv, row, DAT_00387730);
            rec.spinserveTechnique = FUN_0016ffd8(&csv, row, DAT_00387744);
            rec.spinserveRigor = FUN_0016ff78(&csv, row, DAT_00387758);
            rec.sliceservePower = FUN_0016ffd8(&csv, row, DAT_00387768);
            rec.sliceserveSpin = FUN_0016ffd8(&csv, row, DAT_00387778);
            rec.sliceserveSidespin = FUN_0016ffd8(&csv, row, DAT_00387788);
            rec.sliceserveTechnique = FUN_0016ffd8(&csv, row, DAT_0038779c);
            rec.sliceserveRigor = FUN_0016ff78(&csv, row, DAT_003877b0);
            rec.flatservePower = FUN_0016ffd8(&csv, row, DAT_003877c0);
            rec.flatserveSpin = FUN_0016ffd8(&csv, row, DAT_003877d0);
            rec.flatserveSidespin = FUN_0016ffd8(&csv, row, DAT_003877e0);
            rec.flatserveTechnique = FUN_0016ffd8(&csv, row, DAT_003877f4);
            rec.flatserveRigor = FUN_0016ff78(&csv, row, DAT_00387808);
            rec.topForte = FUN_0016ff78(&csv, row, DAT_00387818);
            rec.sliceForte = FUN_0016ff78(&csv, row, DAT_00387824);
            rec.lobForte = FUN_0016ff78(&csv, row, DAT_00387830);
            rec.volleyForte = FUN_0016ff78(&csv, row, DAT_0038783c);
            rec.volleyWeakForte = FUN_0016ff78(&csv, row, DAT_00387848);
            rec.lowvolleyForte = FUN_0016ff78(&csv, row, DAT_00387858);
            rec.lowvolleyWeakForte = FUN_0016ff78(&csv, row, DAT_00387868);
            rec.highvolleyForte = FUN_0016ff78(&csv, row, DAT_0038787c);
            rec.flatForte = FUN_0016ff78(&csv, row, DAT_0038788c);
            rec.dropForte = FUN_0016ff78(&csv, row, DAT_00387898);
            rec.airshotForte = FUN_0016ff78(&csv, row, DAT_003878a4);
            rec.risingForte = FUN_0016ff78(&csv, row, DAT_003878b4);
            rec.smashForte = FUN_0016ff78(&csv, row, DAT_003878c0);
            rec.spinserveForte = FUN_0016ff78(&csv, row, DAT_003878cc);
            rec.sliceserveForte = FUN_0016ff78(&csv, row, DAT_003878dc);
            rec.flatserveForte = FUN_0016ff78(&csv, row, DAT_003878ec);
            max_id = rec.id < max_id ? max_id : rec.id;
            FUN_0016f9b8(self, rec);
        }
        self->index.resize(max_id + 1, DAT_003870bc);
        for (int i = 0; i < self->size; i++)
            self->index[(*self)[i].id] = i;
    }
    if (FUN_00014d64(path, DAT_003878fc) != 0) {
        DAT_003ea84c = (CharParamTable *)FUN_000008d8(FUN_00000a90(0x28), (void *)FUN_0016e76c, (void *)FUN_0016e78c, 0x18, 1);
        FUN_0016e8c4(DAT_003ea84c, DAT_0038790c);
        DAT_003ea850 = FUN_000008d8(FUN_00000a90(0x1c), (void *)FUN_0016cd1c, (void *)FUN_0016e864, 0xc, 1);
        FUN_0016cd30(DAT_003ea850);
    }
}
