/* HSTMATCH module=EBOOT.BIN start=0x001cf798 end=0x001d059c symbol=FUN_001cf798 lang=c++ */
/* Hand-written reconstruction from Ghidra pseudocode; near miss. */

extern "C" {
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned int uint;
typedef int undefined;

extern int DAT_002c6a9c;
extern int DAT_002c6ac4;
extern int DAT_002c6ae4;
extern int DAT_002c6b04;
extern int DAT_002c6b24;
extern int DAT_002c6b44;
extern int DAT_002c6b64;
extern int DAT_002c6b84;
extern int DAT_002c6ba4;
extern int DAT_002c6bc4;
extern int DAT_002c6be4;
extern int DAT_002c6c04;
extern int DAT_002c6c24;
extern int DAT_002c6c2c;
extern int DAT_002c6c34;
extern int DAT_002c6c3c;
extern int DAT_002c6c44;
extern int DAT_002c6c4c;
extern int DAT_002c6c58;
extern int DAT_002c6c64;
extern int DAT_002c6c70;
extern int DAT_002c6c7c;
extern int DAT_002c6c88;
extern int DAT_002c6c94;
extern int DAT_002c6c98;
extern int DAT_002cfc8c;
extern int DAT_002eacfc;
extern int DAT_002ead00;
extern int DAT_002ead14;
extern int DAT_002ead6c;
extern int DAT_002ead7c;
extern int DAT_002ead94;
extern int DAT_00309364;
extern int DAT_003093e8;
extern int DAT_0030ab9c;
extern int DAT_0030ebf4;
extern int DAT_00331b84;
extern int DAT_003386c8;
extern int DAT_00338ac4;
extern int DAT_003415ac;
extern int DAT_0034179c;
extern int DAT_0034a104;
extern int DAT_0034a120;
extern int DAT_0034a124;
extern int DAT_0034a128;
extern int DAT_0034a129;
extern int DAT_0034a12c;
extern int DAT_0034a678;
extern int DAT_0034a67c;
extern int DAT_0034a680;
extern int DAT_0034a684;

extern int FUN_00000bcc(...);
extern int FUN_00010184(...);
extern int FUN_000104e0(...);
extern int FUN_000114c0(...);
extern int FUN_0001342c(...);
extern int FUN_0001bfb4(...);
extern int FUN_0001cad0(...);
extern int FUN_0001cb88(...);
extern int FUN_0001cbfc(...);
extern int FUN_0001cc54(...);
extern int FUN_0001dd24(...);
extern int FUN_0001f06c(...);
extern int FUN_00049094(...);
extern int FUN_00054330(...);
extern int FUN_00057344(...);
extern int FUN_0006de18(...);
extern int FUN_0008d994(...);
extern int FUN_000e98f0(...);
extern int FUN_000ffca4(...);
extern int FUN_001026b8(...);
extern int FUN_0010276c(...);
extern int FUN_00102a88(...);
extern int FUN_00102e38(...);
extern int FUN_00103128(...);
extern int FUN_00103540(...);
extern int FUN_001037c8(...);
extern int FUN_001039d8(...);
extern int FUN_00103d60(...);
extern int FUN_00104118(...);
extern int FUN_0019cf80(...);
extern int FUN_0019d230(...);
extern int FUN_001b2cd0(...);
extern int FUN_001d0608(...);
extern int FUN_001d0610(...);
extern int FUN_001d0618(...);
extern int FUN_001d0624(...);
extern int FUN_001d08f0(...);
extern int FUN_001d091c(...);
extern int FUN_001d0950(...);
extern int FUN_001d31c0(...);
extern int FUN_001d31c8(...);
extern int FUN_001d31d8(...);
extern int FUN_001d31e8(...);
extern int FUN_001d3564(...);
extern int FUN_001d43b4(...);
extern int FUN_001d57f0(...);
extern int FUN_001d61e4(...);
extern int FUN_001d61ec(...);
extern int FUN_001d6200(...);
extern int FUN_001d6208(...);
extern int FUN_001d621c(...);
extern int FUN_001d6240(...);
extern int FUN_001d6264(...);
extern int FUN_001d6280(...);
extern int FUN_001d6288(...);
extern int FUN_001d62f8(...);
extern int FUN_001d6304(...);
extern int FUN_001d630c(...);
extern int FUN_001d688c(...);
extern int FUN_001d68b4(...);
extern int FUN_001d6ab0(...);
extern int FUN_001d6bb0(...);
extern int FUN_001d6bc8(...);
extern int FUN_001d9eb0(...);
extern int FUN_0022c928(...);
extern int FUN_0022c988(...);
extern int FUN_002310d0(...);
extern int FUN_00232c08(...);
extern int FUN_0026bc68(...);
extern int FUN_0026bf18(...);
extern int FUN_0027fed8(...);
extern int FUN_00281090(...);
extern int FUN_002a03c0(...);
extern int FUN_002a1a64(...);
extern int FUN_002a1d7c(...);
extern int FUN_002a2434(...);
extern int FUN_002a45d8(...);
extern int FUN_002a4938(...);
extern int FUN_002b7d10(...);
extern int FUN_002b7d18(...);
extern int FUN_002b7d20(...);

struct EmptyTag {};
void FUN_001cf798(undefined4 *param_1, EmptyTag tag = EmptyTag())

{
  bool bVar1;
  char cVar2;
  undefined2 uVar3;
  short sVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  int iVar9;
  undefined *puVar10;
  undefined1 *puVar11;
  char *pcVar12;
  undefined4 *puVar13;
  int iVar14;
  undefined4 *puVar15;
  int iVar16;
  uint uVar17;
  undefined **ppuVar18;
  int iVar19;
  undefined **ppuVar20;
  char dummy_pad[16]; (void)&dummy_pad;
  undefined1 auStack_30 [16];
  undefined1 auStack_20 [8];
  undefined1 auStack_18 [8];
  undefined1 auStack_10 [16];
  
  *(undefined4 **volatile)&param_1 = param_1;
  FUN_0001f06c(param_1);
  *param_1 = (undefined4)&DAT_00309364;
  param_1[0xf] = 0;
  DAT_0034a120 = (int)param_1;
  FUN_0001dd24(DAT_002cfc8c);
  FUN_00049094(1);
  FUN_002a4938();
  FUN_00281090(*(undefined4 *)(DAT_003386c8 + 0x40));
  DAT_0034a124 = FUN_00010184(0x40,0x200000);
  FUN_0001bfb4(0x30ab9c,DAT_0034a124,0x200000);
  cVar2 = FUN_0022c988(8);
  if (cVar2 == '\0') {
    pcVar12 = (char *)&DAT_002ead94;
  }
  else {
    pcVar12 = (char *)&DAT_002ead7c;
  }
  FUN_0001cad0(0x30ebf4,1,pcVar12);
  FUN_001b2cd0(0x34179c,0);
  puVar5 = (undefined4 *)FUN_00000bcc(0x11a0);
  if (puVar5 != (undefined4 *)0x0) {
    FUN_00102e38(puVar5);
    *puVar5 = (undefined4)&DAT_003093e8;
    puVar5[0x15] = 0;
    puVar5[0x16] = 0;
    puVar5[0x17] = 0;
    puVar5[0x18] = 0;
    FUN_001d6bc8(puVar5 + 0x24);
    puVar5[0x35] = 0;
    puVar5[0x36] = 0;
    puVar15 = puVar5 + 0x49;
    do {
      FUN_001d6bb0(puVar15);
      puVar15 = puVar15 + 4;
      puVar13 = puVar5 + 0x59;
    } while (puVar15 < puVar13);
    do {
      FUN_00103d60(puVar13);
      puVar13 = puVar13 + 1;
    } while (puVar13 < puVar5 + 0x5b);
    FUN_00103d60(puVar5 + 0x60);
    FUN_000e98f0(puVar5 + 100);
    puVar15 = puVar5 + 0x78;
    do {
      FUN_00057344(puVar15);
      puVar15 = puVar15 + 4;
    } while (puVar15 < puVar5 + 0x9c);
    FUN_002a03c0(puVar5 + 0xa4);
    FUN_001d6ab0(puVar5 + 0x124);
    puVar5[0x144] = 0x40c00000;
    puVar5[0x148] = 0;
    *(undefined1 *)(puVar5 + 0x149) = 0;
    FUN_0026bc68(puVar5 + 0x14c);
    FUN_001d68b4(puVar5 + 0x444);
    FUN_001d688c(DAT_003415ac);
    FUN_0019cf80(DAT_00331b84);
    FUN_001d630c(puVar5);
    FUN_00104118(puVar5,DAT_002c6c98,0);
    FUN_00103540(0x447a0000,puVar5);
    FUN_001037c8(puVar5,0x10);
    uVar6 = FUN_0008d994(DAT_003386c8);
    FUN_00103540(0x47c35000,uVar6);
    uVar6 = FUN_0008d994(DAT_003386c8);
    FUN_001037c8(uVar6,0xffffffff);
    uVar6 = FUN_001039d8(puVar5,&DAT_002c6c94,0);
    uVar6 = FUN_001026b8(uVar6,DAT_002c6c88);
    FUN_001d62f8(uVar6,puVar5 + 0x19);
    DAT_0034a12c = FUN_001d9eb0(0);
    DAT_0034a12c = DAT_0034a12c + -1;
    FUN_001d6288(puVar5[0x19]);
    uVar6 = FUN_001039d8(puVar5,DAT_002c6c7c,0);
    puVar5[0x1a] = uVar6;
    uVar7 = FUN_001026b8(uVar6,DAT_002c6c70);
    puVar5[0x23] = uVar7;
    FUN_001d6264(puVar5 + 0x24,uVar7);
    uVar7 = FUN_0010276c(uVar6,DAT_002c6c64);
    uVar8 = FUN_00102a88(uVar6,DAT_002eacfc,uVar7);
    puVar5[0x21] = uVar8;
    uVar7 = FUN_00102a88(uVar6,DAT_002ead00,uVar7);
    puVar5[0x22] = uVar7;
    iVar16 = 0;
    ppuVar18 = (undefined **)&DAT_002eacfc;
    iVar19 = 0;
    ppuVar20 = (undefined **)&DAT_002ead14;
    iVar14 = 0;
    do {
      uVar7 = FUN_001026b8(uVar6,*ppuVar18);
      puVar10 = *ppuVar20;
      *(undefined4 *)((int)puVar5 + iVar19 + 0x6c) = uVar7;
      uVar7 = FUN_001026b8(uVar6,puVar10);
      uVar3 = FUN_001d6240(uVar7);
      *(undefined2 *)((int)puVar5 + iVar14 + 0x9c) = uVar3;
      uVar3 = FUN_001d621c(uVar7);
      *(undefined2 *)((int)puVar5 + iVar14 + 0x9e) = uVar3;
      iVar9 = FUN_001d6208(uVar7);
      *(float *)((int)puVar5 + iVar14 + 0xa0) = (float)iVar9;
      FUN_000ffca4(uVar7);
      iVar9 = FUN_001d6208(uVar7);
      iVar16 = iVar16 + 1;
      ppuVar18 = ppuVar18 + 1;
      iVar19 = iVar19 + 4;
      ppuVar20 = ppuVar20 + 1;
      *(float *)((int)puVar5 + iVar14 + 0xa0) =
           *(float *)((int)puVar5 + iVar14 + 0xa0) / (float)iVar9;
      iVar14 = iVar14 + 8;
    } while (iVar16 < 6);
    uVar7 = FUN_001026b8(uVar6,DAT_002c6c58);
    puVar5[0x466] = uVar7;
    uVar3 = FUN_001d6240(uVar7);
    *(undefined2 *)(puVar5 + 0x9e) = uVar3;
    uVar3 = FUN_001d621c(puVar5[0x466]);
    *(undefined2 *)((int)puVar5 + 0x27a) = uVar3;
    uVar7 = FUN_001d6200(puVar5[0x1b]);
    puVar5[0x33] = uVar7;
    uVar7 = FUN_001d6200(puVar5[0x1c]);
    puVar5[0x34] = uVar7;
    uVar3 = FUN_001d6208(puVar5[0x1b]);
    *(undefined2 *)(puVar5 + 0x9f) = uVar3;
    uVar3 = FUN_001d61ec(puVar5[0x1b]);
    *(undefined2 *)((int)puVar5 + 0x27e) = uVar3;
    uVar3 = FUN_001d6208(puVar5[0x1c]);
    *(undefined2 *)(puVar5 + 0xa0) = uVar3;
    uVar3 = FUN_001d61ec(puVar5[0x1c]);
    *(undefined2 *)((int)puVar5 + 0x282) = uVar3;
    FUN_000114c0(puVar5 + 0x37,0,8);
    uVar17 = 0;
    ppuVar18 = (undefined **)&DAT_002ead6c;
    iVar16 = 0;
    iVar14 = 0;
    iVar19 = 0;
    do {
      uVar7 = FUN_001026b8(uVar6,*ppuVar18);
      puVar10 = *ppuVar18;
      *(undefined4 *)((int)puVar5 + iVar16 + 0x114) = uVar7;
      uVar7 = FUN_00102a88(uVar6,puVar10,0xffffffff);
      *(undefined4 *)((int)puVar5 + iVar16 + 0x104) = uVar7;
      FUN_001d61e4(uVar7);
      FUN_001d61e4(*(undefined4 *)((int)puVar5 + iVar16 + 0x114));
      *(undefined2 *)((int)puVar5 + iVar14 + 0xe4) =
           *(undefined2 *)(*(int *)((int)puVar5 + iVar16 + 0x104) + 4);
      *(undefined2 *)((int)puVar5 + iVar14 + 0xe6) =
           *(undefined2 *)(*(int *)((int)puVar5 + iVar16 + 0x104) + 6);
      FUN_0027fed8((int)puVar5 + iVar19 + 0x124,*(undefined4 *)((int)puVar5 + iVar16 + 0x104),
                   uVar17 == 0 || uVar17 == 2);
      uVar17 = uVar17 + 1;
      ppuVar18 = ppuVar18 + 1;
      iVar16 = iVar16 + 4;
      iVar14 = iVar14 + 8;
      iVar19 = iVar19 + 0x10;
    } while (uVar17 < 4);
    uVar7 = FUN_001026b8(uVar6,DAT_002c6c4c);
    puVar5[0x5b] = uVar7;
    sVar4 = FUN_001d621c(uVar7);
    *(short *)((int)puVar5 + 0x176) = sVar4 - *(short *)((int)puVar5 + 0x9e);
    puVar5[0x5e] = 0x3f6e147b;
    uVar7 = FUN_001026b8(uVar6,&DAT_002c6c44);
    puVar5[0x5f] = uVar7;
    FUN_001d61e4(uVar7);
    uVar6 = FUN_001026b8(uVar6,DAT_002c6c3c);
    puVar5[0x61] = uVar6;
    FUN_001d61e4(uVar6);
    uVar17 = 0;
    puVar15 = puVar5;
    do {
      iVar14 = (int)(uVar17 + 5) % 6;
      uVar6 = FUN_0006de18((float)(int)*(short *)(puVar5 + iVar14 * 2 + 0x27),
                           (float)(int)*(short *)((int)puVar5 + iVar14 * 8 + 0x9e),0,
                           puVar5[iVar14 * 2 + 0x28],auStack_30);
      FUN_00054330(puVar15 + 0x78,uVar6);
      uVar17 = uVar17 + 1;
      puVar15 = puVar15 + 4;
    } while (uVar17 < 9);
    puVar5[0x9c] = 0;
    puVar5[0x9d] = 0;
    uVar6 = FUN_001039d8(puVar5,DAT_002c6c34,0);
    uVar6 = FUN_001026b8(uVar6,DAT_002c6c2c);
    puVar5[0x145] = uVar6;
    uVar6 = FUN_001d6200(uVar6);
    puVar5[0x146] = uVar6;
    uVar6 = FUN_001039d8(puVar5,DAT_002c6c24,0);
    puVar5[0x467] = uVar6;
    FUN_002a1d7c(puVar5 + 0xa4,uVar6);
    FUN_002a2434(puVar5 + 0xa4);
    puVar5[0x147] = 6;
    if (DAT_0034a67c == 0x48) {
      puVar5[0x35] = 0;
      DAT_0034a104 = 0;
    }
    iVar14 = 0;
    if (0 < DAT_0034a104) {
      do {
        FUN_001d57f0(puVar5,2,0);
        iVar14 = iVar14 + 1;
      } while (iVar14 < DAT_0034a104);
    }
    iVar14 = 0;
    do {
      FUN_00103128(puVar5);
      FUN_001d43b4(puVar5);
      FUN_001d3564(puVar5);
      FUN_001d31e8(puVar5);
      iVar14 = iVar14 + 1;
      puVar5[0x18] = puVar5[0x18] + 1;
    } while (iVar14 < 6);
    FUN_001d57f0(puVar5,3,0);
    iVar14 = 0;
    do {
      FUN_00103128(puVar5);
      FUN_001d43b4(puVar5);
      FUN_001d3564(puVar5);
      FUN_001d31e8(puVar5);
      puVar5[0x18] = puVar5[0x18] + 1;
      if (puVar5[0x15] == 4) break;
      iVar14 = iVar14 + 1;
    } while (iVar14 < 0x1e);
    FUN_0001cb88(0x30ebf4);
    uVar6 = FUN_0001cbfc();
    uVar7 = FUN_0001cc54();
    FUN_0001bfb4(0x30ab9c,uVar6,uVar7);
    if (DAT_0034a124 != 0) {
      FUN_000104e0(DAT_0034a124);
      DAT_0034a124 = 0;
    }
    iVar14 = FUN_00000bcc(0x14);
    if (iVar14 != 0) {
      FUN_00232c08(iVar14);
    }
    iVar14 = FUN_00000bcc(0xc);
    if (iVar14 != 0) {
      FUN_002310d0(iVar14,0);
    }
    FUN_001d31d8(puVar5);
    FUN_001d31c8(puVar5);
    FUN_001d31c0(puVar5 + 0x444,puVar5 + 0xa4);
    iVar14 = 0x59;
    uVar17 = 0;
    puVar11 = (undefined1 *)&DAT_0034a678;
    do {
      if ((*(int *)(puVar11 + 4) != 0x1e) && (*(int *)(puVar11 + 4) != 0x4b)) {
        iVar14 = *(int *)(&DAT_0034a67c + uVar17 * 4);
        break;
      }
      uVar17 = uVar17 + 1;
      puVar11 = puVar11 + 4;
    } while (uVar17 < 10);
    bVar1 = false;
    uVar17 = 0;
    puVar11 = (undefined1 *)&DAT_0034a678;
    do {
      if ((int)uVar17 < 1) {
        iVar16 = *(int *)(puVar11 + 4);
      }
      else {
        if (*(int *)(puVar11 + 4) == 0x1e) break;
        iVar16 = *(int *)(puVar11 + 4);
      }
      if ((((iVar16 == 0x44) || (iVar16 == 0x35)) || (iVar16 == 0x31)) ||
         ((iVar16 == 0x2a || (iVar16 == 0)))) {
        bVar1 = true;
      }
      uVar17 = uVar17 + 1;
      puVar11 = puVar11 + 4;
    } while (uVar17 < 10);
    if (iVar14 == 0x44) {
      FUN_001d0950(puVar5,1);
      puVar5[0x35] = 1;
      FUN_001d091c(puVar5 + 0x14c);
      FUN_001d08f0(DAT_002c6c04,0);
    }
    else if (((((DAT_0034a67c == 0x3e) || (DAT_0034a67c == 0x37)) ||
              ((DAT_0034a67c == 0x29 || (DAT_0034a680 == 0x2a)))) ||
             ((((DAT_0034a684 == 0x2a && (DAT_0034a67c == 0x4b)) || (DAT_0034a67c == 0x2b)) ||
              ((DAT_0034a67c == 0x39 || (DAT_0034a67c == 0x30)))))) ||
            ((DAT_0034a680 == 0x31 || ((DAT_0034a684 == 0x31 && (DAT_0034a67c == 0x4b)))))) {
      if (bVar1) {
        uVar17 = FUN_0001342c();
        if ((uVar17 & 1) == 0) {
          uVar6 = 0x19;
        }
        else {
          uVar6 = 0x17;
        }
        FUN_002a1a64(puVar5 + 0xa4,uVar6);
      }
      FUN_001d0950(puVar5,1);
      FUN_001d0624(puVar5 + 0x444,2);
      if ((((DAT_0034a67c == 0x2b) || (DAT_0034a67c == 0x39)) || (DAT_0034a67c == 0x30)) ||
         ((DAT_0034a680 == 0x31 || ((DAT_0034a684 == 0x31 && (DAT_0034a67c == 0x4b)))))) {
        FUN_001d0618(puVar5 + 0x444,1);
      }
      FUN_001d08f0(DAT_002c6be4,1);
    }
    else if (DAT_0034a67c - 0x3fU < 4) {
      FUN_001d0950(puVar5,1);
      if (DAT_0034a680 - 0x3cU < 2) {
        FUN_001d0624(puVar5 + 0x444,2);
        FUN_001d08f0(DAT_002c6bc4,2);
      }
      else if (DAT_0034a680 == 0x3b) {
        puVar5[0x35] = 1;
        FUN_001d0610(puVar5 + 0x14c);
        FUN_001d08f0(DAT_002c6ba4,3);
      }
      else {
        FUN_0026bf18(puVar5 + 0x14c,1);
        FUN_001d08f0(DAT_002c6b84,4);
      }
    }
    else if (DAT_0034a67c == 0x3d) {
      puVar5[0x35] = 1;
      FUN_001d0950(puVar5);
      if ((DAT_00338ac4 == 0) && (DAT_0034a680 != 0x3c)) {
        FUN_0026bf18(puVar5 + 0x14c,2);
        FUN_001d08f0(DAT_002c6b44,6);
      }
      else {
        FUN_0026bf18(puVar5 + 0x14c,3);
        FUN_001d08f0(DAT_002c6b64,5);
      }
    }
    else if ((DAT_0034a67c == 0x43) || (DAT_0034a67c == 0x4e)) {
      FUN_001d0950(puVar5,1);
      FUN_0026bf18(puVar5 + 0x14c,1);
      FUN_001d08f0(DAT_002c6b24,7);
    }
    else if (DAT_0034a678 == 0x1f) {
      FUN_002a45d8(puVar5 + 0xa4);
      FUN_001d0950(puVar5,1);
      FUN_001d0624(puVar5 + 0x444,7);
      FUN_001d08f0(DAT_002c6b04,8);
    }
    else if (DAT_0034a67c == 0x48) {
      FUN_001d0950(puVar5,1);
      FUN_001d0624(puVar5 + 0x444,7);
      FUN_001d08f0(DAT_002c6ae4,9);
    }
    else if (DAT_0034a680 == 0x20) {
      FUN_002a45d8(puVar5 + 0xa4);
      FUN_001d08f0(DAT_002c6ac4,10);
    }
    else if ((DAT_0034a67c == 0x1d) || (DAT_0034a67c == 8)) {
      iVar14 = FUN_0019d230();
      if (iVar14 == 0) {
        if (DAT_0034a129 == '\0') {
          DAT_0034a129 = '\x01';
          FUN_002a1a64(puVar5 + 0xa4,0x1e);
        }
      }
      else {
        iVar14 = FUN_0022c928(8);
        if ((iVar14 == 0) || (iVar14 = FUN_0022c988(8), iVar14 == 0)) {
          if (DAT_0034a128 == '\0') {
            DAT_0034a128 = '\x01';
            iVar16 = 0xc;
            iVar14 = FUN_002b7d18(auStack_18);
            if (iVar14 == 0) {
              FUN_002b7d10(auStack_18,auStack_20);
              FUN_002b7d20(auStack_10,auStack_20);
              iVar16 = FUN_001d0608(auStack_10);
              bVar1 = iVar16 < 4;
            }
            else {
              FUN_001d08f0(DAT_002c6a9c,iVar14);
              bVar1 = false;
            }
            if ((bVar1) || (10 < iVar16)) {
              if ((iVar16 < 0xb) || (0x11 < iVar16)) {
                FUN_002a1a64(puVar5 + 0xa4,0x1c);
              }
              else {
                FUN_002a1a64(puVar5 + 0xa4,0x1b);
              }
            }
            else {
              FUN_002a1a64(puVar5 + 0xa4,0x1a);
            }
          }
        }
        else {
          FUN_002a45d8(puVar5 + 0xa4);
        }
      }
    }
  }
  param_1[0xf] = (undefined4)puVar5;
  return;
}
}

// Differing words: 808 / 897 words. First difference at +0x28.
// Ruled out:
// - Plain C (c99): frame is 96 instead of 112; C++ with trailing EmptyTag parameter matches the 112-byte frame.
// - Parameter allocation: retail homes param_1 at 60(sp) and leaves s8..s0 for loop/array indices, while MWCC prioritizes param_1 for s8.
// - Object layout: 4512-byte allocated menu object (0x11a0) needs high-level C++ struct recovery.
// near miss from lane dsn4 (unreviewed)
