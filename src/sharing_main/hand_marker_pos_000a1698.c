/* HSTMATCH module=hstp_trans_main.prx start=0x000a1698 end=0x000a17dc symbol=FUN_000a1698 lang=c99 data=0x0037aae0 */
/* Hand-written reconstruction: clone of the source of hand_marker_pos_0009f0ec.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen.py tag from inputs/generated/FUN_000a1698.c) corrected by hand, from the lane fm5 near miss: restructured into struct members and ternaries, added the dropped leading owner argument (int) to FUN_000956ec; readability pass; exact match verified; names and types await human review. Data: its string literals (2 strings, 0x002d45fc-0x002d4613) are defined in place and placed with data= (tools/hstdatapool.py); before, the source named them through address externs. */
#include "hst/rig_obj.h"

extern void FUN_000956ec(void *, float, float);
extern void FUN_0009ddbc(RigObj *);
extern void FUN_000958c8(float *, void *, char *);
extern void FUN_000224c0(float, float *, float *);
extern void FUN_00022570(float *, float *, float *);

/* Part of the model that carries two float settings. */
typedef struct ModelPart {
  char pad00[0x108];
  float unk108;     /* 0x108 */
  char pad10c[0x13c - 0x10c];
  float unk13c;     /* 0x13c */
} ModelPart;

/* Model view: only the part pointer at +0x58 is read. */
typedef struct ModelView {
  char pad00[0x58];
  ModelPart *part;  /* 0x58 */
} ModelView;

/* Position of the hand marker: a fixed offset (mirrored for the right hand) transformed by the hand bone's matrix. */
void FUN_000a1698(float *dst,RigObj *obj)
{
  float v[4];
  float m[4][4];
  float out[4];
  ModelPart *part;
  float hi;
  float lo;
  char *name;

  part = ((ModelView *)obj->model)->part;
  hi = part != 0 ? part->unk13c : 0.0f;
  lo = part != 0 ? part->unk108 : 0.0f;
  FUN_000956ec(obj->model,lo,hi);
  FUN_0009ddbc(obj);
  v[0] = 0.066430002450943f;
  v[1] = -0.08136999607086182f;
  v[2] = 0.008729999884963036f;
  v[3] = 1.0f;
  if (obj->hand == 1) {
    v[0] = v[0] * -1.0f;
  }
  name = obj->hand == 0 ? "Bip01LHand" : "Bip01RHand";
  FUN_000958c8(m[0],obj->model,name);
  FUN_000224c0(100.0f,m[0],m[0]);
  FUN_000224c0(100.0f,m[1],m[1]);
  FUN_000224c0(100.0f,m[2],m[2]);
  FUN_00022570(out,m[0],v);
  dst[0] = out[0];
  dst[1] = out[1];
  dst[2] = out[2];
  dst[3] = out[3];
}
