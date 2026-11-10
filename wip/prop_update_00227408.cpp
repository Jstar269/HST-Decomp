/* HSTMATCH module=EBOOT.BIN start=0x00227408 end=0x00228600 symbol=FUN_00227408 lang=c++ rodata=0x002c9a30 */
/* Vector with an out-of-line copy constructor (0x4c644), used by the collision hit record. */
struct Vec4 {
  float x, y, z, w;
  Vec4() {}
  Vec4(const Vec4 &v);
} __attribute__((aligned(16)));

struct TriKey {
  Vec4 v[3];
  int type;
  int id;
  TriKey() {}
  TriKey(const TriKey &k);
};

struct Hit {
  Vec4 pos;
  Vec4 normal;
  Vec4 c;
  float m30;
  float m34;
  int m38;
  int m3c;
  int m40;
  TriKey key;
};

/* Plain row vector of the matrix. */
struct Row {
  float x, y, z, w;
} __attribute__((aligned(16)));

struct Mat4 {
  Row r0, r1, r2, r3;
  Mat4() {}
  Mat4(const Mat4 &o) { r0 = o.r0; r1 = o.r1; r2 = o.r2; r3 = o.r3; }
};

/* Vector with an inline memberwise copy constructor. */
struct V4 {
  float x, y, z, w;
  V4() {}
  V4(const V4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));

struct Clip {
  char pad0[8];
  float length;
};
struct Anim {
  Clip *clip;
};
struct Track {
  Anim *anim;   /* 0x00 */
  char pad04[0xc];
  float time;   /* 0x10 */
  char pad14[0x34 - 0x14];
};

struct Prop {
  char pad00[0x3c];
  int state;            /* 0x3c */
  V4 pos;               /* 0x40 */
  char pad50[0x10];
  char mat[0x30];       /* 0x60 */
  V4 unk90;             /* 0x90 */
  int unka0;            /* 0xa0 */
  char pada4[0xc];
  char unkb0[0x60];     /* 0xb0 */
  V4 center;            /* 0x110 */
  char pad120[0x10];
  float unk130;         /* 0x130 */
  char pad134[0x260 - 0x134];
  char anim[0xc0];      /* 0x260 */
  unsigned char unk320; /* 0x320 */
  char pad321[3];
  float unk324;         /* 0x324 */
  char pad328[8];
  float unk330;         /* 0x330 */
  char pad334[0x354 - 0x334];
  Track tracks[1];      /* 0x354 */
  char pad388[0x3b8 - 0x388];
  int first;            /* 0x3b8 */
  int last;             /* 0x3bc */
  char pad3c0[0x414 - 0x3c0];
  void *unk414;         /* 0x414 */
  char pad418[0x428 - 0x418];
  int kind;             /* 0x428 */
  int unk42c;           /* 0x42c */
  int pad430;
  unsigned char active; /* 0x434 */
  unsigned char unk435; /* 0x435 */
  char pad436[0x440 - 0x436];
  V4 vel;               /* 0x440 */
  char pad450[0x10];
  int counter;          /* 0x460 */
  int Last() const { return last; }
};

struct Player {
  char pad[0x5bc];
  void *body;
};
struct Court {
  char pad0[0x20];
  V4 v20;      /* 0x20 */
  char pad30[0x140 - 0x30];
  Hit hit;     /* 0x140 */
  char pad1d0[0x2f0 - 0x1d0];
  V4 v2f0;     /* 0x2f0 */
};
struct Deep {
  char pad[0x58];
  int unk58;
};
struct Mid {
  char pad0[0x44];
  void *fx;
};
struct Ctx {
  char pad0[0x70];
  Player *players[4]; /* 0x70 */
  char pad80[8];
  Court *court;       /* 0x88 */
  char pad8c[4];
  Mid *mid;           /* 0x90 */
  char pad94[0x9c - 0x94];
  Deep *deep;         /* 0x9c */
};

extern "C" {
extern Ctx *DAT_003386b0;
extern float DAT_002f2a44[];
extern float DAT_002f29dc[];
extern const Mat4 DAT_002bc410;
void FUN_00055044(void *, void *);
void FUN_00022498(float, V4 *, const V4 *);
V4 *FUN_00022434(V4 *, const V4 *, const V4 *);
float FUN_000224d8(const V4 *, const V4 *);
float FUN_00022e44(float);
V4 *FUN_00093838(void *);
double FUN_0000b16c(double);
void FUN_00228600(Prop *, int);
void FUN_002289a4(Prop *);
void FUN_000ce754(void *, Mat4 *, int, float, float, int, float);
void FUN_00057b58(float, void *, int, int);
void FUN_00058ec4(float, void *, int);
void FUN_00058ddc(void *, unsigned char, int);
void FUN_00059084(void *, int);
void FUN_00058f38(float, void *, int);
void FUN_0005f110(float, void *);
void FUN_00058fc0(void *, int);
void FUN_00059174(void *);
void FUN_0005f0d8(void *);
}

