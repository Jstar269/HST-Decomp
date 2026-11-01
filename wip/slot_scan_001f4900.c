/* HSTMATCH module=EBOOT.BIN start=0x001f4900 end=0x001f4b60 symbol=FUN_001f4900 lang=c99 */
extern int DAT_002c8e2c[];
extern int DAT_002c8e44[];
extern int FUN_00093b00(int *, int);
extern void FUN_001f37e4(int, int *, int, int, int, int);
extern void FUN_000937f0(int, int *, int);
extern void FUN_0005f110(int, int);
extern void FUN_00058f38(int, int, int);

void FUN_001f4900(int param_1)
{
  int iVar1;
  int iVar2;
  unsigned int uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  float fVar9;

  iVar7 = 0;
  iVar5 = param_1;
  do {
    piVar6 = *(int **)(iVar5 + 0x3a0);
    if (piVar6 != (int *)0x0) {
      if (piVar6[0x16] == 0) {
        uVar3 = 0xffffffff;
      }
      else {
        uVar3 = *(unsigned int *)(piVar6[0x16] + 0xf0);
      }
      if ((int)uVar3 < 0) {
        iVar8 = *piVar6;
      }
      else {
        if ((int)uVar3 < 10) {
          iVar8 = uVar3 * 0x14;
          if (uVar3 < 2) {
            iVar1 = FUN_00093b00(piVar6, 0xffffffff);
            if (iVar1 != 0) {
              FUN_001f37e4(param_1, piVar6, *(int *)(iVar5 + 0x3b0), iVar7,
                           DAT_002c8e2c[*(int *)(param_1 + 0x524)], 0);
              goto LAB_001f4a0c;
            }
            iVar1 = *(int *)((char *)DAT_002c8e44 + iVar8);
          }
          else {
            if ((uVar3 - 7 < 3) && (iVar1 = FUN_00093b00(piVar6, 0xffffffff), iVar1 != 0)) {
              FUN_001f37e4(param_1, piVar6, *(int *)(iVar5 + 0x3b0), iVar7, 5, 0);
            }
LAB_001f4a0c:
            iVar1 = *(int *)((char *)DAT_002c8e44 + iVar8);
          }
          if (iVar1 == 2) {
            if (*(char *)(param_1 + iVar7 + 0x3e4) == '\0') {
              if (piVar6[0x16] == 0) {
                fVar9 = 0.0f;
              }
              else {
                fVar9 = *(float *)(piVar6[0x16] + 0x104);
              }
              if (*(float *)((char *)DAT_002c8e44 + iVar8 + 0xc) <= fVar9) {
                iVar1 = piVar6[0x19];
                iVar4 = piVar6[0x14];
                FUN_000937f0(*(int *)((char *)DAT_002c8e44 + iVar8 + 8), piVar6, 0xffffffff);
                if (iVar4 != 0) {
                  FUN_0005f110(*(int *)((char *)DAT_002c8e44 + iVar8 + 8), iVar4);
                }
                if (iVar1 == 0) {
                  iVar8 = *piVar6;
                }
                else {
                  FUN_00058f38(*(int *)((char *)DAT_002c8e44 + iVar8 + 8), iVar1, 0xffffffff);
                  iVar8 = *piVar6;
                }
                (*(int (**)(int *))(iVar8 + 0xc))(piVar6);
              }
            }
            else {
              iVar8 = FUN_00093b00(piVar6, 0xffffffff);
              if (iVar8 != 0) {
                iVar1 = 0;
                iVar8 = param_1;
                do {
                  if (piVar6 == (int *)0x0) {
                    iVar4 = *(int *)(iVar8 + 0x3a0);
LAB_001f4adc:
                    if (iVar4 == 0) {
                      iVar2 = 0;
                    }
                    else {
                      iVar2 = *(int *)(iVar4 + 0x58);
                    }
                    if (iVar2 != 0) {
                      FUN_001f37e4(param_1, (int *)iVar4, *(int *)(iVar8 + 0x3b0), iVar1, 5, 0);
                    }
                  }
                  else if (piVar6 == *(int **)(iVar8 + 0x3a0)) {
                    iVar4 = *(int *)(iVar8 + 0x3a0);
                    goto LAB_001f4adc;
                  }
                  iVar1 = iVar1 + 1;
                  iVar8 = iVar8 + 4;
                } while (iVar1 < 4);
              }
            }
          }
        }
        iVar8 = *piVar6;
      }
      (*(int (**)(int *))(iVar8 + 0xc))(piVar6);
    }
    iVar7 = iVar7 + 1;
    iVar5 = iVar5 + 4;
    if (3 < iVar7) {
      return;
    }
  } while (1);
}

/* NEAR 162/152 words, 152 differ, first +0x28. Probe 1 = full transpcription of Ghidra's body; 10 words
   over and every word differs, so this is a BODY gap, not allocation -- parked after one probe by the
   pass rule. Retail structure decoded: 32-byte frame, s0-s6; s5=param_1, s4=slot i, s1=param_1+i*4;
   outer do/while over 4 slots: s3=*(s1+0x3a0); null -> next slot (beqzl with addiu s4 in delay);
   state = s3[0x16] ? *(u32*)(s3[0x16]+0xf0) : -1 (beqzl/li -1); bltzl -> tail virtual call;
   >=10 -> tail call; s0=&DAT_002c8e44+state*20 (sll/add/sll); state<2: FUN_00093b00(s3,-1) != 0 ->
   FUN_001f37e4(param_1,s3,*(s1+0x3b0),i,DAT_002c8e2c[param_1[0x524]],0) then L; else state-7<3 with the
   same guard -> FUN_001f37e4(...,i,5,0); L: v1=*s0; if v1!=2 -> tail call; byte(param_1+i+0x3e4)==0:
   f = s3[0x16] ? *(float*)(s3[0x16]+0x104) : 0.0f; if (*(float*)(s0+0xc) <= f) { s2=s3[0x19];
   s6=s3[0x14]; FUN_000937f0(*(s0+8),s3,-1); if (s6) FUN_0005f110(*(s0+8),s6); if (s2) FUN_00058f38(
   *(s0+8),s2,-1); (*(*(int*)s3+0xc))(s3); } else: FUN_00093b00 guard then a 4-iteration scan
   (s0=param_1, k=0..3) with the piVar6==0 / piVar6==*(s0+0x3a0) dual test, child = *(a1+0x58), and
   FUN_001f37e4(param_1,a1,*(s0+0x3b0),k,5,0); tail: (*(*(int*)s3+0xc))(s3); i++; until i>3.
   NEXT: our 10 extra words come from the two virtual-call sites being emitted twice plus the duplicated
   state load; try hoisting `piVar6[0x16]` into a single local read before the state test (retail reads
   it twice via beqzl, so only the second site should remain) and verify the outer loop's null arm jumps
   to the increment (beqzl) rather than a separate block. */
// near miss from lane dsn8 (unreviewed)
