/* HSTMATCH module=EBOOT.BIN start=0x0021dd14 end=0x0021de38 symbol=FUN_0021dd14 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern int *DAT_0034b5d4;
extern int FUN_002af920(int arg);
extern void FUN_002afa48(int *pIdx, void *zero, int val);
extern void FUN_0021df6c(void *self, int idx);
extern void FUN_0021e090(void *self, float pct);
struct Obj_0021dd14 { char pad[0x170]; int f170; char pad2[0x1f0-0x174]; int f1F0; int f1F4; int f1F8; int f1FC; int f200; };
static inline float Val(int i)
{
    int *t = DAT_0034b5d4;
    float r;
    if (i >= 0 && i < 0x1e) r = (float)(t[i * 2] >> 4);
    else r = 0.0f;
    return r;
}
void FUN_0021dd14(struct Obj_0021dd14 *self)
{
    float fVar2;
    int idx;
    self->f1F8 = FUN_002af920(self->f170);
    self->f200 = self->f1F8;
    self->f1FC = self->f1F8;
    FUN_002afa48(&idx, 0, self->f1F8);
    FUN_0021df6c(self, idx);
    fVar2 = Val(idx);
    self->f1F0 = (int)fVar2;
    fVar2 = Val(idx + 1);
    self->f1F4 = (int)fVar2;
    FUN_0021e090(self, (float)(self->f200 - self->f1F0) * 100.0f / (float)((int)(fVar2) - self->f1F0));
}
