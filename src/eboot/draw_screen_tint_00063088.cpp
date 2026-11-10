/* HSTMATCH module=EBOOT.BIN start=0x00063088 end=0x00063478 symbol=FUN_00063088 lang=c++ */
/* Clone of src/sharing_main/draw_screen_tint_00063f4c.cpp (hstclone.py tag c145620): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct Vec4 { float x, y, z, w; } __attribute__((aligned(16)));
union Color { struct { unsigned char r, g, b, a; } c; unsigned int u; };

struct Tex { char pad[0x18]; float f18; };
struct Obj {
  int f0;
  Tex *tex;
  char pad8[8];
  float f10;
  char pad14[0x40 - 0x14];
  unsigned char f40;
  unsigned char f41;
  char pad42[2];
  float f44;
};

extern "C" char DAT_00311170[];
extern "C" {
void FUN_00063554(Vec4 *out, Obj *o);
void FUN_000503d0(char *name, float f);
void *FUN_00050534(char *name);
void FUN_00051398(void *tex, float f);
void FUN_00051490(void *tex, int a, int b, int w, int h, int c, int d);
void FUN_00051a10(void *tex, unsigned color, float x0, float y0, float x1, float y1, float z);
void FUN_00051894(void *tex, unsigned color, float u0, float v0, float x0, float y0, float u1, float v1,
                  float x1, float y1, float z);
void FUN_0005185c(void *tex);
}

extern "C" void FUN_00063088(Obj *self, float alpha)
{
  Color c1;
  Color c2;
  Vec4 v;

  if (self->f40 != 0 && self->f41 == 0) {
    FUN_000503d0(DAT_00311170, self->f44);
    self->f41 = 1;
    return;
  }
  FUN_00063554(&v, self);
  if (self->f10 >= 0.0f)
    v.w = self->f10;
  if (self->f40 == 0) {
    c1.c.r = (unsigned)(255.0f * v.x);
    c1.c.g = (unsigned)(255.0f * v.y);
    c1.c.b = (unsigned)(255.0f * v.z);
    c1.c.a = (unsigned)(128.0f * v.w);
    void *tex = self->tex;
    FUN_00051490(tex, 0, 0, 480, 272, 0, 0);
    FUN_00051a10(tex, c1.u, 0.0f, 0.0f, 480.0f, 272.0f, alpha);
    FUN_0005185c(tex);
  } else {
    c2.c.r = (unsigned)(128.0f * v.x);
    c2.c.g = (unsigned)(128.0f * v.y);
    c2.c.b = (unsigned)(128.0f * v.z);
    c2.c.a = (unsigned)(128.0f * v.w);
    void *tex = FUN_00050534(DAT_00311170);
    FUN_00051398(tex, self->tex->f18);
    if (tex != 0) {
      float x = 0.0f;
      FUN_00051490(tex, 0, 0, 480, 272, 0, 0);
      int i;
      unsigned col = c2.u;
      for (i = 0; i < 4; i++) {
        FUN_00051894(tex, col, x, 0.0f, x, 0.0f, x + 120.0f, 272.0f, x + 120.0f, 272.0f, alpha);
        x += 120.0f;
      }
      FUN_0005185c(tex);
    }
  }
}
