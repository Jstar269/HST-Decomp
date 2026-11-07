/* HSTMATCH module=hstp_trans_main.prx start=0x0029a168 end=0x0029a220 symbol=FUN_0029a168 lang=c99 */
/* Hand-written reconstruction: clone of the source of create_object_002889cc.c in the other module, addresses substituted by a script (try/ow1104b/twinclone.py); exact match verified; names and types await human review. */
/* Hand-written reconstruction from the disassembly; matching idiom: reuse the allocated object pointer as the constructor result; exact match verified; names and types await human review. */
#define OBJECT_SIZE 0x90 /* bytes allocated for the new object */

extern void *DAT_003f344c;
extern int DAT_003a7a0c[];
extern int DAT_003a7a30[];
extern int FUN_001d54e8(void *, int);
extern void FUN_00297e90(int *, int, int);
extern int FUN_00000bcc(int);
extern int FUN_002a4a48(int);
extern void FUN_002a4b70(int, int);
extern void FUN_001d4f60(void *, int, int);

/* If the global container already has `key`, configure from the first table; otherwise use the second,
   create a new object, initialise it and store it under `key`. */
void FUN_0029a168(int key, int arg)
{
  int obj;
  int r;

  if (FUN_001d54e8(DAT_003f344c, key) != 0) {
    FUN_00297e90(DAT_003a7a0c, key, arg);
  }
  else {
    FUN_00297e90(DAT_003a7a30, key, arg);
    obj = FUN_00000bcc(OBJECT_SIZE);
    if (obj != 0) obj = FUN_002a4a48(obj);
    r = obj;
    FUN_002a4b70(r, arg);
    FUN_001d4f60(DAT_003f344c, key, r);
  }
}
