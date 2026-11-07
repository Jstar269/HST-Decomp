/* HSTMATCH module=hstp_trans_main.prx start=0x0006e458 end=0x0006e4c4 symbol=FUN_0006e458 lang=c++ */
/* Hand-written reconstruction from the disassembly, adapted from the twin src/eboot/copy_ctor2_0006ce70.cpp in the other module: addresses substituted; exact match verified; names and types await human review. */ /* matching idiom: a tag temporary whose class derives from another empty class is passed from a never-written frame slot. */
struct in_tag_0006ce70 {};
struct fwd_tag_0006ce70 : in_tag_0006ce70 {};
extern "C" int FUN_0006e4fc(void *o);
extern "C" void FUN_0006e4e8(void *t, int a);
extern "C" void *FUN_0006e4e0(void *o);
extern "C" void *FUN_0006e4c4(void *o);
extern "C" void FUN_0006c508(void *t, void *f, void *l, fwd_tag_0006ce70);
extern "C" void *FUN_0006e458(void *t, void *o)
{
    FUN_0006e4e8(t, FUN_0006e4fc(o));
    FUN_0006c508(t, FUN_0006e4e0(o), FUN_0006e4c4(o), fwd_tag_0006ce70());
    return t;
}
