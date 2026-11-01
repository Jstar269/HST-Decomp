/* HSTMATCH module=EBOOT.BIN start=0x00048d5c end=0x00048dd0 symbol=FUN_00048d5c lang=c99 */
/* HSTMATCH module=EBOOT.BIN start=0x00048d5c end=0x00048dd0 symbol=FUN_00048d5c lang=c99 */
/* HSTMATCH module=EBOOT.BIN start=0x00048d5c end=0x00048dd0 symbol=FUN_00048d5c lang=c99 */
extern void FUN_00000a00(char *, ...);
extern void FUN_00048e6c(int *, int, int);

int *FUN_00048d5c(int *param_1, short param_2)
{
  if (param_1 != 0) {
    param_1[1] = 0;
    FUN_00048e6c(param_1, param_1[1], 0);
    if (param_1 != 0) {
      if (*param_1 != 0) {
        FUN_00000a00((char *)*param_1);
      }
    }
    if (0 < param_2) {
      FUN_00000a00((char *)param_1);
    }
  }
  return param_1;
}
// Ruling: PROGRESS.md VERDICTS (0x00048d5c); experiment: try/hsttry/free_obj_00048d5c-6476
// BATCH 2026-11-01 (third-argument width, 3 variants): char arg, char local and (char)param_2 all 22 differ /
// 28 words, frame still 16 against retail's 32. The width of the third argument is not the missing 16 bytes.
// NEXT LEVER: retail's extra 4 words sit between the two `beqz s1` guards and the `lw a0,0(s1)`; the frame is
// the size of a 4-word local, so look for a struct/array the original passes by address rather than a scalar
// argument, and check whether the return path builds the value in a local.
// BATCH 2026-11-01 (proven levers, 5 variants): splitting the compound guard into separate statements --
// call FUN_00048e6c first, then `if (param_1 != 0) { if (*param_1 != 0) { ... } }` -- took 22 -> 11 differ and
// the word count to 29/29, with the branch targets now matching retail (+0x48db8 / +0x48da4). returnlocal 16,
// chararg 22, twoguards 25, structarg did not compile (int[4] to int* call).
// ONLY REMAINING: the frame is 16 against retail's 32 -- the four extra words sit between the prologue and
// the first guard, i.e. an allocation the source never names. NEXT LEVER: retail saves nothing between 12 and
// 28, so the 16 bytes are outgoing-argument homes or an unnamed temporary; try passing a 4-word object by
// address to the FUN_00000a00 call or declaring the return value as a struct.
// BATCH 2026-11-01 (frame reservation, 4 variants): unused int[4], char[16], float[4] and a 4-int struct all
// compile BYTE-IDENTICALLY (11 differ, 29/29 words, frame still 16). Dead locals are not allocated (V12), so the
// 16 bytes cannot be declared.
// READ EVIDENCE: retail's only sp-relative accesses in the whole function are the saves at 4/8/12(sp) -- 16..28
// is NEVER touched. So the frame is a reservation, not a used local: the source must contain something that
// forces an outgoing argument/return area (a call needing 5+ arguments, or a struct-returning callee) even
// though the emitted code never addresses it.
// NEXT LEVER: give one of the calls a struct return or a 5th argument and see whether the frame grows to 32
// without changing the 29 words; if the words change, the reservation came from the callee prototype instead.
// BATCH 2026-11-01 (call shape, 4 variants): FUN_00000a00 IS variadic -- its own prologue does
// `move s8,sp; addiu a0,s8,24`, i.e. it reads varargs out of its register-save area. Declaring it
// `void FUN_00000a00(char *, ...)` and passing the format pointer took 11 -> 10 differ (29/29 words), so the
// prototype is right -- but the FRAME STILL DOES NOT MOVE (16 against 32). variadic_noargs failed to compile,
// structret needed an undefined symbol (FUN_00000a00x), both was identical to variadic.
// CONCLUSION for this row: the reservation is not reachable from the call shape either -- a variadic callee
// saves its own a0-a3, so it does not make the caller reserve 16 bytes. Park with the frame gap as the single
// residual; the next attempt needs evidence from another item whose frame is 16 larger with untouched sp space.
// BATCH 2026-11-01 (own signature, 3 variants): declaring FUN_00048d5c itself variadic (... after param_2)
// made it WORSE -- 31 differ, 35 words -- so the target is not variadic; __builtin_va_list and
// __builtin_va_start do not exist under this compiler (both variants failed to compile). PARKED at 10 differ,
// 29/29 words: both call-shape and own-signature hypotheses are measured dead, frame is the only residual.
// CORPUS SCAN 2026-11-01 (all 51 installed EXACTs, prologue frame vs every sp-relative access): untouched frame
// bytes exist in 19 files, but ALL are at the BOTTOM of the frame -- the o32 16-byte minimum/home area -- and
// every function still packs its saves at the TOP (ra at frame-4). Examples: close_handle_00062780.c (frame 16,
// s0@8, ra@12 -> 8 bytes at 0..7 untouched, source has no locals at all) and emit_idx_0006e094.c (frame 32,
// f20@16, s0@24, ra@28 -> 16 bytes at 0..15 untouched; deleting its two unused parameters still compiles to
// frame 32). The only TOP-area case is switch_state_0022fcdc.cpp (frame 32, saves at 4/8/12), and its 16..31 is
// a REAL `float local_10[4]` stored at 16/20/24/28 and indexed with `addu v0,v0,sp` -- i.e. touched. So an
// untouched TOP reservation has no precedent among the installed matches.
// BATCH 2026-11-01 (variadic save-area, 4 variants): naming all eight integer argument registers and then `...`
// (`int *f(int *p1, short p2, int u3..u8, ...)`) reserves NOTHING -- va8 is byte-identical to the base (10
// differ, 29/29 words, frame 16). 7/6/5 named params add the unnamed t-register stores (+1/+2/+3 words). MWCC
// allocates the vararg save area only when it has unnamed registers to store, so the 16 bytes at 16..31 cannot
// come from a variadic signature either. CONCLUSION: declaration, call shape, own signature and variadic
// reserve area are all measured dead; ordinary C in this profile cannot emit a real 16-byte object that the
// body never touches. PARKED at differ=10, 29/29 words, frame 16 against retail's 32.
// improved by lane dsn8 (unreviewed): 22 -> 10 differing words
