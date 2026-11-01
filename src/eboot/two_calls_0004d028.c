/* HSTMATCH module=EBOOT.BIN start=0x0004d028 end=0x0004d084 symbol=FUN_0004d028 lang=c99 */
/* Hand-written reconstruction (external matching lane); exact match verified; names and types await human review. */
extern int DAT_00311158;
extern unsigned char DAT_0030fbfd;
extern char DAT_0030ab9c[];
extern void FUN_0001c604(void *);
extern void FUN_0001c1fc(void *);
extern void FUN_002b7720(void);

void FUN_0004d028(void)
{
  if ((DAT_00311158 == 0) && (DAT_0030fbfd != 0)) {
    FUN_0001c604(&DAT_0030ab9c);
    FUN_0001c1fc(&DAT_0030ab9c);
  }
  else {
    FUN_002b7720();
  }
}
