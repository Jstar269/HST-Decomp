/* HSTMATCH module=hstp_trans_main.prx start=0x00132344 end=0x0013244c symbol=FUN_00132344 lang=c99 helpers=FUN_00131f8c@0x00131f8c-0x00131fb8 */
/* Hand-written reconstruction from the disassembly (same-file static log stub FUN_00131f8c placed by helpers=; the first range forwards param_2 with -1/0); exact match verified; names and types await human review. */
/* Ids below MOTION_PLAIN_END are kinds of their own (variant -1); each later range is one kind (6 to 10) whose
   variant is the id's offset inside the range. Out-of-range ids are reported through the log stub. */
enum {
    MOTION_PLAIN_END = 6,
    MOTION_KIND6_END = 0xe,
    MOTION_KIND7_END = 0x14,
    MOTION_KIND8_END = 0x1c,
    MOTION_KIND9_END = 0x26,
    MOTION_KIND10_END = 0x29
};
extern char DAT_00381898[];
static void FUN_00131f8c(...) {}
extern void FUN_001354c4(int, ...);

void FUN_00132344(int obj, int id)
{
    if (id < 0)
        FUN_00131f8c(DAT_00381898);
    if (id < MOTION_PLAIN_END)
        FUN_001354c4(obj, id, -1, 0);
    else if (id < MOTION_KIND6_END)
        FUN_001354c4(obj, 6, id - MOTION_PLAIN_END, 0);
    else if (id < MOTION_KIND7_END)
        FUN_001354c4(obj, 7, id - MOTION_KIND6_END, 0);
    else if (id < MOTION_KIND8_END)
        FUN_001354c4(obj, 8, id - MOTION_KIND7_END, 0);
    else if (id < MOTION_KIND9_END)
        FUN_001354c4(obj, 9, id - MOTION_KIND8_END, 0);
    else if (id < MOTION_KIND10_END)
        FUN_001354c4(obj, 10, id - MOTION_KIND9_END, 0);
    else
        FUN_00131f8c(DAT_00381898);
}
