/* HSTMATCH module=EBOOT.BIN start=0x0023ffb4 end=0x00241164 symbol=FUN_0023ffb4 lang=c++ */
#pragma inline_depth(8)
struct V4 {
  float x, y, z, w;
  V4() {}
  V4(float ax, float ay, float az, float aw) : x(ax), y(ay), z(az), w(aw) {}
  V4(const V4 &o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
} __attribute__((aligned(16)));

struct Row {
  float x, y, z, w;
} __attribute__((aligned(16)));

struct Mat4 {
  Row r0, r1, r2, r3;
  Mat4() {}
};

struct Clip {
  char pad0[8];
  float length;
};
struct AnimRef {
  Clip *clip;
};
struct Track {
  AnimRef *anim; /* 0x00 */
  char pad04[0xc];
  float time;    /* 0x10 */
  char pad14[0x34 - 0x14];
};

struct Anim {
  char pad00[0x60];
  V4 pos;        /* 0x60 (object 0xc0) */
  char pad70[0xc0 - 0x70];
};

struct Obj {
  char pad00[0x3c];
  unsigned char alive;  /* 0x3c */
  char pad3d[3];
  int mode;             /* 0x40 */
  int pad44;
  unsigned char dirty;  /* 0x48 */
  char pad49[3];
  int time;             /* 0x4c */
  int counter;          /* 0x50 */
  char pad54[0xc];
  Anim anim;            /* 0x60 */
  unsigned char f120;   /* 0x120 */
  char pad121[3];
  float f124;           /* 0x124 */
  char pad128[8];
  float f130;           /* 0x130 */
  char pad134[0x154 - 0x134];
  Track tracks[1];      /* 0x154 */
  char pad188[0x1b8 - 0x188];
  int first;            /* 0x1b8 */
  int last;             /* 0x1bc */
  char pad1c0[0x220 - 0x1c0];
  Mat4 mat;             /* 0x220 */
  int state;            /* 0x260 */
  int f264;             /* 0x264 */
  unsigned char f268;   /* 0x268 */
  char pad269[0x280 - 0x269];
  V4 pos;               /* 0x280 */
  V4 home;              /* 0x290 */
  V4 **targets;         /* 0x2a0 */
  char pad2a4[0x2ac - 0x2a4];
  int kind;             /* 0x2ac */
  int pad2b0;
  int limit;            /* 0x2b4 */
  char pad2b8[0x2d8 - 0x2b8];
  int sound;            /* 0x2d8 */
  char pad2dc[0x324 - 0x2dc];
  int f324;             /* 0x324 */
  unsigned char f328;   /* 0x328 */
  unsigned char f329;   /* 0x329 */
  int Last() const { return last; }
};

struct Player {
  char pad[0x5bc];
  void *body;
};
struct Court {
  char pad0[0x2f0];
  V4 v2f0; /* 0x2f0 */
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
struct Flags {
  char pad[0xb0];
  unsigned char fb0;
};
struct Rng {
  char pad[0x1360];
  int seed;
};

extern "C" {
extern Ctx *DAT_003386b0;
extern Flags *DAT_00338b44;
extern Rng *DAT_003402a0;
extern void *DAT_00340284;
extern char DAT_0034179c[];
extern int DAT_002f8420;
extern char DAT_002f8424[];
extern char DAT_002f8430[];
extern char DAT_002f843c[];
extern char DAT_002f844c[];
extern const Mat4 DAT_002bc410;
int *FUN_0005787c(void *, int);
void FUN_00057b58(float, void *, int, int);
void FUN_00058ec4(float, void *, int);
void FUN_00058ddc(void *, unsigned char, int);
void FUN_0005f030(float, void *, int);
void FUN_00058f38(float, void *, int);
void FUN_0005f110(float, void *);
void FUN_00058fc0(void *, int);
void FUN_00059084(void *, int);
void FUN_00059174(void *);
void FUN_0005f0d8(void *);
void FUN_00055044(void *, void *);
void FUN_00244308(Obj *);
void FUN_00241194(Obj *);
int FUN_002457a0(Obj *);
void FUN_00242268(Obj *);
void FUN_002444b0(Obj *);
void FUN_00243928(Obj *);
int FUN_001b28f0(char *, char *);
void FUN_001b2218(char *, int *, V4 *, int, int);
int FUN_001b1f7c(char *, int);
void FUN_001b1f84(char *, int *);
void FUN_0013076c(void *, V4 *, int, void *, int);
void FUN_00022518(V4 *, const V4 *);
void FUN_00022498(float, V4 *, const V4 *);
V4 *FUN_00022434(V4 *, const V4 *, const V4 *);
float FUN_000224d8(const V4 *, const V4 *);
float FUN_00022e44(float);
V4 *FUN_00093838(void *);
void FUN_000ce754(void *, Mat4 *, int, float, float, int, float);
void FUN_00090144(Court *, const V4 &, const V4 &, float, float, int *, int);
}

static inline V4 Sub(const V4 &a, const V4 &b)
{
  V4 r;
  FUN_00022434(&r, &a, &b);
  return r;
}

static inline V4 Scale(float s, V4 v)
{
  V4 r;
  FUN_00022498(s, &r, &v);
  return r;
}

static inline V4 Normalize(const V4 &v)
{
  V4 r;
  FUN_00022518(&r, &v);
  r.w = 0.0f;
  return r;
}

static inline float Len(const V4 &v)
{
  return FUN_00022e44(FUN_000224d8(&v, &v));
}

static inline void SetMode(Obj *self, int m)
{
  if (self->mode != m) {
    self->mode = m;
    self->dirty = 1;
  }
}

static inline bool TracksDone(Obj *self)
{
  for (int i = self->first; i <= self->Last(); i++) {
    float len = self->tracks[i].anim ? self->tracks[i].anim->clip->length : 0.0f;
    if (self->tracks[i].time < len)
      return false;
  }
  return true;
}

extern "C" void FUN_0023ffb4(Obj *self)
{
  if (DAT_00338b44->fb0)
    self->f268 = 1;
  if (!self->alive)
    return;
  switch (self->mode) {
  case 0:
    if (self->dirty) {
      if (*FUN_0005787c(&self->anim, 0) != 0) {
        FUN_00057b58(0.0f, &self->anim, 0, 0);
        FUN_00058ec4(1.0f, &self->anim, -1);
        self->f124 = 1.0f;
        FUN_00058ddc(&self->anim, 1, -1);
        self->f120 = 1;
        if (self->state == 36) {
          FUN_0005f030(0.0f, &self->anim, 1);
          self->f124 = 1.0f;
        }
        FUN_00058f38(0.0f, &self->anim, -1);
        FUN_0005f110(0.0f, &self->anim);
      }
      FUN_00055044(&self->anim, &self->mat);
    }
    if (*FUN_0005787c(&self->anim, 0) != 0) {
      FUN_00059174(&self->anim);
      FUN_0005f0d8(&self->anim);
    }
    self->dirty = 0;
    SetMode(self, 1);
    break;
  case 1:
    if (self->dirty) {
      if (self->kind == 6) {
        if (*FUN_0005787c(&self->anim, 2) != 0) {
          FUN_00057b58(0.0f, &self->anim, 0, 2);
          FUN_00058ec4(1.0f, &self->anim, -1);
          self->f124 = 1.0f;
          FUN_00058ddc(&self->anim, 1, -1);
          self->f120 = 1;
          FUN_00058f38(0.0f, &self->anim, -1);
          FUN_0005f110(0.0f, &self->anim);
          FUN_00059084(&self->anim, -1);
          self->f130 = self->f124;
        }
      } else if (self->kind == 8) {
        if (*FUN_0005787c(&self->anim, 0) != 0) {
          FUN_00057b58(0.0f, &self->anim, 0, 0);
          FUN_00058ec4(1.0f, &self->anim, -1);
          self->f124 = 1.0f;
          FUN_00058ddc(&self->anim, 0, -1);
          self->f120 = 0;
          FUN_00058f38(0.0f, &self->anim, -1);
          FUN_0005f110(0.0f, &self->anim);
          FUN_00058fc0(&self->anim, -1);
          FUN_0005f110(0.0f, &self->anim);
          self->f130 = 0.0f;
        }
      } else {
        if (*FUN_0005787c(&self->anim, 0) != 0) {
          FUN_00057b58(0.0f, &self->anim, 0, 0);
          FUN_00058ec4(1.0f, &self->anim, -1);
          self->f124 = 1.0f;
          FUN_00058ddc(&self->anim, 1, -1);
          self->f120 = 1;
          int seed = DAT_003402a0->seed * 0x343fd + 0x269ec3;
          DAT_003402a0->seed = seed;
          float frame = (float)(((seed >> 16) & 0x7fff) % 20);
          FUN_00058f38(frame, &self->anim, -1);
          FUN_0005f110(frame, &self->anim);
          FUN_00059084(&self->anim, -1);
          self->f130 = self->f124;
        }
      }
      FUN_00055044(&self->anim, &self->mat);
    }
    if (*FUN_0005787c(&self->anim, 0) != 0 && self->kind != 8) {
      FUN_00059174(&self->anim);
      FUN_0005f0d8(&self->anim);
    }
    FUN_00244308(self);
    self->dirty = 0;
    if (self->kind == 3 || self->kind == 6)
      FUN_00241194(self);
    if (self->kind == 2 && self->f328) {
      self->pos = *self->targets[1];
      self->mat.r3.x = self->pos.x;
      self->mat.r3.y = self->pos.y;
      self->mat.r3.z = self->pos.z;
      self->mat.r3.w = self->pos.w;
      FUN_00055044(&self->anim, &self->mat);
    } else if (self->kind == 2 && !self->f328) {
      self->pos = *self->targets[0];
      self->mat.r3.x = self->pos.x;
      self->mat.r3.y = self->pos.y;
      self->mat.r3.z = self->pos.z;
      self->mat.r3.w = self->pos.w;
      FUN_00055044(&self->anim, &self->mat);
    }
    if (FUN_002457a0(self) == 0)
      return;
    if (self->state == 36 && self->f324 == 0) {
      FUN_00057b58(0.0f, &self->anim, 0, 2);
      FUN_00058ec4(1.0f, &self->anim, -1);
      self->f124 = 1.0f;
      FUN_00058ddc(&self->anim, 1, -1);
      self->f120 = 1;
      FUN_00058f38(0.0f, &self->anim, -1);
      FUN_0005f110(0.0f, &self->anim);
      FUN_0005f030(0.0f, &self->anim, 0);
      self->f124 = 1.0f;
      FUN_00058ddc(&self->anim, 1, -1);
      self->f120 = 1;
      FUN_00059084(&self->anim, -1);
      self->f130 = self->f124;
      self->f324 = 1;
      return;
    }
    if (self->state == 36 && self->f324 == 1)
      return;
    if (self->kind == 7) {
      bool near = false;
      int i = 0;
      const V4 &c = self->anim.pos;
      for (; i < 4; i++) {
        Player *p = DAT_003386b0->players[i];
        V4 pos = c;
        if (p) {
          V4 other = *FUN_00093838(p->body);
          if (Len(Sub(pos, other)) < 2.0f)
            near = true;
        }
      }
      if (self->f264 == 1 && !near) {
        self->counter = 0;
        SetMode(self, 2);
        self->time = DAT_003386b0->deep->unk58;
      }
    } else {
      self->counter = 0;
      SetMode(self, 2);
      self->time = DAT_003386b0->deep->unk58;
    }
    break;
  case 2:
    if (self->dirty) {
      if (self->state == 14) {
        if (self->f264 == 1) {
          FUN_00057b58(0.0f, &self->anim, 0, 2);
          FUN_00058ddc(&self->anim, 0, -1);
          self->f120 = 0;
          FUN_001b28f0(DAT_0034179c, DAT_002f8424);
        } else {
          FUN_00057b58(0.0f, &self->anim, 0, 1);
          FUN_00058ddc(&self->anim, 1, -1);
          self->f120 = 1;
        }
      } else if (self->state == 36) {
        FUN_00057b58(0.0f, &self->anim, 0, 1);
        FUN_00058ddc(&self->anim, 0, -1);
        self->f120 = 0;
      } else if (self->kind == 7) {
        FUN_00057b58(0.0f, &self->anim, 0, 1);
        FUN_00058ddc(&self->anim, 0, -1);
        self->f120 = 0;
        int one = 1;
        V4 d = Sub(V4(0.0f, -10.0f, 0.0f, 1.0f), DAT_003386b0->court->v2f0);
        Mat4 m = self->mat;
        m.r3.x = 0.0f;
        m.r3.y = 0.0f;
        m.r3.z = 0.0f;
        m.r3.w = 1.0f;
        { V4 n = Normalize(d); d = Scale(1.0f / 3600.0f, Scale(1000.0f, Scale(190.0f, n))); }
        m = DAT_002bc410;
        m.r3.x = DAT_003386b0->court->v2f0.x;
        m.r3.y = DAT_003386b0->court->v2f0.y;
        m.r3.z = DAT_003386b0->court->v2f0.z;
        m.r3.w = DAT_003386b0->court->v2f0.w;
        FUN_000ce754(DAT_003386b0->mid->fx, &m, 0x203, 3500.0f, 1.0f, 1, 0.0f);
        FUN_00090144(DAT_003386b0->court, DAT_003386b0->court->v2f0, d, 0.0f, 0.0f, &one, -1);
      } else if (self->kind == 8) {
        FUN_00057b58(0.0f, &self->anim, 0, 0);
        FUN_00058ec4(1.0f, &self->anim, -1);
        self->f124 = 1.0f;
        FUN_00058ddc(&self->anim, 0, -1);
        self->f120 = 0;
        FUN_00058f38(0.0f, &self->anim, -1);
        FUN_0005f110(0.0f, &self->anim);
        FUN_00059084(&self->anim, -1);
        self->f130 = self->f124;
      } else if (self->state == 43) {
        FUN_00057b58(0.0f, &self->anim, 0, 1);
        FUN_00058ec4(1.0f, &self->anim, -1);
        self->f124 = 1.0f;
        FUN_00058ddc(&self->anim, 0, -1);
        self->f120 = 0;
        FUN_00058f38(0.0f, &self->anim, -1);
        FUN_0005f110(0.0f, &self->anim);
        FUN_00059084(&self->anim, -1);
        self->f130 = self->f124;
      } else {
        if (self->state == 37)
          self->sound = FUN_001b28f0(DAT_0034179c, DAT_002f8430);
        FUN_00057b58(0.0f, &self->anim, 0, 1);
        FUN_00058ddc(&self->anim, 1, -1);
        self->f120 = 1;
      }
      FUN_00058ec4(1.0f, &self->anim, -1);
      self->f124 = 1.0f;
      FUN_00058f38(0.0f, &self->anim, -1);
      FUN_0005f110(0.0f, &self->anim);
      FUN_00059084(&self->anim, -1);
      self->f130 = self->f124;
    }
    FUN_00059174(&self->anim);
    FUN_0005f0d8(&self->anim);
    if (self->f264 == 1 && self->state == 14) {
      FUN_00242268(self);
    } else if (self->state == 36 || self->kind == 7) {
      FUN_002444b0(self);
    } else {
      if (self->kind == 8 && TracksDone(self)) {
        FUN_00057b58(0.0f, &self->anim, 0, 1);
        FUN_00058ec4(1.0f, &self->anim, -1);
        self->f124 = 1.0f;
        FUN_0005f030(0.0f, &self->anim, 0);
        self->f124 = 1.0f;
        FUN_00058ddc(&self->anim, 1, -1);
        self->f120 = 1;
        FUN_00058f38(0.0f, &self->anim, -1);
        FUN_0005f110(0.0f, &self->anim);
        FUN_00059084(&self->anim, -1);
        self->f130 = self->f124;
        self->counter = 0;
        self->f329 = 1;
        if (self->state == 44) {
          int h = FUN_001b28f0(DAT_0034179c, DAT_002f843c);
          FUN_001b2218(DAT_0034179c, &h, &self->pos, 0, 0);
        }
      } else if (self->kind == 8 && self->f329 == 0) {
        FUN_00242268(self);
      } else if (self->state == 43) {
        FUN_002444b0(self);
      } else {
        FUN_00243928(self);
        FUN_00242268(self);
        FUN_002444b0(self);
      }
    }
    if (self->state == 44) {
      if ((int)self->tracks[0].time == DAT_002f8420) {
        int h = FUN_001b28f0(DAT_0034179c, DAT_002f844c);
        FUN_001b2218(DAT_0034179c, &h, &self->pos, 0, 0);
      }
    }
    if (self->state == 14) {
      if (self->f264 == 1 && TracksDone(self)) {
        FUN_00057b58(0.0f, &self->anim, 0, 3);
        FUN_00058ddc(&self->anim, 1, -1);
        self->f120 = 1;
        FUN_00058ec4(1.0f, &self->anim, -1);
        self->f124 = 1.0f;
        FUN_00059084(&self->anim, -1);
        self->f130 = self->f124;
      }
    }
    self->dirty = 0;
    if (++self->counter > self->limit) {
      if (self->state != 43) {
        if (self->kind == 4 || self->kind == 5 || self->kind == 7)
          self->counter = 0;
        else
          self->alive = 0;
      }
      SetMode(self, 1);
      self->pos = self->home;
      FUN_00058fc0(&self->anim, -1);
      FUN_0005f110(0.0f, &self->anim);
      self->f130 = 0.0f;
      if (FUN_001b1f7c(DAT_0034179c, self->sound))
        FUN_001b1f84(DAT_0034179c, &self->sound);
    }
    if (self->alive)
      FUN_0013076c(DAT_00340284, &self->pos, self->state, &self->anim, 0);
    break;
  }
}
// NEAR MISS: 1132/1132 words, 10 differ, all in the kind==7 block (0x240924..0x2409b4): only the stack slots of
// the three nested Scale() result temps. Ours: Scale(190) r@48, Scale(1000) r@64, Scale(1/3600) r@80; retail has
// them in pre-order (outer r@48, Scale(1000)@64, Scale(190)@80). Normalize r@96 and the by-value copies
// 144/160/176 already match. Everything else in the function is identical.
// Needs `#pragma inline_depth(8)` (else Normalize/V4 ctors stay out of line).
// Ruled out (toys + real compiles): single nested statement (both Normalize and Scale r's reversed, 20 words);
// one named statement per call {a=N; b=S(a); c=S(b); d=S(c)} (r's right, copies 144/160/176 reversed, 10 words);
// nested scopes of the same; S(V4,float) arg order; operator* / operator/ chains; member chains; wrapper
// inlines (W1(W1(W1())), W3); ctor-form V4(float,const V4&) and struct-return prototypes (elide copies);
// return *F(&r,..) / return V4(r) / V4 q=r; return q; other inline_depth values (2..16, same order).
// MWCC always creates the inline-body r temps of one statement in post-order; no source form found that
// creates them outer-first while keeping the by-value copies in program order.
// near miss from lane fe1 (unreviewed)
