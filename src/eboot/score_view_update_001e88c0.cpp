/* HSTMATCH module=EBOOT.BIN start=0x001e88c0 end=0x001e8bc4 symbol=FUN_001e88c0 lang=c++ */
/* Hand-written reconstruction from the disassembly; matching idiom: second side's set count read into a local through a const-reference accessor; exact match verified; names and types await human review. */
#include "hst/rule_obj.h"

/* Score display copied from the match rule object (DAT_00338b44). */
struct ScoreView {
    char pad0[0x398];
    int numSets;          /* 0x398 */
    int sets[2];          /* 0x39c */
    int resultGame[2][6]; /* 0x3a4 */
    int mode[2];          /* 0x3d4 */
    int state[2];         /* 0x3dc */
    int tally[2][6];      /* 0x3e4 */
};

extern RuleObj *DAT_00338b44;

extern "C" {
bool FUN_000def58(RuleObj *rule);
unsigned char FUN_000def0c(RuleObj *rule);
void FUN_000de100(RuleObj *rule, int *mode, unsigned char *flag, int *which);
int FUN_000dea20(RuleObj *rule, int team);
int FUN_000de51c(RuleObj *rule, int team);
int FUN_000de2c8(RuleObj *rule, int team);
int FUN_000de3f0(RuleObj *rule, int team);
void FUN_001e88c0(ScoreView *self, int init);
}

/* sets won by one side, read through a const reference (the reference keeps retail's load order) */
static inline const int &SetsWon(RuleObj *r, int side) { return r->sets[side]; }
static inline const int &Min(const int &a, const int &b) { return a < b ? a : b; }

void FUN_001e88c0(ScoreView *self, int init)
{
    unsigned char flag;
    int which;
    int half = FUN_000def58(DAT_00338b44);
    if (init) {
        int played;
        int total = DAT_00338b44->numSets;
        int won1 = SetsWon(DAT_00338b44, 1);
        played = (unsigned char)((half != 0) ^ 1) + (DAT_00338b44->sets[0] + won1);
        self->numSets = Min(total, played);
    }
    FUN_000de100(DAT_00338b44, self->mode, &flag, &which);
    for (int i = 0; i < 2; i++) {
        self->sets[i] = DAT_00338b44->sets[i];
        for (int j = 0; j < self->numSets; j++)
            self->resultGame[i][j] = DAT_00338b44->resultGame[i][j];
        if (flag) {
            self->mode[i] = 4;
        } else if (which == 0) {
            self->mode[0] = 5;
            self->mode[1] = 3;
        } else if (which == 1) {
            self->mode[0] = 3;
            self->mode[1] = 5;
        }
        if (FUN_000dea20(DAT_00338b44, i))
            self->state[i] = 3;
        else if (FUN_000dea20(DAT_00338b44, 1 - i))
            self->state[i] = 4;
        else if (FUN_000de51c(DAT_00338b44, i))
            self->state[i] = 2;
        else if (FUN_000de2c8(DAT_00338b44, i))
            self->state[i] = 0;
        else if (FUN_000de3f0(DAT_00338b44, i))
            self->state[i] = 1;
        else
            self->state[i] = -1;
    }
    unsigned char tie = FUN_000def0c(DAT_00338b44);
    if (init)
        self->tally[0][0] = FUN_000def0c(DAT_00338b44);
    if (!tie && self->tally[0][0]) {
        for (int i = 0; i < 2; i++)
            if (DAT_00338b44->unka4 == i)
                self->tally[i][self->numSets]++;
    } else if (self->tally[0][0]) {
        self->tally[0][self->numSets] = self->mode[0];
        self->tally[1][self->numSets] = self->mode[1];
    }
}
