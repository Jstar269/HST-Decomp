/* HSTMATCH module=EBOOT.BIN start=0x001e12dc end=0x001e1614 symbol=FUN_001e12dc lang=c++ */
/* Hand-written reconstruction from the disassembly (starting from a near-miss draft of lane fm10), restructured by hand; exact match verified; matching idiom: the 21-slot walk keeps a char* induction variable (p += 4) next to the counter, as retail; names and types await human review. */
struct VC { virtual ~VC(); };

extern int *DAT_0034a130;

#define DELB(base, off)                                    if (*(VC **)((base) + (off)) != 0) {                       delete *(VC **)((base) + (off));                       *(VC **)((base) + (off)) = 0;                      }
#define DEL(off) DELB(self, off)

extern "C" void FUN_001e12dc(char *self)
{
    DEL(0x40);
    DEL(0x3c);
    DEL(0xd4);
    DEL(500);
    if (DAT_0034a130 != 0) {
        delete[] DAT_0034a130;
        DAT_0034a130 = 0;
    }
    DEL(0x9b4);
    DEL(0x810);
    DEL(0x175c);
    DEL(0x1d68);
    DEL(0x21e4);
    DEL(0x235c);
    DEL(0x2464);
    DEL(0x250c);
    {
        int i = 0;
        char *p = self;
        for (; i < 21; i++, p += 4) {
            DELB(p, 0x1adc);
        }
    }
    DEL(0x1958);
    DEL(0x195c);
    DEL(0x1d64);
    DEL(0x1954);
    DEL(0x25b8);
}
