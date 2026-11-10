/* HSTMATCH module=hstp_trans_main.prx start=0x00053150 end=0x00053504 symbol=FUN_00053150 lang=c++ */
/* Hand-written reconstruction from the disassembly; NEAR MISS, not exact: 238 of 237 words, 30 differing; names and types await human review. */
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

extern "C" int DAT_00376a54, DAT_00376a58, DAT_00376a5c, DAT_00376a60;

typedef union { u8 c[4]; u32 u; } Color;
typedef struct { u16 u, v; u32 color; float x, y, z; } Vertex;
typedef struct { char pad[0x28]; u32 *list; } Ctx;

static inline float CH(const float &v, const int &m) { return v > m ? m : v; }

extern "C" void FUN_00053150(Ctx *g, float *uv0, float *pos0, float *uv1, float *pos1, float *c)
{
  Color col;
  float f;
  u32 *p;
  Vertex *v;
  f = c[0] * 255.0f;
  p = g->list;
  col.c[0] = (u32)CH(f, DAT_00376a54);
  f = c[1] * 255.0f;
  col.c[1] = (u32)CH(f, DAT_00376a58);
  f = c[2] * 255.0f;
  col.c[2] = (u32)CH(f, DAT_00376a5c);
  f = c[3] * 255.0f;
  col.c[3] = (u32)CH(f, DAT_00376a60);
  {
    int z0 = (u16)(int)pos0[2];
    int z1 = (u16)(int)pos1[2];
    p[0] = ((((u32)(p + 0xb) >> 0x18) & 0xf) << 0x10) | 0x10000000;
    p[1] = ((u32)(p + 0xb) & 0xffffff) | 0x8000000;
    v = (Vertex *)(p + 2);
    v[0].u = (u32)uv0[0];
    v[0].v = (u32)uv0[1];
    v[0].color = col.u;
    v[0].x = pos0[0];
    v[0].y = pos0[1];
    v[0].z = (float)z0;
    v[1].u = (u32)uv1[0];
    v[1].v = (u32)uv1[1];
    v[1].color = col.u;
    v[1].x = pos1[0];
    v[1].y = pos1[1];
    v[1].z = (float)z1;
  }
  p[0xc] = 0x1e000001;
  p[0xd] = 0x1280019e;
  p[0xe] = ((((u32)v >> 0x18) & 0xf) << 0x10) | 0x10000000;
  p[0xf] = ((u32)v & 0xffffff) | 0x1000000;
  p[0x10] = 0x4060002;
  g->list = p + 0x11;
}
// NEAR MISS 30 words (lane fs1, 2026-11-10): one extra word, everything else is the cascade of it.
// Matched: frame, the four colour channels (`CH(const float &v, const int &m) { return v > m ? m : v; }` by const ref gives
// retail's lw + lwc1/cvt double load of the int limit; the result through (u32) gives the 2^31 unsigned conversion),
// the GE command words (`(((u32)x >> 24) & 0xf) << 16` gives retail's `ext`), the u/v unsigned conversions, and
// z as `int z0 = (u16)(int)pos[2];` read before the first list store, stored as `(float)z0` (one andi, cvt.s.w).
// Still different: retail loads the colour word `lw t3,12(sp)` ONCE and stores it to both vertices; ours reloads it
// (`lw a1,12(sp)`) for the second vertex, which shifts the tail scheduling. Ruled out: a `u32 cw = col.u;` local (before
// or after the list stores; copy-propagated back to the load), `v[1].color = v[0].color`, an inline MakeColor() returning
// the word, an inline SetVertex() taking the word or the union by value, union member order, the vertex colour typed as the
// union (aggregate copy, 44) or a byte struct (84). `v[0].color = v[1].color = col.u;` loads once but groups the two
// stores (38).
// near miss from lane fs1 (unreviewed)
