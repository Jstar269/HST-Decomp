/* HSTMATCH module=hstp_trans_main.prx start=0x00170ab4 end=0x00170b24 symbol=FUN_00170ab4 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct E_00170ab4 { int a, b; float f; int c, d; };
struct V_00170ab4 { E_00170ab4 *data; int n; E_00170ab4 &back() { return data[n - 1]; } };
extern "C" V_00170ab4 *DAT_003ea850;
extern "C" float FUN_001703f0(void *p, int z);
extern "C" int FUN_00170ab4(void *p)
{
    float x = DAT_003ea850->back().f;
    return (int)(0.5f + 2.0f * (x - FUN_001703f0(p, 0)));
}
