/* HSTMATCH module=hstp_trans_main.prx start=0x0013b8bc end=0x0013bc0c symbol=FUN_0013b8bc lang=c99 */
/* Hand-written reconstruction: clone of the source of FUN_00130304.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
/* Ghidra pseudocode (hstgen mechanical draft) corrected by hand: flipped the strcmp-equality chain to the positive if/else-if form, an accumulator per digit field, and the loop increment order; readability pass; exact match verified; names and types await human review. */
#include "hst/types.h"

extern u8 DAT_0038233c;
extern u8 DAT_00382340;
extern char DAT_00382228[];
extern char DAT_0038230c[];
extern char DAT_00382318[];
extern char DAT_00382324[];
extern char DAT_00382330[];
extern char DAT_00382348[];
extern char DAT_00382350[];
extern char DAT_0038235c[];
extern char DAT_00382364[];
extern char DAT_00382370[];
extern char DAT_00382380[];
extern char DAT_0038238c[];
extern char *FUN_00014d64();

/* Classifies a command name by substring match and returns its kind id (0 if none). Kinds that carry
   digits in the name also set *out: kind 4 ORs in one bit per 2-digit number in a separator-delimited
   list; the others store a 2- or 3-digit number. */
u32 FUN_0013b8bc(u32 unused, char *name, unsigned int *out)
{
  char *hit;
  char *cur;
  int num;
  u32 kind;

  kind = 0;
  if (name == 0) {
    kind = 0;
  }
  else {
    if (FUN_00014d64(name, DAT_0038230c) != 0) {
      kind = 1;
    }
    else if (FUN_00014d64(name, DAT_00382318) != 0) {
      kind = 2;
    }
    else if (FUN_00014d64(name, DAT_00382324) != 0) {
      kind = 3;
    }
    else if (FUN_00014d64(name, DAT_00382330) != 0) {
      kind = 4;
      cur = FUN_00014d64(name, DAT_00382330) + 10;
      while (1) {
        num = 0;
        num += (*cur - '0') * 10;
        num += cur[1] - '0';
        *out = *out | 1 << num;
        cur++;
        hit = FUN_00014d64(cur, &DAT_0038233c);
        if (hit == 0) break;
        cur = hit + 1;
      }
    }
    else if (FUN_00014d64(name, &DAT_00382340) != 0) {
      kind = 5;
      hit = FUN_00014d64(name, &DAT_00382340);
      num = 0;
      num += (hit[4] - '0') * 10;
      num += hit[5] - '0';
      *out = num;
    }
    else if (FUN_00014d64(name, DAT_00382348) != 0) {
      kind = 6;
      hit = FUN_00014d64(name, DAT_00382348);
      num = 0;
      num += (hit[6] - '0') * 10;
      num += hit[7] - '0';
      *out = num;
    }
    else if (FUN_00014d64(name, DAT_00382350) != 0) {
      kind = 7;
    }
    else if (FUN_00014d64(name, DAT_0038235c) != 0) {
      kind = 8;
    }
    else if (FUN_00014d64(name, DAT_00382364) != 0) {
      kind = 9;
    }
    else if (FUN_00014d64(name, DAT_00382228) != 0) {
      kind = 10;
      hit = FUN_00014d64(name, DAT_00382228);
      num = 0;
      num += (hit[10] - '0') * 100;
      num += (hit[0xb] - '0') * 10;
      num += hit[0xc] - '0';
      *out = num;
    }
    else if (FUN_00014d64(name, DAT_00382370) != 0) {
      kind = 0xc;
    }
    else if (FUN_00014d64(name, DAT_00382380) != 0) {
      kind = 0xd;
    }
    else if (FUN_00014d64(name, DAT_0038238c) != 0) {
      kind = 0xe;
      hit = FUN_00014d64(name, DAT_0038238c);
      num = 0;
      num += (hit[0xb] - '0') * 100;
      num += (hit[0xc] - '0') * 10;
      num += hit[0xd] - '0';
      *out = num;
    }
  }
  return kind;
}
