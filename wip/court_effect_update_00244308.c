/* HSTMATCH module=EBOOT.BIN start=0x00244308 end=0x002444b0 symbol=FUN_00244308 lang=c99 */
#include "hst/types.h"

struct GlobalSeedHolder {
    char pad[0x1360];
    int seed;
};

struct Obj_00244308 {
    char pad000[0x260];
    int f260;            // 0x260
    char pad264[0x280 - 0x264];
    char f280[0x2d8 - 0x280]; // 0x280
    int f2d8;            // 0x2d8
    char pad2dc[0x300 - 0x2dc];
    unsigned char f300;  // 0x300
};

extern struct GlobalSeedHolder *DAT_003402a0;
extern char DAT_0034179c[];
extern const char DAT_002f8458[];
extern const char DAT_002f8464[];
extern const char DAT_002f8470[];

extern int FUN_001b1f7c(void *sys, int handle);
extern int FUN_001b28f0(void *sys, const char *name);
extern int FUN_001b2218(void *sys, void *handle_ptr, void *pos, int a3, int t0);

void FUN_00244308(struct Obj_00244308 *self)
{
    if (self->f300 != 0) {
        if (self->f260 == 32) {
            goto radioheri;
        }
        if (self->f260 == 31 || self->f260 == 11) {
            int seed = DAT_003402a0->seed * 0x343fd + 0x269ec3;
            DAT_003402a0->seed = seed;
            if (((seed >> 16) & 0x7fff) % 300 == 0) {
                int local_4 = FUN_001b28f0(DAT_0034179c, DAT_002f8458);
                FUN_001b2218(DAT_0034179c, &local_4, &self->f280, 0, 0);
            }
            if (FUN_001b1f7c(DAT_0034179c, self->f2d8) == 0) {
                self->f2d8 = FUN_001b28f0(DAT_0034179c, DAT_002f8464);
                FUN_001b2218(DAT_0034179c, &self->f2d8, &self->f280, 0, 0);
            } else {
                FUN_001b2218(DAT_0034179c, &self->f2d8, &self->f280, 0, 0);
            }
        radioheri:
            if (FUN_001b1f7c(DAT_0034179c, self->f2d8) == 0) {
                self->f2d8 = FUN_001b28f0(DAT_0034179c, DAT_002f8470);
                FUN_001b2218(DAT_0034179c, &self->f2d8, &self->f280, 0, 0);
            } else {
                FUN_001b2218(DAT_0034179c, &self->f2d8, &self->f280, 0, 0);
            }
        }
    }
}
// Near miss: 104/106 words.
// Retail emits:
//   beq a0, 31, cars; nop; beq a0, 11, cars; nop; b exit; lw ra, 12(sp)
// Whereas MWCC optimizes the second condition into:
//   beq a0, 31, cars; nop; bnel a0, 11, exit; lw ra, 12(sp)
// which saves 2 words (8 bytes) and shifts the remaining 100+ words by 2 words.
// Ruled out: switch/case fallthrough, goto chains, do-while wrappers, C++ mode.
// near miss from lane dsn7 (unreviewed)
