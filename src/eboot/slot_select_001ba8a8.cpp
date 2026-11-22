/* HSTMATCH module=EBOOT.BIN start=0x001ba8a8 end=0x001ba9b4 symbol=FUN_001ba8a8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Ent48 {
  unsigned char flag;
  char pad[0x2f];
};

struct ArrWrap {
  Ent48 *entries;
};

struct Info {
  char pad0[0x40];
  short s40;
  short s42;
  short s44;
  short s46;
  short s48;
  short s4a;
  char pad4c[0xa6 - 0x4c];
  unsigned char ba6;
  char pada7[0xdc - 0xa7];
  unsigned char bdc;
  unsigned char bdd;
  unsigned char bde;
};

struct Slot {
  int f0;
  int f4;
};

struct Obj {
  int f0;
  Slot **slots;
  char pad8[8];
  unsigned char f10[4];
  unsigned char f14[4];
};

extern "C" {
Info *FUN_001feec4(void);
unsigned char FUN_001baf80(int);
void FUN_00205df4(Slot *, unsigned char, int *, unsigned char, unsigned char, unsigned char, bool);
}

extern "C" void FUN_001ba8a8(Obj *self, int idx, ArrWrap *wrap)
{
  if (wrap->entries[idx].flag == 0)
    return;
  Info *info = FUN_001feec4();
  int first = info->s40;
  int arr[6] = { first };
  arr[1] = info->s42;
  arr[2] = info->s44;
  arr[3] = info->s46;
  arr[4] = info->s48;
  arr[5] = info->s4a;
  unsigned char r = FUN_001baf80(self->slots[idx]->f4);
  FUN_00205df4(self->slots[idx], r, arr, info->bde, info->bdc, info->bdd, info->ba6 == 2);
  self->f10[idx] = 1;
  self->f14[idx] = 1;
}
