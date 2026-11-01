/* HSTMATCH module=EBOOT.BIN start=0x000b5900 end=0x000b5d60 symbol=FUN_000b5900 lang=c99 */
/* Hand-written: 8 parameters (a0-a3 + t0-t3), two 80-byte stack blocks (0x50 <- param_1, 0xa0 <- param_2),
   the pair compare at word 14 of each block, and the four-way recursive splitter. Layout taken from the
   disassembly (addiu a2,sp,80 / addiu a3,sp,160), not from Ghidra's overlapping local names. */

extern void FUN_000b41b8(void *a, void *b);
extern void *FUN_000b42ac(void *p);
extern void FUN_000b5d60(void *a, void *b);
extern int  FUN_000b5d78(void *a, void *b);
extern void FUN_000b5d90(void *a);
extern void FUN_000b5da4(void *a);
extern void FUN_000b5db8(void *a, void *b);
extern void FUN_000b5ed4(void *a, void *b);
extern void FUN_000b6894(void *out, void *a, void *b, void *c, int d);
extern void FUN_000b6990(void *out, void *a, void *b, void *c, int d);
extern void FUN_000b6a8c(void *out, void *a, void *b, void *c, int d, int e, void *f, void *g);
extern void FUN_000b6ffc(void *a, void *b, void *c, void *d, void *e, void *f, int g);
extern void FUN_000b74d4(void *a, void *b, void *c, void *d, void *e, void *f, int g);

void FUN_000b5900(void *param_1, void *param_2, void *param_3, char param_4,
                  int param_5, int param_6, int *param_7, int *param_8)
{
  unsigned int *puVar3;
  unsigned int *puVar4;
  void *uVar2;
  unsigned int u32;
  void *uVar1;
  void *local_144;
  void *puVar5;
  int iVar5;
  int iVar6;
  unsigned int blk2[20];
  unsigned int blk1[20];
  float local_a8;
  float local_f8;
  char auStack_84[8];
  char auStack_6c[8];
  char auStack_30[8];
  char auStack_28[4];
  char auStack_24[8];
  char auStack_1c[12];
  int local_7c;
  int local_78;
  int local_64;
  int local_60;
  void *local_74;
  void *local_70;
  void *local_5c;
  void *local_58;
  void *local_54;
  void *local_50;
  void *local_4c;
  void *local_48;
  void *local_44;
  void *local_40;
  int local_3c;
  void *local_38;
  void *local_34;
  int local_10;
  int local_c;
  int local_8;
  int local_4;

  local_7c = param_7[0];
  local_78 = param_7[1];
  local_10 = param_8[0];
  local_c = param_8[1];
  if (param_1 != param_2) {
    if (param_2 != param_3) {
      local_8 = local_7c;
      local_4 = local_78;
      if (param_5 + param_6 == 2) {
        puVar4 = blk2;
        iVar5 = 10;
        puVar3 = (unsigned int *)param_2;
        do {
          u32 = puVar3[1];
          iVar5 = iVar5 - 1;
          *puVar4 = *puVar3;
          puVar4[1] = u32;
          puVar3 = puVar3 + 2;
          puVar4 = puVar4 + 2;
        } while (0 < iVar5);
        puVar4 = blk1;
        iVar5 = 10;
        puVar3 = (unsigned int *)param_1;
        do {
          u32 = puVar3[1];
          iVar5 = iVar5 - 1;
          *puVar4 = *puVar3;
          puVar4[1] = u32;
          puVar3 = puVar3 + 2;
          puVar4 = puVar4 + 2;
        } while (0 < iVar5);
        local_f8 = ((float *)blk2)[14];
        local_a8 = ((float *)blk1)[14];
        if (local_f8 < local_a8) {
          local_54 = param_2;
          local_50 = param_1;
          uVar2 = FUN_000b42ac(&local_50);
          uVar1 = FUN_000b42ac(&local_54);
          FUN_000b41b8(uVar2, uVar1);
        }
      }
      else {
        iVar5 = (local_10 - local_7c) / 0x50;
        if (param_6 < param_5 || iVar5 < param_5) {
          if (iVar5 < param_6) {
            if (param_6 < param_5) {
              iVar6 = param_5;
              if (param_5 < 0) {
                iVar6 = param_5 + 1;
              }
              iVar6 = iVar6 >> 1;
              local_144 = (char *)param_1 + iVar6 * 0x50;
              FUN_000b6990(&local_38, param_2, param_3, local_144, (int)param_4);
              iVar5 = (int)((char *)local_38 - (char *)param_2) / 0x50;
              puVar5 = local_38;
              local_4c = param_2;
              local_44 = param_2;
            }
            else {
              iVar5 = param_6;
              if (param_6 < 0) {
                iVar5 = param_6 + 1;
              }
              iVar5 = iVar5 >> 1;
              puVar5 = (char *)param_2 + iVar5 * 0x50;
              FUN_000b6894(&local_34, param_1, param_2, puVar5, (int)param_4);
              local_144 = local_34;
              iVar6 = (int)((char *)local_34 - (char *)param_1) / 0x50;
              local_48 = param_1;
              local_40 = param_1;
            }
            FUN_000b6a8c(&local_3c, local_144, param_2, puVar5, param_5 - iVar6, iVar5,
                         &local_8, &local_10);
            FUN_000b5900(param_1, local_144, &local_3c, param_4, iVar6, iVar5,
                         &local_8, &local_10);
            FUN_000b5900(&local_3c, puVar5, param_3, param_4, param_5 - iVar6,
                         param_6 - iVar5, &local_8, &local_10);
          }
          else {
            local_74 = param_3;
            local_70 = param_2;
            iVar5 = FUN_000b5d78(&local_70, &local_74);
            while (iVar5 != 0) {
              FUN_000b5ed4(auStack_84, &local_7c);
              uVar2 = FUN_000b42ac(&local_70);
              FUN_000b5db8(auStack_84, uVar2);
              FUN_000b5da4(&local_70);
              FUN_000b5d90(&local_7c);
              iVar5 = FUN_000b5d78(&local_70, &local_74);
            }
            FUN_000b5d60(auStack_30, &local_7c);
            FUN_000b74d4(auStack_28, param_1, param_2, &local_8, auStack_30, param_3,
                         (int)param_4);
          }
        }
        else {
          local_64 = local_7c;
          local_60 = local_78;
          local_5c = param_2;
          local_58 = param_1;
          iVar5 = FUN_000b5d78(&local_58, &local_5c);
          while (iVar5 != 0) {
            FUN_000b5ed4(auStack_6c, &local_64);
            uVar2 = FUN_000b42ac(&local_58);
            FUN_000b5db8(auStack_6c, uVar2);
            FUN_000b5da4(&local_58);
            FUN_000b5d90(&local_64);
            iVar5 = FUN_000b5d78(&local_58, &local_5c);
          }
          FUN_000b5d60(auStack_24, &local_64);
          FUN_000b6ffc(auStack_1c, &local_8, auStack_24, param_2, param_3, param_1,
                       (int)param_4);
        }
      }
    }
  }
}

