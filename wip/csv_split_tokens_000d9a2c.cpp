/* HSTMATCH module=EBOOT.BIN start=0x000d9a2c end=0x000d9b1c symbol=FUN_000d9a2c lang=c++ */
/* Hand-written reconstruction attempt (near miss; exact match not yet verified). */
extern "C" char DAT_002d6acc[];


struct Tag {};
extern "C" void FUN_0005da00(void *, char **, Tag);
extern "C" void FUN_000d9a2c(int *vec, char *s) {
    char *local = 0;
    char *start = 0;
    char c = *s;
    if (c == 0) return;
    do {
        unsigned char uc = (unsigned char)c;
        if (uc == ',' || uc == 13 || uc == 10) {
            *s++ = 0;
            if (local == 0) local = DAT_002d6acc;
            if ((unsigned)vec[1] < (unsigned)vec[2]) {
                vec[1] = vec[1] + 1;
                *(char **)(vec[0] + vec[1] * 4 - 4) = local;
            } else {
                FUN_0005da00(vec, &local, Tag());
            }
            local = 0;
            start = 0;
        } else {
            if (start == 0) {
                local = s;
                start = s;
            }
            s++;
        }
        c = *s;
    } while (c != 0);
}
// api-hste1 NEAR 19 differ (was 35): `*s++ = 0;` in the separator arm and `s++` at the end of the else arm (not one shared s++) fix the shape and the 3rd argument is a by-value empty Tag
// (seb a2,v0). Remaining: register colours only. Retail copies vec into s1 BEFORE loading the first char (move s1,a0; sw zero,28(sp); lb a0,0(a1)) so c=a0, uc=a1, start=v1, constants a0;
// ours copies s first and keeps c=v1, uc=a0, start=a1. All 24 declaration orders of local/start/c/uc and a vec alias local give 19-30.
// improved by lane api-hste1-20261121 (unreviewed): 35 -> 19 differing words
