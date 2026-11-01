/* HSTMATCH module=EBOOT.BIN start=0x000d3730 end=0x000d399c symbol=FUN_000d3730 lang=c99 rodata=0x2c0580 */
extern char DAT_003386b0[];
extern int DAT_002d5ab0;
extern void FUN_000513a0(int, int);
extern void FUN_00051398(int, int);
extern int FUN_000b3490(int);
extern int FUN_000b34d0(int);
extern int FUN_000b13c4(int);
extern int FUN_00000bcc(int);
extern int FUN_000e985c(int, int);
extern void FUN_0001f21c(int, int);

void FUN_000d3730(int param_1, int param_2, int param_3)
{
  int uVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int s4;

  param_3 = param_3 * 4;
  FUN_000513a0(*(int *)(*(int *)(param_1 + 0x3c) + param_3), 1);
  FUN_00051398((int)0x4592e000, *(int *)(*(int *)(param_1 + 0x3c) + param_3));
  iVar4 = *(int *)(param_1 + 0x48) + param_2 * 0x10;
  piVar2 = *(int **)(iVar4 + 0xc);
  if (piVar2 != (int *)0x0) {
    if (piVar2 == (int *)0x0) {
      *(int *)(iVar4 + 0xc) = 0;
    }
    else {
      (*(int (**)(int *, int))(*piVar2 + 8))(piVar2, 1);
      *(int *)(iVar4 + 0xc) = 0;
    }
  }
  iVar5 = *(int *)((char *)DAT_003386b0 + param_2 * 4 + 0x70);
  if ((DAT_002d5ab0 & 0x1000) != 0) {
    iVar3 = *(int *)(iVar5 + 0x4e4);
    s4 = -1;
    switch (*(int *)(iVar5 + 0x4e8)) {
    case 0:
      if (iVar3 == 2) {
        s4 = 2;
      }
      else if (iVar3 == 1) {
        s4 = 1;
      }
      else if (iVar3 == 0) {
        if (*(char *)(iVar5 + 0x4d8) == '\0') {
          s4 = 8;
        }
        else {
          s4 = 6;
        }
      }
      break;
    case 1:
      s4 = 5;
      break;
    case 2:
      s4 = 4;
      break;
    case 3:
      s4 = 0;
      break;
    case 4:
      s4 = 9;
      break;
    case 5:
      s4 = 9;
      break;
    case 6:
      s4 = 1;
      break;
    case 7:
      s4 = 3;
      break;
    case 8:
    case 9:
    case 10:
      s4 = 7;
      break;
    case 0xb:
      s4 = 3;
      break;
    case 0xc:
      s4 = 3;
    }
    iVar3 = FUN_000b3490(iVar5);
    if ((((iVar3 == 0) && (iVar3 = FUN_000b34d0(iVar5), iVar3 == 0)) && (s4 != 2)) &&
       (((s4 != 1 && (s4 != 7)) && (iVar5 = FUN_000b13c4(iVar5), iVar5 == 0)))) {
      iVar5 = FUN_00000bcc(0x630);
      if (iVar5 == 0) {
        uVar1 = 0;
      }
      else {
        uVar1 = FUN_000e985c(iVar5, *(int *)(*(int *)(param_1 + 0x3c) + param_3));
      }
      *(int *)(iVar4 + 0xc) = uVar1;
      FUN_0001f21c(param_1, uVar1);
      iVar4 = *(int *)(iVar4 + 0xc);
      *(int *)(iVar4 + 0x600) = 0x43000000;
      *(int *)(iVar4 + 0x604) = 0x43000000;
      *(int *)(iVar4 + 0x608) = 0x43000000;
      *(int *)(iVar4 + 0x60c) = 0x43000000;
    }
  }
  return;
}

/* NEAR 142/155 words, 128 differ, first +0x0. Probe 1 = pseudocode transcription. BODY gap: 13 words
   short and the frame is already wrong (retail 64 bytes / saves s4..s0; ours smaller), so parked after one.
   Retail decoded: param_3 *= 4; FUN_000513a0(*(p+0x3c)[param_3/4],1); FUN_00051398(float 0x4592e000,
   *(p+0x3c)[..]) with `mtc1 v0,$f12`; iVar4 = p[0x48] + param_2*0x10; the slot pointer at +0xc is
   freed via a double null test then a virtual call (*(*a0)+8)(a0,1) with sw zero,12(s1);
   iVar5 = *(DAT_003386b0 + param_2*4 + 0x70); if (DAT_002d5ab0 & 0x1000):
   switch (*(iVar5+0x4e8)) with `sltiu at,v0,13` and a JUMP TABLE AT 0x2c0580 (13 words) -> header needs
   rodata=0x2c0580; case 0 nests iVar3=*(iVar5+0x4e4) tests 2/1/0 with 0 -> byte(iVar5+0x4d8) ? 6 : 8;
   the guarded 0x630 alloc (FUN_00000bcc), FUN_000e985c, store to +0xc, FUN_0001f21c, then four
   `0x43000000` stores at +0x600/+0x604/+0x608/+0x60c.
   NEXT: Ghidra's `unaff_s4` is the switch's default leaving s4 live-in; our `s4 = -1` initializer costs a
   `li` and drops a saved register. Try `int s4;` with no initializer (MWCC accepts; UB in C but this is
   byte-matching) and re-check the frame before touching the case bodies. */
// near miss from lane dsn8 (unreviewed)
