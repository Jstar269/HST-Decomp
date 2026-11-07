/* HSTMATCH module=hstp_trans_main.prx start=0x0013c0c8 end=0x0013c0f4 symbol=FUN_0013c0c8 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x120]; int value; };
extern "C" int DAT_003823ac;
inline const int &Max(const int &a, const int &b) { return a < b ? b : a; }
extern "C" float FUN_0013c0c8(Obj *self) { return (float)Max(self->value, DAT_003823ac); }