static inline V4 Scale(float s, V4 v)
{
  V4 r;
  FUN_00022498(s, &r, &v);
  return r;
}

static inline V4 Sub(const V4 &a, const V4 &b)
{
  V4 r;
  FUN_00022434(&r, &a, &b);
  return r;
}

static inline float Len(const V4 &v)
{
  return FUN_00022e44(FUN_000224d8(&v, &v));
}

static inline void SetState(Prop *self, int s)
{
  if (self->state != s) {
    self->state = s;
    self->active = 1;
  }
}

static inline bool TracksDone(Prop *self)
{
  for (int i = self->first; i <= self->Last(); i++) {
    float len = self->tracks[i].anim ? self->tracks[i].anim->clip->length : 0.0f;
    if (self->tracks[i].time < len)
      return false;
  }
  return true;
}

extern "C" void FUN_00227408(Prop *self)
{
  void *fx = DAT_003386b0->mid->fx;
  Mat4 mat = DAT_002bc410;
  switch (self->state) {
  case 0: {
    SetState(self, 1);
    FUN_00055044(self->unk414, self->mat);
    FUN_00055044(self->unkb0, self->mat);
    int m = self->kind;
    if (m == 1 || m == 23 || m == 24 || m == 25 || m == 11 || m == 15)
      FUN_00055044(self->anim, self->mat);
    break;
  }
  case 1: {
    self->active = 0;
    Hit hit = DAT_003386b0->court->hit;
    int k;
    if ((self->unka0 == 0 && hit.m40 == (int)self->unk414) ||
        (((k = self->kind) == 15 || k == 11 || k == 9 || k == 1 || (unsigned)(k - 23) < 3) &&
         self->unk42c == DAT_003386b0->deep->unk58)) {
      int m = self->kind;
      if (m == 1 || m == 23 || m == 24 || m == 25 || m == 11 || m == 15) {
        SetState(self, 3);
        self->unk42c = DAT_003386b0->deep->unk58;
      } else {
        SetState(self, 2);
        self->unk42c = DAT_003386b0->deep->unk58;
        self->vel = Scale(DAT_002f2a44[self->kind], DAT_003386b0->court->v20);
        float a = (float)FUN_0000b16c((double)DAT_002f29dc[self->kind]);
        float b = (float)FUN_0000b16c((double)self->vel.y);
        if (a > b)
          self->vel.y = DAT_002f29dc[self->kind];
        else
          self->vel.y = -self->vel.y;
      }
    } else if (hit.m40 == (int)self->unk414) {
      FUN_00228600(self, 0);
      if (self->kind == 13) {
        mat = DAT_002bc410;
        mat.r3.x = self->pos.x;
        mat.r3.y = self->pos.y;
        mat.r3.z = self->pos.z;
        mat.r3.w = self->pos.w;
        FUN_000ce754(fx, &mat, 60, 3500.0f, 1.0f, 1, 0.0f);
      } else if (self->kind == 18) {
        mat = DAT_002bc410;
        mat.r3.x = self->pos.x;
        mat.r3.y = self->pos.y;
        mat.r3.z = self->pos.z;
        mat.r3.w = self->pos.w;
        FUN_000ce754(fx, &mat, 61, 3500.0f, 1.0f, 1, 0.0f);
      }
    }
    if (self->kind == 13) {
      self->counter++;
      self->unk130 = (float)self->counter;
      if (self->counter >= 600) {
        self->counter = 0;
        self->unk130 = 0.0f;
      }
    }
    break;
  }
  case 2:
    if (self->active) {
      mat = DAT_002bc410;
      mat.r3.y = 10.0f;
      FUN_00055044(self->unk414, &mat);
      V4 pos = self->center;
      int variant = 1;
      V4 ball = DAT_003386b0->court->v2f0;
      if (Len(Sub(pos, ball)) < 1.5f)
        variant = 1;
      for (int i = 0; i < 4; i++) {
        Player *p = DAT_003386b0->players[i];
        if (p) {
          V4 other = *FUN_00093838(p->body);
          if (Len(Sub(pos, other)) < 1.5f)
            variant = 2;
        }
      }
      if (variant == 1) {
        mat = DAT_002bc410;
        mat.r3.x = self->center.x;
        mat.r3.y = self->center.y;
        mat.r3.z = self->center.z;
        mat.r3.w = self->center.w;
        FUN_000ce754(fx, &mat, 0x203, 3500.0f, 1.0f, 1, 0.0f);
        FUN_000ce754(fx, &mat, 0x227, 3500.0f, 1.0f, 1, 0.0f);
      } else if (variant == 2) {
        mat = DAT_002bc410;
        mat.r3.x = self->center.x;
        mat.r3.y = self->center.y;
        mat.r3.z = self->center.z;
        mat.r3.w = self->center.w;
        FUN_000ce754(fx, &mat, 0x22c, 3500.0f, 1.0f, 1, 0.0f);
        FUN_000ce754(fx, &mat, 0x227, 3500.0f, 1.0f, 1, 0.0f);
      }
    }
    FUN_00228600(self, 0);
    self->active = 0;
    FUN_002289a4(self);
    break;
  case 3:
    if (self->active) {
      FUN_00057b58(0.0f, self->anim, 0, 0);
      FUN_00058ec4(1.0f, self->anim, -1);
      self->unk324 = 1.0f;
      FUN_00058ddc(self->anim, 0, -1);
      self->unk320 = 0;
      FUN_00059084(self->anim, -1);
      self->unk330 = self->unk324;
      
      mat = DAT_002bc410;
      mat.r3.y = 10.0f;
      FUN_00055044(self->unk414, &mat);
      V4 pos = self->center;
      int variant = 1;
      V4 ball = DAT_003386b0->court->v2f0;
      if (Len(Sub(pos, ball)) < 1.5f)
        variant = 1;
      for (int i = 0; i < 4; i++) {
        Player *p = DAT_003386b0->players[i];
        if (p) {
          V4 other = *FUN_00093838(p->body);
          if (Len(Sub(pos, other)) < 1.5f)
            variant = 2;
        }
      }
      if (variant == 1) {
        mat = DAT_002bc410;
        mat.r3.x = self->center.x;
        mat.r3.y = self->center.y;
        mat.r3.z = self->center.z;
        mat.r3.w = self->center.w;
        FUN_000ce754(fx, &mat, 0x203, 3500.0f, 1.0f, 1, 0.0f);
        FUN_000ce754(fx, &mat, 0x227, 3500.0f, 1.0f, 1, 0.0f);
      } else if (variant == 2) {
        mat = DAT_002bc410;
        mat.r3.x = self->center.x;
        mat.r3.y = self->center.y;
        mat.r3.z = self->center.z;
        mat.r3.w = self->center.w;
        FUN_000ce754(fx, &mat, 0x22c, 3500.0f, 1.0f, 1, 0.0f);
        FUN_000ce754(fx, &mat, 0x227, 3500.0f, 1.0f, 1, 0.0f);
      }
    }
    FUN_00228600(self, 0);
    self->active = 0;
    if (TracksDone(self)) {
      FUN_00058f38(0.0f, self->anim, -1);
      FUN_0005f110(0.0f, self->anim);
      FUN_00058fc0(self->anim, -1);
      FUN_0005f110(0.0f, self->anim);
      self->unk330 = 0.0f;
      SetState(self, 5);
    }
    FUN_00059174(self->anim);
    FUN_0005f0d8(self->anim);
    break;
  case 4:
    if (self->active) {
      self->pos.y = 10.0f;
      self->unk90 = self->pos;
      FUN_00055044(self->unkb0, self->mat);
      FUN_00055044(self->unk414, self->mat);
      self->unk435 = 0;
    }
    self->active = 0;
    break;
  case 5:
    if (self->active) {
      self->pos.y = 10.0f;
      self->unk90 = self->pos;
      FUN_00055044(self->unkb0, self->mat);
      FUN_00055044(self->unk414, self->mat);
      self->unk435 = 0;
    }
    self->active = 0;
    break;
  }
}
// NEAR (180 differing words, all saved-register colouring): every instruction matches retail except three values
// swap saved registers: self (retail s0, ours s1), fx = DAT_003386b0->mid->fx (retail s2, ours s0) and the Hit
// copy source &court->hit (retail s1, ours s2); i/variant (s3/s4) already match. Ruled out (all stay 180):
// a named Hit* local (case scope or declared at the top), member-function form (this or a self copy), inline
// predicates or plain self->kind instead of the int m/k locals, inline helpers for cases 0/4/5, no SetState /
// TracksDone inlines, an inline Update(self, fx) body. Declaring mat before fx (or splitting fx's declaration
// from its load, or Mat4 mat; mat = ...) turns retail's 8-pass lw/sw block-copy loop into a per-field copy.
// The copy forms: `Mat4 mat = DAT_002bc410;` with a memberwise copy ctor whose BODY assigns the rows (mem-init
// form gives a per-field copy here) right after the fx load = retail's loop; `mat = DAT_002bc410;` = the
// pipelined copy with one lw/sw element.
// near miss from lane fe1 (unreviewed)
