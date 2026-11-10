/* HSTMATCH module=EBOOT.BIN start=0x0018cb38 end=0x0018cd90 symbol=FUN_0018cb38 lang=c99 */
extern void FUN_0018cd90(const char *fmt, ...);
extern char DAT_002e11d4[];
extern char DAT_002e1208[];

int FUN_0018cb38(unsigned int code)
{
    int r = 0;

    switch (code) {
    case 0:
        break;
    case 0x80110301:
    case 0x80110381:
    case 0x80110341:
    case 0x801103c1:
    case 1:
    case 2:
        r = 1;
        break;
    case 0x80110302:
    case 0x80110382:
    case 0x80110342:
    case 0x801103c2:
        r = 2;
        break;
    case 0x80110383:
        r = 3;
        break;
    case 0x80110384:
    case 0x80110344:
        r = 4;
        break;
    case 0x80110305:
    case 0x80110385:
    case 0x80110345:
    case 0x801103c5:
        r = 5;
        break;
    case 0x80110306:
    case 0x80110386:
    case 0x801103c6:
        r = 6;
        break;
    case 0x80110307:
    case 0x80110347:
    case 0x801103c7:
        r = 7;
        break;
    case 0x80110308:
    case 0x80110388:
    case 0x80110348:
    case 0x801103c8:
        r = 8;
        break;
    case 0x8011030b:
    case 0x8011038b:
    case 0x8011034b:
    case 0x801103cb:
        r = 9;
        break;
    case 0x80110309:
        r = 10;
        break;
    default:
        FUN_0018cd90(DAT_002e11d4, code);
        break;
    }
    FUN_0018cd90(DAT_002e1208, code, r);
    return r;
}
// NEAR (lane w3f4; 60 differing words, but only 4 instructions are really different: every later
// branch offset shifts). Fixed from the fm4 draft (66): ONE case per result value (MWCC turns the
// first-tested label of each case into `beql s1,v1,join; li s0,k` and points the other labels at the
// value block), labels written in reverse test order. Left: retail keeps an UNREFERENCED `li s0,3; b join;
// nop` block for the single-label case 0x80110383 (ours deletes it after the beql conversion) and leaves
// the chain's final `b <print>` slot empty (ours fills it with the print's `lui a0`). Ruled out: C++,
// int code with (int) labels, `if(code!=0)` in default without case 0 (default moves), default first
// or after case 0 (laid out in place), the switch in a static inline helper (with r or with returns).
// It looks like a late cleanup pass that did not run on retail's function; no source spelling found.
// improved by lane w3f4 (unreviewed): 66 -> 60 differing words
