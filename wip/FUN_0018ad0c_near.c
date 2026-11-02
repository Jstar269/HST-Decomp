/* HSTMATCH module=EBOOT.BIN start=0x0018ad0c end=0x0018b1d0 symbol=FUN_0018ad0c lang=c99 */
struct Cfg { char pad[0x8b]; signed char lo : 2; signed char mode : 2; };
struct Sub { char pad[100]; float f100; };
struct Obj {
    char pad0[0x5c];
    int f5c;
    int f60;
    int f64;
    char pad1[4];
    unsigned char f6c;
    unsigned char f6d;
    unsigned char f6e;
    char pad2[1];
    unsigned char f70;
    char pad3[0x7c - 0x71];
    struct Sub *f7c;
    struct Sub *f80;
    char pad4[0x8c - 0x84];
    float f8c;
    char pad5[0x98 - 0x90];
    char *f98;
    char text[0xc00];
};
extern struct Cfg *DAT_002cfc8c;
extern float DAT_002e0b80, DAT_002e0b84, DAT_002e0b88;
extern int DAT_002e0b8c, DAT_002e0b90;
extern char s_lobby_text_002e0b94[];
extern char DAT_0034179c[];
extern char *FUN_0022cd60(void);
extern void *memcpy(void *, const void *, unsigned int);
extern void FUN_0018aa0c(struct Obj *, int);
extern void FUN_001b28f0(char *, char *);

void FUN_0018ad0c(struct Obj *p)
{
    char cVar1;
    unsigned char ucFlag;
    int bVar2;
    int m;
    unsigned int uVar4;
    int iVar5;
    int uVar6;
    int iVar7;
    unsigned int uVar8;
    int uVar10;
    int cnt;
    float fVar11;
    float fVar12;

    m = *(int *)(FUN_0022cd60() + 0x134);
    if (p->f6d == 0)
        m = 1;
    iVar7 = 1;
    uVar10 = 0;
    if (m == 2) {
        iVar7 = 3;
    } else {
        if (m == 1) {
            ucFlag = p->f6e;
            goto LAB_74;
        }
        if (m == 0)
            uVar10 = 1;
    }
    ucFlag = p->f6e;
LAB_74:
    fVar12 = DAT_002e0b88;
    if (ucFlag == 0)
        fVar12 = DAT_002e0b84;
    if (p->f60 < 1) {
        fVar11 = p->f7c->f100;
        if (fVar11 <= p->f8c - DAT_002e0b80 * (float)(p->f64 - 1)) {
            bVar2 = 0;
            if (p->f6d != 0) {
                if (DAT_002cfc8c->mode != 0)
                    bVar2 = (p->f70 != 0);
                else
                    bVar2 = 1;
            }
            cnt = 0;
            do {
                if (bVar2) {
                    fVar11 = p->f7c->f100;
                    if (fVar11 <= p->f8c - DAT_002e0b80 * (float)(p->f64 - 1)) {
                        uVar8 = p->f5c;
                    } else {
                        p->f7c->f100 = fVar11 - fVar12;
                        p->f80->f100 = p->f80->f100 - fVar12;
                        goto incr;
                    }
                } else {
                    uVar8 = p->f5c;
                }
                uVar4 = uVar8 & 1;
                if ((int)uVar8 < 0 && uVar4 != 0)
                    uVar4 = uVar4 - 2;
                uVar6 = uVar10;
                if (uVar4 == 0)
                    uVar6 = 0;
                p->f60 = uVar6;
                iVar5 = p->f5c;
                p->f5c = iVar5 + 1;
                memcpy((char *)p + p->f64 * 0x400 + 0x9c, p->f98, iVar5 + 2);
                uVar8 = p->f64;
                iVar5 = p->f5c;
                cVar1 = *(char *)((char *)p + uVar8 * 0x400 + iVar5 + 0x9c);
                if (cVar1 == '/' || cVar1 == '_') {
                    cnt = (cnt / 3) * 3 + 1;
                } else if (cVar1 == 7 ||
                           (iVar5 == 1 && *(char *)((char *)p + uVar8 * 0x400 + 0x9c) == 7)) {
                    cnt = (cnt / 3) * 3;
                } else if (cVar1 == 11 ||
                           (iVar5 == 1 && *(char *)((char *)p + uVar8 * 0x400 + 0x9c) == 11)) {
                    uVar4 = 0;
                    if (uVar8 == 0)
                        uVar4 = (iVar5 == 1);
                    cnt = (cnt / 3) * 3 + uVar4;
                } else if (cVar1 == 8 ||
                           (iVar5 == 1 && *(char *)((char *)p + uVar8 * 0x400 + 0x9c) == 8)) {
                    uVar4 = 0;
                    if (uVar8 == 0)
                        uVar4 = (iVar5 == 1);
                    cnt = (cnt / 3) * 3 + -1 + uVar4;
                } else {
                    if (cVar1 == 0) {
                        p->f98 = p->f98 + iVar5;
                        p->f5c = 0;
                        if (p->f6d == 0) {
                            uVar6 = DAT_002e0b90;
                            if (p->f6e == 0)
                                uVar6 = 0;
                            p->f60 = uVar6;
                        } else {
                            uVar6 = DAT_002e0b8c;
                            if (p->f6e == 0)
                                uVar6 = 0;
                            p->f60 = uVar6;
                        }
                        p->f6c = 1;
                        FUN_0018aa0c(p, 2);
                        break;
                    }
                    if (cVar1 == 10) {
                        uVar6 = DAT_002e0b8c;
                        if (p->f6e == 0)
                            uVar6 = 0;
                        p->f60 = uVar6;
                        iVar5 = p->f5c + 1;
                        p->f5c = iVar5;
                        *(char *)((char *)p + p->f64 * 0x400 + iVar5 + 0x9c) = 0;
                        p->f98 = p->f98 + p->f5c;
                        p->f5c = 0;
                        uVar8 = p->f64 + 1;
                        p->f64 = uVar8;
                        uVar4 = uVar8 & 1;
                        if ((int)uVar8 < 0 && uVar4 != 0)
                            uVar4 = uVar4 - 2;
                        if (uVar4 == 0) {
                            p->f64 = 0;
                            FUN_0018aa0c(p, 2);
                            break;
                        }
                        if (!bVar2)
                            break;
                    }
                }
                *(char *)((char *)p + p->f64 * 0x400 + p->f5c + 0x9c) = 0;
            incr:
                cnt = cnt + 1;
            } while (cnt < iVar7 * 3 || bVar2);
            FUN_001b28f0(DAT_0034179c, s_lobby_text_002e0b94);
        } else {
            p->f7c->f100 = fVar11 - fVar12;
            p->f80->f100 = p->f80->f100 - fVar12;
        }
    } else {
        p->f60 = p->f60 + -1;
    }
}
// near miss: 311/305 words, 308 differ, first +0x8. Faithful reconstruction from the address-labelled
// disassembly (mode dispatch, f60 countdown, float-threshold early return, three-line text scanner).
// Ruled out / residual: an extra callee-saved s5 holds p->f80 across the loop (retail reloads lw v0,128(s3))
// because my source references p->f80->f100 in two places; also f6e read as lb not lbu; the whole body is
// 6 words long. Prior lane dsn3 reached 307/305 (287 differ) on the same structure and parked it.
// near miss from lane dsn6 (unreviewed)
