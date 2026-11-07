/* HSTMATCH module=hstp_trans_main.prx start=0x001cd780 end=0x001cd7f8 symbol=FUN_001cd780 lang=c++ */
/* Hand-written reconstruction: clone of the source of set_have_001cd7f8.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
/* menuglobal.cpp (debug build, line 2005): stores the "have" state of item idx (0..639, byte table at +0xd4c,
   the inline range-checked accessor of the EBOOT indexed_slot_address_0016b390 family) after asserting
   0 <= p_have < eDAT_HAVE_NUM (7). FUN_001c9440 is the module's empty variadic assert printer. Names provisional. */
extern "C" {
extern const char DAT_00391910[];   /* "assertion \"%s\" failed: file \"%s\", line %d\n" */
extern const char DAT_00391964[];   /* "menuglobal.cpp" */
extern const char DAT_00391c24[];   /* "0<=p_have&&p_have<eDAT_HAVE_NUM" */
void FUN_001c9440(...);
}
#define HST_ASSERT(cond, text, line) do { if (!(cond)) { FUN_001c9440(DAT_00391910, text, DAT_00391964, line); for (;;) {} } } while (0)

struct Block {
    char pad[0xacc];
    char slots[640];
    char *Slot(int idx)
    {
        if (idx >= 0 && idx < 640)
            return &slots[idx];
        return 0;
    }
};

extern "C" void FUN_001cd780(Block *b, int idx, int have)
{
    char *p = b->Slot(idx);
    if (p) {
        HST_ASSERT(0 <= have && have < 7, DAT_00391c24, 1980);
        *p = have;
    }
}
