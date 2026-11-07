/* HSTMATCH module=hstp_trans_main.prx start=0x001f5300 end=0x001f5358 symbol=FUN_001f5300 lang=c++ */
/* Hand-written reconstruction: clone of the source of active_call_001e70e8.cpp in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x1b30]; int state; char pad1[0x1d64 - 0x1b34]; void *target; };
extern "C" int FUN_0022d1f8(void *target);
inline bool Active(Obj *o) { switch (o->state) { case 1: case 2: case 3: return true; } return false; }
extern "C" int FUN_001f5300(Obj *self) { if (Active(self)) return FUN_0022d1f8(self->target); return 1; }
