/* HSTMATCH module=EBOOT.BIN start=0x001e7140 end=0x001e7198 symbol=FUN_001e7140 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Obj { char pad[0x1b30]; int state; char pad1[0x1d64 - 0x1b34]; void *target; };
extern "C" int FUN_0021dd0c(void *target);
inline bool Active(Obj *o) { switch (o->state) { case 1: case 2: case 3: return true; } return false; }
extern "C" int FUN_001e7140(Obj *self) { if (Active(self)) return FUN_0021dd0c(self->target); return 1; }
