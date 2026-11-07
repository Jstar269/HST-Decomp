/* HSTMATCH module=hstp_trans_main.prx start=0x0022d6c0 end=0x0022d780 symbol=FUN_0022d6c0 lang=c++ */
/* Hand-written reconstruction: clone of the source of node_pct_0021e14c.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag a1) corrected by hand, starting from the fm5 near miss: the /100 of the ratio is applied inside the null-checked block (the first division stays before the branch) and the clamp tests v > hi; exact match verified; names and types await human review. */
struct Node {
  unsigned char pad00[4];
  short f4;                   /* 0x04 */
  unsigned char pad06[2];
  short f8;                   /* 0x08 */
};

struct Obj {
  unsigned char pad00[0xa8];
  struct Node *node;          /* 0xa8 */
  unsigned char padac[0x128];
  float f1d4;                 /* 0x1d4 */
  unsigned char pad1d8[0x18];
  int f1f0;                   /* 0x1f0 */
  int f1f4;                   /* 0x1f4 */
  unsigned char pad1f8[8];
  int f200;                   /* 0x200 */
  int f204;                   /* 0x204 */
};

extern "C" float DAT_00398c40;
extern "C" float DAT_00398c44;

static inline float Clamp(const float &v, const float &lo, const float &hi)
{
  return v < lo ? lo : (v > hi ? hi : v);
}

extern "C" void FUN_0022d6c0(struct Obj *param_1, int param_2)
{
  struct Node *node;
  float ratio;
  float k;
  float den;

  param_1->f204 = param_2;
  den = (float)(param_1->f1f4 - param_1->f1f0);
  ratio = (float)(param_1->f200 + param_2 - param_1->f1f0) * 100.0f / den;
  node = param_1->node;
  if (node != 0) {
    ratio = ratio / 100.0f;
    k = Clamp(ratio, DAT_00398c40, DAT_00398c44);
    node->f8 = (short)(int)((float)node->f4 + param_1->f1d4 * k);
  }
}
