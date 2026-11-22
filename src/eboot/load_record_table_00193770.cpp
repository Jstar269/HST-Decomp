/* HSTMATCH module=EBOOT.BIN start=0x00193770 end=0x001938d8 symbol=FUN_00193770 lang=c++ */
/* Hand-written reconstruction from the disassembly (wip near miss loader_00193770 as hint); exact match verified; matching notes: ok is set only on the success path (a size mismatch returns 0 and still frees the buffer), and the array-new helper FUN_000008d8 takes the element ctor ADDRESS as a `void *` second parameter (`&DAT_0008d650`); names and types await human review. */
extern int DAT_00341730;
extern int DAT_002e1420;
extern unsigned char DAT_002e1524;
extern char DAT_0008d650;
extern "C" int FUN_0004cad0(int, void *, int, int);
extern "C" void FUN_00011090(void *, void *, int);
extern "C" void *FUN_00000bcc(unsigned int);
extern "C" int FUN_00000a90(int);
extern "C" int FUN_000008d8(int, void *, int, int, int);
extern "C" void FUN_000104e0(int);

extern "C" int FUN_00193770(void) {
    int size;
    int count;
    int ok = 0;
    if (DAT_00341730 == 0) {
        int h = FUN_0004cad0(DAT_002e1420, &size, 16, 1);
        if (h != 0) {
            FUN_00011090(&count, (void *)h, 4);
            if (size == count * 156 + 4) {
                int *p = (int *)FUN_00000bcc(8);
                if (p != 0) {
                    p[0] = 0;
                    p[1] = 0;
                }
                DAT_00341730 = (int)p;
                if (p != 0) {
                    int n = count;
                    int mem = FUN_00000a90(n * 156 + 16);
                    mem = FUN_000008d8(mem, &DAT_0008d650, 0, 156, n);
                    ((int *)DAT_00341730)[1] = mem;
                    ((int *)DAT_00341730)[0] = count;
                    FUN_00011090((void *)((int *)DAT_00341730)[1], (void *)(h + 4), count * 156);
                    if (DAT_002e1524 != 0) {
                        DAT_002e1524 = 0;
                    }
                    ok = 1;
                }
            }
            if (h != 0) {
                FUN_000104e0(h);
            }
        }
    }
    return ok;
}
