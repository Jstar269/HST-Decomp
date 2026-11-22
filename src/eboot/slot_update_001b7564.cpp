/* HSTMATCH module=EBOOT.BIN start=0x001b7564 end=0x001b7664 symbol=FUN_001b7564 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Vec4 {
  float x, y, z, w;
} __attribute__((aligned(16)));

struct Mat4 {
  Vec4 r0, r1, r2, r3;
};

struct Sub {
  char pad[0x58];
  void *f58;
};

struct Player {
  char pad[0x5bc];
  Sub *f5bc;
};

struct Game {
  char pad[0x70];
  Player *players[4];
};

struct Obj {
  char pad[0x5c];
  int *handles;
  unsigned char *flags;
};

extern "C" {
extern Game *DAT_003386b0;
extern char DAT_002e99f8[];
extern char DAT_0034179c;
void FUN_0005548c(Mat4 *, void *, const char *);
void FUN_001b2218(void *, int *, Vec4 *, int, int);
void FUN_001b2438(void *, int *);
}

extern "C" void FUN_001b7564(Obj *self)
{
  for (int i = 0; i < 4; i++) {
    if (self->handles[i] >= 0) {
      if (self->flags[i] != 0) {
        Player *pl = DAT_003386b0 ? DAT_003386b0->players[i] : 0;
        if (pl != 0 && pl->f5bc != 0) {
          Vec4 pos;
          Mat4 mat;
          FUN_0005548c(&mat, pl->f5bc->f58, DAT_002e99f8);
          pos = mat.r3;
          FUN_001b2218(&DAT_0034179c, &self->handles[i], &pos, 0, 1);
        }
      } else {
        FUN_001b2438(&DAT_0034179c, &self->handles[i]);
      }
    }
  }
}
