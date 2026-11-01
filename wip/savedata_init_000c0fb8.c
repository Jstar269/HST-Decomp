/* HSTMATCH module=EBOOT.BIN start=0x000c0fb8 end=0x000c13c0 symbol=FUN_000c0fb8 lang=c99 rodata=0x002c0130 */
/* Hand-written from the pseudocode; savedata init: helper fills, a 14-case constant dispatch on mode, then the per-mode object rebuild. */

extern void FUN_000114c0(void *dst, int value, int size);
extern void FUN_000149a8(void *dst, void *src);
extern void FUN_00011090(void *dst, void *src, int size);
extern void FUN_00199280(void *self);
extern void FUN_000c1534(int self);
extern int FUN_000c0b5c(char *msg, int error);
extern int FUN_002b7c50(void *self);
extern int DAT_00338ad0;
extern char DAT_002d59dc[];
extern char DAT_002d5a04[];

int FUN_000c0fb8(int param_1, int param_2)
{
  int iVar1;
  int iVar4;
  int uVar2;
  int *piVar3;
  unsigned uVar5;
  char *rec;

  *(int *)(param_1 + 0x40) = param_2;
  FUN_000114c0((void *)(param_1 + 0x94), 0, 0x600);
  *(int *)(param_1 + 0x94) = 0x600;
  *(int *)(param_1 + 0x9c) = 0;
  *(int *)(param_1 + 0xa0) = 0x2a;
  *(int *)(param_1 + 0xa4) = 0x2c;
  *(int *)(param_1 + 0xa8) = 0x2b;
  *(int *)(param_1 + 0xac) = 0x29;
  FUN_00199280((void *)(param_1 + 0x94));
  FUN_00011090((void *)(param_1 + 0x670), (void *)(param_1 + 0x68), 0x10);
  *(int *)(param_1 + 0x680) = 0;
  FUN_000149a8((void *)(param_1 + 0xd0), *(void **)(param_1 + 0x78));
  FUN_000149a8((void *)(param_1 + 0xe0), *(void **)(param_1 + 0x7c));
  if (*(int *)(param_1 + 0x774) == 0) {
    iVar1 = *(int *)(param_1 + 0x3c);
  }
  else {
    if ((*(int *)(param_1 + 0x40) != 3) && (*(int *)(param_1 + 0x40) != 4)) {
      FUN_000149a8((void *)(param_1 + 0xf8), *(void **)(param_1 + 0x770));
      if (*(int *)(param_1 + 0x3c) < 5) {
        rec = *(char **)(param_1 + 0x770);
        *(int *)(param_1 + 0x108) = *(int *)(rec + 0x40);
      }
      else {
        if (9 < *(int *)(param_1 + 0x3c)) {
          rec = *(char **)(param_1 + 0x770);
          *(int *)(param_1 + 0x108) = *(int *)(rec + 0x40);
        }
        else {
          rec = *(char **)(param_1 + 0x770);
          *(int *)(param_1 + 0x108) = *(int *)(rec + 0x44);
        }
      }
      *(int *)(param_1 + 0x10c) = *(int *)(*(int *)(param_1 + 0x770) + 0x48);
      *(int *)(param_1 + 0x110) = *(int *)(*(int *)(param_1 + 0x770) + 0x48);
    }
    iVar1 = *(int *)(param_1 + 0x3c);
  }
  if (iVar1 == 0) {
    FUN_000114c0((void *)(param_1 + 0x698), 0, 0x14);
    FUN_000114c0((void *)(param_1 + 0x6ac), 0, 0x40);
    FUN_000114c0((void *)(param_1 + 0x6ec), 0, 0x1c);
    FUN_000149a8((void *)(param_1 + 0x6ac), *(void **)(param_1 + 0x78));
    FUN_000149a8((void *)(param_1 + 0x6bc), *(void **)(param_1 + 0x7c));
    *(int *)(param_1 + 0x664) = param_1 + 0x698;
    *(int *)(param_1 + 0x668) = param_1 + 0x6ac;
    *(int *)(param_1 + 0x66c) = param_1 + 0x6ec;
    uVar2 = *(int *)(param_1 + 0x3c);
  }
  else {
    uVar2 = *(int *)(param_1 + 0x3c);
  }
  switch (uVar2) {
  case 0:
    *(int *)(param_1 + 0xc4) = 8;
    break;
  case 1:
    *(int *)(param_1 + 0xc4) = 3;
    break;
  case 2:
    *(int *)(param_1 + 0xc4) = 1;
    break;
  case 3:
    *(int *)(param_1 + 0xc4) = 5;
    break;
  case 4:
  case 9:
    iVar1 = *(int *)(param_1 + 0x40);
    if (iVar1 == 6) {
      *(int *)(param_1 + 0xc4) = 0xf;
    }
    else if (iVar1 == 5) {
      if (*(char *)(param_1 + 0x44) == 0) {
        *(int *)(param_1 + 0xc4) = 0x11;
      }
      else {
        *(int *)(param_1 + 0xc4) = 0xd;
      }
    }
    else if (iVar1 == 4) {
      *(int *)(param_1 + 0xc4) = 0x16;
    }
    else if (iVar1 == 3) {
      *(int *)(param_1 + 0xc4) = 0xc;
    }
    break;
  case 5:
    *(int *)(param_1 + 0xc4) = 0;
    break;
  case 6:
    *(int *)(param_1 + 0xc4) = 2;
    break;
  case 7:
    *(int *)(param_1 + 0xc4) = 0;
    break;
  case 8:
    *(int *)(param_1 + 0xc4) = 4;
    break;
  case 10:
    *(int *)(param_1 + 0xc4) = 10;
    break;
  case 0xb:
    *(int *)(param_1 + 0xc4) = 9;
    break;
  case 0xc:
    *(int *)(param_1 + 0xc4) = 6;
    *(char **)(param_1 + 0xf4) = DAT_002d59dc;
    break;
  case 0xd:
    *(int *)(param_1 + 0xc4) = 7;
    break;
  default:
    break;
  }
  iVar1 = *(int *)(param_1 + 0x3c);
  if (iVar1 == 4 || iVar1 == 9) {
    iVar1 = *(int *)(param_1 + 0x40);
    if (iVar1 != 6) {
      if (iVar1 == 4) {
        *(int *)(param_1 + 0x694) = 2;
        uVar5 = 0;
        if (*(int *)(param_1 + 0x780) != 0) {
          iVar4 = 0;
          do {
            iVar1 = uVar5 * 0x50;
            FUN_000149a8((void *)(*(int *)(param_1 + 0x76c) + iVar4 + 8),
                         (void *)(*(int *)(param_1 + 0x77c) + iVar1));
            uVar5 = uVar5 + 1;
            iVar1 = *(int *)(*(int *)(param_1 + 0x77c) + iVar1 + 0x48);
            piVar3 = (int *)(*(int *)(param_1 + 0x76c) + iVar4);
            *piVar3 = iVar1;
            piVar3[1] = iVar1 >> 0x1f;
            iVar4 = iVar4 + 0x18;
          } while (uVar5 < *(unsigned *)(param_1 + 0x780));
        }
        FUN_000114c0((void *)(param_1 + 0x730), 0, 0x3c);
        *(int *)(param_1 + 0x730) = *(int *)(param_1 + 0x780);
        *(int *)(param_1 + 0x738) = *(int *)(param_1 + 0x76c);
        *(int *)(param_1 + 0x690) = param_1 + 0x730;
      }
      else if (iVar1 == 3) {
        FUN_000114c0((void *)(param_1 + 0x708), 0, 0x24);
        *(int *)(param_1 + 0x708) = *(int *)(param_1 + 0x780);
        *(int *)(param_1 + 0x720) = *(int *)(param_1 + 0x72c);
        *(int *)(param_1 + 0x68c) = param_1 + 0x708;
      }
    }
    uVar2 = *(int *)(param_1 + 0x694);
  }
  else {
    *(int *)(param_1 + 0x694) = 0;
    uVar2 = *(int *)(param_1 + 0x694);
  }
  *(int *)(param_1 + 0x684) = uVar2;
  if (*(char *)(param_1 + 0x44) != 0) {
    FUN_000c1534(param_1);
  }
  iVar1 = FUN_002b7c50((void *)(param_1 + 0x94));
  if (iVar1 != 0) {
    DAT_00338ad0 = FUN_000c0b5c(DAT_002d5a04, iVar1);
  }
  return iVar1 == 0;
}

