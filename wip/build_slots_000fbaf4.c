/* HSTMATCH module=EBOOT.BIN start=0x000fbaf4 end=0x000fbbf8 symbol=FUN_000fbaf4 lang=c99 */
extern void *FUN_00000bcc(unsigned size);
extern void *FUN_00050fc0(void *mem, char *name);
extern void FUN_000511c4(void *obj, char *name, int flag);
extern void FUN_00051398(void *obj, float value);
extern void FUN_00102e38(void *obj);
extern void FUN_00104118(void *obj, char *name, int flag);
extern void FUN_00103540(void *obj, float value);
extern void FUN_001037c8(void *obj, int mode);
extern void FUN_00103628(void *obj, int flag);
extern unsigned char DAT_003386b8;
extern char DAT_002d8324[];
extern char *DAT_002c0d5c[];
void FUN_000fbaf4(char *self)
{
    if (DAT_003386b8 != 0) {
        int i = 0;
        char *slot = self;
        char **src = DAT_002c0d5c;
        do {
            void *obj = FUN_00000bcc(72);
            if (obj != 0)
                obj = FUN_00050fc0(obj, src[i]);
            *(void **)(slot + 64) = obj;
            FUN_000511c4(obj, src[i], 0);
            FUN_00051398(*(void **)(slot + 64), 100000.0f);
            i++;
            slot += 4;
        } while (i < 6);
        void *q = FUN_00000bcc(84);
        if (q != 0)
            FUN_00102e38(q);
        *(void **)(self + 0x3c) = q;
        FUN_00104118(q, DAT_002d8324, 0);
        FUN_00103540(*(void **)(self + 0x3c), 100000.0f);
        FUN_001037c8(*(void **)(self + 0x3c), -1);
        FUN_00103628(*(void **)(self + 0x3c), 1);
    }
}
// api-hste1 NEAR 50 differ (was 68): the new object must not stay in a saved register: retail stores the ctor result once at the join and re-reads it from the slot for the
// later calls (`F(obj,..)` uses v0, `F2(*(void **)(slot + 64), ..)` / `*(void **)(self + 0x3c)` reload), and the q block likewise. Remaining: ours puts `sw v0,64(s1)` into the
// `beqzl v0` delay slot (store hoisted from the join) where retail has `lw a1,0(s0)` there and the store after the join; `F(*(void **)(slot+64) = obj, src[i], 0)` (right-to-left arg
// evaluation) did not move it.
// improved by lane api-hste1-20261121 (unreviewed): 68 -> 50 differing words
