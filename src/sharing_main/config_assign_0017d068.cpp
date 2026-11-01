/* HSTMATCH module=hstp_trans_main.prx start=0x0017d068 end=0x0017d188 symbol=FUN_0017d068 lang=c++ */
/* Clone of src/eboot/config_assign_00170d9c.cpp (hstclone.py tag dsn45): addresses substituted; exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */

struct V3s_00099c98 { short x, y, z; };
struct S_00099c98 {
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
    V3s_00099c98 v8a;
    short a90[5];
    S_00099c98();
    S_00099c98 &operator=(const S_00099c98 &);
};

struct Pair_00285230 {
  unsigned char a, b;
};

struct V4 { float x, y, z, w; };

struct S_0017d068 : public S_00099c98 {
  Pair_00285230 p9c;
  unsigned char b9e, b9f;
  char ba0;
  V4 va4;
  float fb4;
  int ib8;
  short arr_bc[6];
  S_0017d068();
};

extern S_0017d068 DAT_003ead48;

extern "C" {
extern char *DAT_003e1a08;
void *FUN_00177328(char *self);
void FUN_001cfa60(int a, void *b, int c);
void FUN_0017d068(char *self);
}

void FUN_0017d068(char *self)
{
  void *r = FUN_00177328(self);
  r = ((void *(*)(void *))*(void **)(self + 0x3e8))(r);
  FUN_001cfa60(0, r, *(int *)(self + 0x8c));
  if (DAT_003e1a08 == 0)
    return;
  S_0017d068 *s = (S_0017d068 *)(*(char **)(DAT_003e1a08 + 0x40) + 0x648);
  *s = DAT_003ead48;
}
