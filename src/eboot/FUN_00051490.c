/* HSTMATCH module=EBOOT.BIN start=0x00051490 end=0x00051580 symbol=FUN_00051490 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
typedef struct S { float f; int *pp; int cnt; int a, b, c; } S;
typedef struct O { int p0; int m4; int pad[4]; float f24; int pad2[3]; int m[24]; } O;
extern int *DAT_0032b17c;
extern int FUN_0003cc80(int *g, S *s);
extern void FUN_0003ca68(int *g, int v);
extern int FUN_0003cc48(int *g, float f);
extern void FUN_00051580(int *self, int v);
void FUN_00051490(int *self, int a1, int a2, int a3, int a4, int cnt, int *pp)
{
    int loc;
    self[11] = a1;
    self[12] = a2;
    self[13] = a3;
    self[14] = a4;
    loc = self[1];
    if (cnt <= 0 || pp == 0) {
        pp = &loc;
        cnt = self[1] >= 0;
    }
    S s = {0};
    s.f = ((float *)self)[6];
    s.pp = pp;
    s.cnt = cnt;
    s.a = self[15];
    s.b = self[16];
    s.c = self[17];
    self[9] = FUN_0003cc80(DAT_0032b17c, &s);
    FUN_0003ca68(DAT_0032b17c, self[9]);
    self[10] = FUN_0003cc48(DAT_0032b17c, 0.0f);
    FUN_00051580(self, -(*pp < 0));
}
