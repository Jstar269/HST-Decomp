/* HSTMATCH module=EBOOT.BIN start=0x0022ca58 end=0x0022ca9c symbol=FUN_0022ca58 lang=c++ */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
struct Entry { unsigned char kind; char pad[55]; };
struct Table { int count; Entry e[15]; };
extern "C" Table *DAT_00341ccc;
inline Entry *entry_at(int i) { Table *t = DAT_00341ccc; if (i >= 0 && i < 15) return &t->e[i]; return 0; }
extern "C" bool FUN_0022ca58(int i) { return (unsigned)(entry_at(i)->kind - 4) < 2; }