/* JAL PARITY (2026-11-01): retail's jal multiset (b42ac x2 in the pair branch + 2 in the merge loops,
 *   b41b8, b5d78 x3, b5ed4 x2, b5db8 x2, b5da4 x2, b5d90 x2, b5d60 x2, b6ffc, b74d4, b6894, b6990, b6a8c,
 *   self x2) matches this draft exactly. Unmatched addresses: none. The 64 short bytes are argument/storage
 *   traffic at those sites, so the next lever is the store pattern at 0x104..0x110 and 0x11c..0x150.
 */
/* BATCH 2 (2026-11-01): five slot spellings -- enlarged auStack_84/6c/30/24/1c/28, a 4-word accumulator
 *   block, a 4-word pair block and a padded blk2 -- all compiled BYTE-IDENTICAL (250/280, 269 differ,
 *   frame still 320 vs retail 384). Same rule as 0x001c8dc8: unused locals are dead and eliminated.
 * NEXT LEVER: find the 64 live bytes. Retail touches 0x104..0x110 and 0x11c..0x150; match those stores to
 *   the call sites (FUN_000b6a8c / the two recursive calls / FUN_000b74d4) and give the values that survive
 *   those calls their own objects, rather than adding slots for values that do not cross a call.
 */
/* PARKED note (batch 1, 2026-11-01): first measurement pending. Layout settled from the disassembly:
 *   two 20-word (80-byte) blocks, blk1 at sp+0x50 copied from param_1 and blk2 at sp+0xa0 copied from
 *   param_2, compared at word 14 of each (lwc1 0xd8(sp) vs 0x88(sp)) -- the earlier "three 20-word
 *   buffers" reading came from Ghidra's overlapping local names and is wrong.
 *   Retail frame 384 (27bdfe80), saves ra@44 + s8..s0@40..8, no float saves.
 *   Next lever if the frame comes out short: the two self-recursive calls take eight arguments, so the
 *   4-word argument home area at 0x10(sp) is the place to look. */
// near miss from lane dsn8 (unreviewed)