/* PARKED 2026-11-01 (batch 1 spent): NEAR 271/258 words, 264 differ, first at +0x8.
 * Frame matches (32, ra@28) and the switch table is placed (rodata=0x002c0130, 14 entries - retail
 * computes the base with lui v1,0x2c / addiu v1,v1,304 and sltiu v0,14).
 * RESIDUAL: one callee-saved register too many. Ours saves s0-s4 + ra into 32 bytes (s4 holds param_1);
 * retail saves s0-s3 + ra and uses s3 for param_1, so 0xc..0x8 stay free and no 48-byte frame appears.
 * NEXT LEVER: cut one live temporary, in this order - (1) drop `rec` and write
 * *(int *)(*(int *)(param_1 + 0x770) + 0x40) inline in the three 0x108 paths (retail reloads the
 * field instead of holding the record), (2) drop `uVar2` and switch directly on
 * *(int *)(param_1 + 0x3c) after the iVar1 == 0 block, (3) fold piVar3 into the two stores.
 * Do NOT respell the case bodies: the table and every case constant are already placed identically.
 */

/* BATCH 2 (2026-11-01): three variants, all byte-identical in codegen - differ 264, words 271/258.
 *   inline-reload (drop `rec`), direct-switch (drop uVar2), no-piVar3 - the last one got WORSE (273).
 * CONCLUSION: those locals are not what holds the extra callee-saved register. MWCC normalises them.
 * SAVED SETS: ours ra+s0-s4 (6) in 32 bytes; retail ra+s0-s3 (5) in 32 bytes.
 * NEXT LEVER: the extra register is real pressure, so change a shape, not a name - try
 *  (1) the +0x774 branch as a single expression that assigns *(p+0x108) once,
 *  (2) folding the two `*(int *)(param_1 + 0x3c)` reads in that branch into one local read,
 *  (3) declaring param_2 as unsigned so the +0x40 store/reload does not keep a live copy.
 * Everything else (case table, rodata base, frame 32, ra@28) is already placed identically.
 */
// near miss from lane dsn8 (unreviewed)
