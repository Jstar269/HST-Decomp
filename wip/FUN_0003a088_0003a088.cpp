/* HSTMATCH module=EBOOT.BIN start=0x0003a088 end=0x0003a0fc symbol=FUN_0003a088 lang=c99 */

extern void *FUN_00039f38(void *owner, int kind);
extern void FUN_0003ce98(void *o, void *owner);
extern void FUN_0003dbf4(int owner, int key, int list);

void *FUN_0003a088(void *param_1, float param_2)
{
    float f = param_2;
    void *o;
    void *result;
    unsigned int float_as_int;
    unsigned int tmp;
    unsigned int key;

    o = FUN_00039f38(param_1, 0x104);
    FUN_0003ce98(o, param_1);
    result = o;
    
    float_as_int = *(unsigned int *)&f;
    tmp = 0x80000000u | ((int)float_as_int >> 31);
    key = float_as_int ^ tmp;
    FUN_0003dbf4(*(int *)((char *)param_1 + 0xe0), key, *(int *)((char *)o + 0x81c));
    return result;
}
// NEAR: 21 words differ. Core issue: retail moves float param to $f20 (callee-saved) in prologue (mov.s $f20,$f12; swc1 $f20,32(sp)), then stores $f20 to 0(sp) for bit cast (swc1 $f20,0(sp); lw v1,0(sp)). Our code spills $f12 directly (swc1 $f12,28(sp); lw a1,28(sp)). Tried: volatile float, separate declaration/assignment, union, factor*1.0f, volatile pointer cast - all compile to same f12 spill. Frame size 32 vs 48 (missing f20 save). Register allocation for bit manipulation differs: retail uses v1 for float-as-int, v0 for 0x80000000, a1 for result; ours uses a1, v1, v0. Declaration order changes didn't fix. Ruled out: dummy float ops, volatile, union, extra locals. Need: force MWCC to allocate f20 for float param live across calls despite bit-cast-only use.
// near miss from lane dsn14 (unreviewed)
