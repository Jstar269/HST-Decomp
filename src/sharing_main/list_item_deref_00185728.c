/* HSTMATCH module=hstp_trans_main.prx start=0x00185728 end=0x00185780 symbol=FUN_00185728 lang=c99 */
/* Hand-written reconstruction from the disassembly; exact match verified; names and types await human review. */
extern void FUN_00177f38();
extern const char DAT_00387a88[], DAT_00387c04[], DAT_00387ac4[];
#define HST_ASSERT(cond, line) do { if (!(cond)) { FUN_00177f38(DAT_00387a88, DAT_00387c04, DAT_00387ac4, line); for (;;) {} } } while (0)
struct L { int **data; int size; };
struct M { char pad[0x1d4]; struct L *list; };
int FUN_00185728(struct M *m, int i) { HST_ASSERT(m->list, 7737); return *m->list->data[i]; }
