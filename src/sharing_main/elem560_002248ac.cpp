/* HSTMATCH module=hstp_trans_main.prx start=0x002248ac end=0x0022492c symbol=FUN_002248ac lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: the bounds check is an inline operator[] with its own assert (the C spelling with an explicit local index misses). */
extern "C" void FUN_002164c4(...);
extern "C" const char DAT_002d9dd4[], DAT_002d9dac[], DAT_002d9d9c[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_002164c4(DAT_002d9dd4, DAT_002d9dac, DAT_002d9d9c, line); for (;;) {} } } while (0)
struct E_002248ac { char b[560]; };
struct V_002248ac {
    char pad[20]; E_002248ac *data; unsigned n; int pad2;
    E_002248ac &operator[](int i) { HST_ASSERT(i >= 0 && (unsigned)i < n, 235); return data[i]; }
};
extern "C" V_002248ac *DAT_003f39dc;
struct S_002248ac { char pad[344]; int k; int pad2; int i; };
extern "C" E_002248ac *FUN_002248ac(S_002248ac *s)
{
    return &DAT_003f39dc[s->k][s->i];
}
