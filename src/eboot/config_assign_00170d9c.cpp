/* HSTMATCH module=EBOOT.BIN start=0x00170d9c end=0x00170ebc symbol=FUN_00170d9c lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct V3s_00097b2c { short x, y, z; };
struct S_00097b2c {
    short a00[6];
    unsigned char b0c, b0d, b0e;
    char s0f[16];
    unsigned short h20, h22;
    unsigned char b24;
    char s25[6];
    unsigned char b2b, b2c;
    char s2d[16];
    short h3e, h40;
    unsigned char b42, b43;
    float f44, f48, f4c;
    short h50, h52;
    char s54[16];
    unsigned char b64, b65, b66, b67;
    char s68[16];
    short h78, h7a;
    unsigned char b7c;
    char s7d[12];
    V3s_00097b2c v8a;
    short a90[5];
    S_00097b2c();
    S_00097b2c &operator=(const S_00097b2c &);
};

struct Pair_00285230 {
  unsigned char a, b;
};

struct V4 { float x, y, z, w; };

struct S_00170d9c : public S_00097b2c {
  Pair_00285230 p9c;
  unsigned char b9e, b9f;
  char ba0;
  V4 va4;
  float fb4;
  int ib8;
  short arr_bc[6];
  S_00170d9c();
};

extern S_00170d9c DAT_00341a10;

extern "C" {
extern char *DAT_003386c8;
void *FUN_0016b164(char *self);
void FUN_001c184c(int a, void *b, int c);
void FUN_00170d9c(char *self);
}

void FUN_00170d9c(char *self)
{
  void *r = FUN_0016b164(self);
  r = ((void *(*)(void *))*(void **)(self + 0x3e8))(r);
  FUN_001c184c(0, r, *(int *)(self + 0x8c));
  if (DAT_003386c8 == 0)
    return;
  S_00170d9c *s = (S_00170d9c *)(*(char **)(DAT_003386c8 + 0x40) + 0x648);
  *s = DAT_00341a10;
}
