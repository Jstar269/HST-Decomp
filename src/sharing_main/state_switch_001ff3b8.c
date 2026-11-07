/* HSTMATCH module=hstp_trans_main.prx start=0x001ff3b8 end=0x001ff4f0 symbol=FUN_001ff3b8 lang=c99 */
/* Hand-written reconstruction: clone of the source of state_switch_001f0fb4.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int DAT_003eaad8;
extern void FUN_00209384(void *);
extern void FUN_00201c30(void *);
extern void FUN_00202b14(void *, int);
extern void FUN_001f0be4(int);
extern void FUN_001f443c(int, int, int, unsigned char, int);
extern void FUN_001f5214(int, int);
extern int FUN_001c0bb0(int, int);

#define ENTRY_NONE 0x7f /* entry id meaning "no entry selected" */
#define ENTRY_SPECIAL 5 /* entry id that sets the special flag */

typedef struct Entry {
  int id;                 /* 0x0 */
  int arg;                /* 0x4 */
  unsigned char flag;     /* 0x8 */
  char pad9[3];
} Entry;

typedef struct Self {
  char pad0[0x510];
  int f510;               /* 0x510 */
  char pad514[0x5b8 - 0x514];
  int f5b8;               /* 0x5b8 */
  char pad5bc[0x12dc - 0x5bc];
  int f12dc;              /* 0x12dc */
  Entry entries[1];       /* 0x12e0, indexed by cur */
  char pad12ec[0x135c - 0x12ec];
  int cur;                /* 0x135c index of the current entry */
  int f1360;              /* 0x1360 */
  unsigned char f1364;    /* 0x1364 */
} Self;

void FUN_001ff3b8(Self *self)
{
  FUN_00209384(self);
  if (self->entries[self->cur].id != ENTRY_NONE) {
    FUN_00202b14(self, 0);
    FUN_001f0be4(self->f510);
    FUN_001f443c(self->f510,
                 self->entries[self->cur].id,
                 self->entries[self->cur].arg,
                 self->entries[self->cur].flag, 0);
    FUN_001f5214(self->f510, self->entries[self->cur].id);
    if (self->entries[self->cur].id != ENTRY_SPECIAL) {
      self->f1364 = 0;
    }
    else {
      self->f1364 = 1;
    }
    if (self->entries[self->cur].flag != 0) {
      self->f5b8 = FUN_001c0bb0((int)&DAT_003eaad8, 0xd);
    }
    else {
      self->f5b8 = FUN_001c0bb0((int)&DAT_003eaad8, 0xc);
    }
    self->f12dc = 1;
    self->f1360 = 0;
  }
  else {
    FUN_00201c30(self);
  }
}
