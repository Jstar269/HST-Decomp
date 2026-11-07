/* HSTMATCH module=EBOOT.BIN start=0x0006ce70 end=0x0006cedc symbol=FUN_0006ce70 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */ /* matching idiom: a tag temporary whose class derives from another empty class is passed from a never-written frame slot (lb a3,28(sp)). */
struct in_tag_0006ce70 {};
struct fwd_tag_0006ce70 : in_tag_0006ce70 {};
extern "C" int FUN_0006cf14(void *o);
extern "C" void FUN_0006cf00(void *t, int a);
extern "C" void *FUN_0006cef8(void *o);
extern "C" void *FUN_0006cedc(void *o);
extern "C" void FUN_0006af20(void *t, void *f, void *l, fwd_tag_0006ce70);
extern "C" void *FUN_0006ce70(void *t, void *o)
{
    FUN_0006cf00(t, FUN_0006cf14(o));
    FUN_0006af20(t, FUN_0006cef8(o), FUN_0006cedc(o), fwd_tag_0006ce70());
    return t;
}
